#include "system.h"
#include <nvs_flash.h>
#include <esp_netif.h>
#include <esp_mac.h>
#include <esp_chip_info.h>
#include <esp_flash.h>
#include <esp_app_desc.h>
#include <app/server/Server.h>
#include <esp_matter_providers.h>
#include "cJSON.h"
#include "util.h"
#include "logger.h"
#include "definition.h"
#include "occupancysensor.h"
#include "pmodpirctrl.h"

#define TASK_TIMER_STACK_DEPTH  3072
#define TASK_TIMER_PRIORITY     5
#define MEASURE_PERIOD_US       10000000

CSystem* CSystem::_instance = nullptr;
bool CSystem::m_default_btn_pressed_long = false;
bool CSystem::m_commisioning_session_working = false;

typedef struct matter_node {
    void *endpoint_list;
    uint16_t min_unused_endpoint_id;
} matter_node_t;

CSystem::CSystem() 
{
    m_root_node = nullptr;
    m_handle_default_btn = nullptr;
    m_device_list.clear();
    m_initialized = false;
}

CSystem::~CSystem()
{
    if (_instance) {
        delete _instance;
        _instance = nullptr;
    }
}

CSystem* CSystem::Instance()
{
    if (!_instance) {
        _instance = new CSystem();
    }

    return _instance;
}

bool CSystem::initialize()
{
    GetLogger(eLogType::Info)->Log("Start Initializing System");
 
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to initialize nsv flash (%d)", ret);
        return false;
    }

    if (!init_default_button()) {
        GetLogger(eLogType::Warning)->Log("Failed to init default on-board button");
    }

    if (!GetPmodPIRCtrl()->initialize(GPIO_PIN_SENSOR_SIGNAL)) {
        return false;
    } else {
        GetPmodPIRCtrl()->register_callback_gpio_change(std::bind(&CSystem::on_pmod_pir_level_changed, this, std::placeholders::_1));
    }
    
    // create matter root node
    esp_matter::node::config_t node_config;
    snprintf(node_config.root_node.basic_information.node_label, sizeof(node_config.root_node.basic_information.node_label), PRODUCT_NAME);
    m_root_node = esp_matter::node::create(&node_config, matter_attribute_update_callback, matter_identification_callback);
    if (!m_root_node) {
        GetLogger(eLogType::Error)->Log("Failed to create root node");
        return false;
    }
    GetLogger(eLogType::Info)->Log("Root node (endpoint 0) added");

    // start matter
    ret = esp_matter::start(matter_event_callback);
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to start matter (ret: %d)", ret);
        return false;
    }
    // prevent endpoint id increment when board reset
    matter_set_min_endpoint_id(1);
    GetLogger(eLogType::Info)->Log("Matter started");

    // add occupancy sensor endpoint
    COccupancySensor *sensor = new COccupancySensor();
    if (sensor && sensor->matter_init_endpoint()) {
        m_device_list.push_back(sensor);
    } else {
        return false;
    }

    m_initialized = true;
    GetLogger(eLogType::Info)->Log("Initialized");
    // print_system_info();
    // print_matter_endpoints_info();
    
    return true;
}

void CSystem::release()
{
    deinit_default_button();
    m_initialized = false;
}

void CSystem::callback_default_button(void *arg, void *data)
{
    button_event_t event = iot_button_get_event(arg);

//  GetLogger(eLogType::Info)->Log("button callback event: %d", event);
    switch (event) {
    case BUTTON_PRESS_DOWN: // 0
        break;
    case BUTTON_PRESS_UP:   // 1
        if (m_default_btn_pressed_long) {
            _instance->factory_reset();
        }
        m_default_btn_pressed_long = false;
        break;
    case BUTTON_SINGLE_CLICK:   // 4
        _instance->print_system_info();
        break;
    case BUTTON_DOUBLE_CLICK:   // 5
        _instance->print_matter_endpoints_info();
        break;
    case BUTTON_LONG_PRESS_START:   // 6
        m_default_btn_pressed_long = true;
        GetLogger(eLogType::Info)->Log("ready to factory reset");
        break;
    case BUTTON_LONG_PRESS_HOLD:    // 7
        m_default_btn_pressed_long = true;
        break;
    default:
        break;
    }
}

bool CSystem::init_default_button()
{
    button_config_t cfg = button_config_t();
    cfg.type = BUTTON_TYPE_GPIO;
    cfg.long_press_time = 5000;
    cfg.short_press_time = 180;
    cfg.gpio_button_config.gpio_num = GPIO_PIN_DEFAULT_BTN;
    cfg.gpio_button_config.active_level = 0; // active low (zero level when pressed)

    m_handle_default_btn = iot_button_create(&cfg);
    if (!m_handle_default_btn) {
        GetLogger(eLogType::Error)->Log("Failed to create iot button");
        return false;
    }

    iot_button_register_cb(m_handle_default_btn, BUTTON_PRESS_DOWN, callback_default_button, nullptr);
    iot_button_register_cb(m_handle_default_btn, BUTTON_PRESS_UP, callback_default_button, nullptr);
    iot_button_register_cb(m_handle_default_btn, BUTTON_SINGLE_CLICK, callback_default_button, nullptr);
    iot_button_register_cb(m_handle_default_btn, BUTTON_DOUBLE_CLICK, callback_default_button, nullptr);
    iot_button_register_cb(m_handle_default_btn, BUTTON_LONG_PRESS_START, callback_default_button, nullptr);
    iot_button_register_cb(m_handle_default_btn, BUTTON_LONG_PRESS_HOLD, callback_default_button, nullptr);
    
    return true;
}

bool CSystem::deinit_default_button()
{
    if (m_handle_default_btn) {
        iot_button_delete(m_handle_default_btn);
        return true;
    }

    return false;
}

uint16_t CSystem::matter_get_vendor_id()
{
    uint16_t vendor_id = 0;
    chip::DeviceLayer::DeviceInstanceInfoProvider *dev_info_provider = chip::DeviceLayer::GetDeviceInstanceInfoProvider();
    if (dev_info_provider) {
        chip::ChipError ret = dev_info_provider->GetVendorId(vendor_id);
        if (ret != CHIP_NO_ERROR) {
            GetLogger(eLogType::Error)->Log("Failed to get vendor ID");
        }
    }
    return vendor_id;
}

uint16_t CSystem::matter_get_product_id()
{
    uint16_t product_id = 0;
    chip::DeviceLayer::DeviceInstanceInfoProvider *dev_info_provider = chip::DeviceLayer::GetDeviceInstanceInfoProvider();
    if (dev_info_provider) {
        chip::ChipError ret = dev_info_provider->GetProductId(product_id);
        if (ret != CHIP_NO_ERROR) {
            GetLogger(eLogType::Error)->Log("Failed to get product ID");
        }
    }
    return product_id;
}

uint32_t CSystem::matter_get_setup_passcode()
{
    uint32_t passcode = 0;
    chip::DeviceLayer::CommissionableDataProvider *comm_data_provider = chip::DeviceLayer::GetCommissionableDataProvider();
    if (comm_data_provider) {
        chip::ChipError ret = comm_data_provider->GetSetupPasscode(passcode);
        if (ret != CHIP_NO_ERROR) {
            GetLogger(eLogType::Error)->Log("Failed to get setup passcode");
        }
    }
    return passcode;
}

uint16_t CSystem::matter_get_setup_discriminator()
{
    uint16_t discriminator;
    chip::DeviceLayer::CommissionableDataProvider *comm_data_provider = chip::DeviceLayer::GetCommissionableDataProvider();
    if (comm_data_provider) {
        chip::ChipError ret = comm_data_provider->GetSetupDiscriminator(discriminator);
        if (ret != CHIP_NO_ERROR) {
            GetLogger(eLogType::Error)->Log("Failed to get setup discriminator");
        }
    }
    return discriminator;
}

void CSystem::factory_reset()
{
    esp_err_t ret;
    
    ret = esp_matter::factory_reset();
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to matter factory reset (ret: %d)", ret);
    }
}

void CSystem::print_system_info()
{
    GetLogger(eLogType::Info)->Log("System Info");

    // ESP32 specific
    GetLoggerM(eLogType::Info)->Log("----- ESP32 -----");
    auto desc = esp_app_get_description();
    GetLoggerM(eLogType::Info)->Log("Project Name: %s", desc->project_name);
    GetLoggerM(eLogType::Info)->Log("App Version: %s", desc->version);

    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    GetLoggerM(eLogType::Info)->Log("CPU Core(s): %d", chip_info.cores);
    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    GetLoggerM(eLogType::Info)->Log("Revision: %d.%d", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) == ESP_OK) {
        float flash_size_mb = (float)flash_size / (1024.f * 1024.f);
        GetLoggerM(eLogType::Info)->Log("Flash Size: %g MB (%s)", flash_size_mb, (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");
    }
    size_t heap_free_size = heap_caps_get_free_size(MALLOC_CAP_8BIT);
    GetLoggerM(eLogType::Info)->Log("Heap Free Size: %d", heap_free_size);

    // network interface
    GetLoggerM(eLogType::Info)->Log("----- Network -----");
    esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");   // "WIFI_AP_DEF"
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(netif, &ip_info);
    GetLoggerM(eLogType::Info)->Log("IPv4 Address: %d.%d.%d.%d", IP2STR(&ip_info.ip));
    GetLoggerM(eLogType::Info)->Log("Gateway: %d.%d.%d.%d", IP2STR(&ip_info.gw));
    unsigned char mac[6] = {0};
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    GetLoggerM(eLogType::Info)->Log("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    // matter related information
    GetLoggerM(eLogType::Info)->Log("----- Matter -----");
    GetLoggerM(eLogType::Info)->Log("Vendor ID: 0x%04X", matter_get_vendor_id());
    GetLoggerM(eLogType::Info)->Log("Product ID: 0x%04X", matter_get_product_id());
    // GetLoggerM(eLogType::Info)->Log("Setup Passcode: %d", matter_get_setup_passcode());
    GetLoggerM(eLogType::Info)->Log("Setup Discriminator: %d", matter_get_setup_discriminator());
}

void CSystem::print_matter_endpoints_info()
{
    if (!m_root_node)
        return;

    uint16_t endpoint_id;
    GetLogger(eLogType::Info)->Log("Matter Data Model Information");
    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get_first(m_root_node);
    while (endpoint != nullptr) {
        endpoint_id = esp_matter::endpoint::get_id(endpoint);
        cJSON *json = dump_matter_endpoint_info(endpoint_id);
        char *string = cJSON_PrintUnformatted(json);
        printf(string);
        cJSON_Delete(json);
        free(string);
        printf("\n");
        endpoint = esp_matter::endpoint::get_next(endpoint);
    }
}

bool CSystem::matter_set_min_endpoint_id(uint16_t endpoint_id)
{
    if (!m_root_node)
        return false;
    matter_node_t *temp = (matter_node_t *)m_root_node;
    temp->min_unused_endpoint_id = endpoint_id;
    GetLogger(eLogType::Info)->Log("Set minimum endpoint id as %u", endpoint_id);
    return true;
}

bool CSystem::matter_align_endpoint_id()
{
    if (!m_root_node)
        return false;

    uint16_t endpoint_id;
    uint16_t max_endpoint_id = 0;
    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get_first(m_root_node);
    while (endpoint != nullptr) {
        endpoint_id = esp_matter::endpoint::get_id(endpoint);
        if (endpoint_id > max_endpoint_id) {
            max_endpoint_id = endpoint_id;
        }
        endpoint = esp_matter::endpoint::get_next(endpoint);
    }
    GetLogger(eLogType::Info)->Log("Current max endpoint id is %u", max_endpoint_id);

    return matter_set_min_endpoint_id(max_endpoint_id + 1);
}

CDevice* CSystem::find_device_by_endpoint_id(uint16_t endpoint_id)
{
    for (auto & dev : m_device_list) {
        if (dev->matter_get_endpoint_id() == endpoint_id) {
            return dev;
        }
    }

    return nullptr;
}

void CSystem::matter_event_callback(const ChipDeviceEvent *event, intptr_t arg)
{
    switch (event->Type) {
    case chip::DeviceLayer::DeviceEventType::kInterfaceIpAddressChanged:
        if (event->InterfaceIpAddressChanged.Type == chip::DeviceLayer::InterfaceIpChangeType::kIpV6_Assigned) {
            GetLogger(eLogType::Info)->Log("IP Address(v6) assigned");
        } else if (event->InterfaceIpAddressChanged.Type == chip::DeviceLayer::InterfaceIpChangeType::kIpV4_Assigned) {
            // IPv4 주소를 할당받으면 (commision된 AP에서 주소 할당) 웹서버를 (재)시작해준다 
            GetLogger(eLogType::Info)->Log("IP Address(v4) assigned");
        }
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningComplete:
        GetLogger(eLogType::Info)->Log("Commissioning complete");
        m_commisioning_session_working = false;
        break;
    case chip::DeviceLayer::DeviceEventType::kFailSafeTimerExpired:
        GetLogger(eLogType::Error)->Log("Commissioning failed, fail safe timer expired");
        m_commisioning_session_working = false;
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStarted:
        GetLogger(eLogType::Info)->Log("Commissioning session started");
        m_commisioning_session_working = true;
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningSessionStopped:
        GetLogger(eLogType::Info)->Log("Commissioning session stopped");
        m_commisioning_session_working = false;
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowOpened:
        GetLogger(eLogType::Info)->Log("Commissioning window opened");
        break;
    case chip::DeviceLayer::DeviceEventType::kCommissioningWindowClosed:
        GetLogger(eLogType::Info)->Log("Commissioning window closed");
        break;
    default:
        break;
    }
}

esp_err_t CSystem::matter_identification_callback(esp_matter::identification::callback_type_t type,  uint16_t endpoint_id, uint8_t effect_id, uint8_t effect_variant, void *priv_data)
{
    GetLogger(eLogType::Info)->Log("Identification callback > type: %d, endpoint_id: %d, effect_id: %d, effect_variant: %d", type, endpoint_id, effect_id, effect_variant);
    
    return ESP_OK;
}

esp_err_t CSystem::matter_attribute_update_callback(esp_matter::attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data)
{
    CDevice *device = GetSystem()->find_device_by_endpoint_id(endpoint_id);
    if (device){
        device->matter_on_change_attribute_value(type, cluster_id, attribute_id, val);
    }
    
    return ESP_OK;
}

void CSystem::on_pmod_pir_level_changed(uint8_t level)
{
    CDevice *dev = find_device_by_endpoint_id(1);
    if (dev) {
        dev->update_occupancy(level);
    }
}