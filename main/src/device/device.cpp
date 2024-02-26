#include "device.h"
#include "logger.h"
#include "system.h"

CDevice::CDevice()
{
    m_endpoint = nullptr;
    m_endpoint_id = 0;
    m_occupancy = 0;
    m_occupancy_prev = 0;
}

CDevice::~CDevice()
{
}

bool CDevice::matter_init_endpoint()
{
    esp_err_t ret;
    
    if (m_endpoint != nullptr) {
        matter_config_attributes();

        // get endpoint id
        m_endpoint_id = esp_matter::endpoint::get_id(m_endpoint);

        ret = esp_matter::endpoint::enable(m_endpoint);  // should be called after esp_matter::start()
        if (ret != ESP_OK) {
            GetLogger(eLogType::Error)->Log("Failed to enable endpoint (%d, ret=%d)", m_endpoint_id, ret);
            matter_destroy_endpoint();
            return false;
        }
    } else {
        GetLogger(eLogType::Error)->Log("endpoint instance is null!");
        return false;
    }

    return true;
}

bool CDevice::matter_config_attributes()
{
    return true;
}

bool CDevice::matter_destroy_endpoint()
{
    esp_err_t ret;

    esp_matter::node_t *root = GetSystem()->get_root_node();
    ret = esp_matter::endpoint::destroy(root, m_endpoint);
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to destroy endpoint (%d)", ret);
        return false;
    }

    return true;
}

esp_matter::endpoint_t* CDevice::matter_get_endpoint() 
{ 
    return m_endpoint; 
}

uint16_t CDevice::matter_get_endpoint_id()
{ 
    return m_endpoint_id; 
}

void CDevice::matter_on_change_attribute_value(esp_matter::attribute::callback_type_t type, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *value)
{
    
}

void CDevice::matter_update_all_attribute_values()
{

}

bool CDevice::matter_get_attribute_value(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *value)
{
    esp_matter::node_t *root = GetSystem()->get_root_node();
    esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(root, endpoint_id);
    if (!endpoint) {
        GetLogger(eLogType::Error)->Log("Cannot find endpoint instance");
        return false;
    }
    esp_matter::cluster_t *cluster = esp_matter::cluster::get(endpoint, cluster_id);
    if (!cluster) {
        GetLogger(eLogType::Error)->Log("Cannot find cluster instance");
        return false;
    }
    esp_matter::attribute_t *attribute = esp_matter::attribute::get(cluster, attribute_id);
    if (!attribute) {
        GetLogger(eLogType::Error)->Log("Cannot find attribute instance");
        return false;
    }
    esp_err_t ret = esp_matter::attribute::get_val(attribute, value);
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to get value from <endpoint: %d, cluster: 0x%04X, attribute: 0x%04X> (ret: %d)", 
            endpoint_id, cluster_id, attribute_id, ret);
        return false;
    }

    return true;
}

void CDevice::matter_update_cluster_attribute_common(uint16_t endpoint_id, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t target_value, bool* updating_flag, bool force_update/*=false*/)
{
    bool value_diff = true;
    esp_matter_attr_val_t current_value = esp_matter_invalid(nullptr);
    if (!force_update) {
        if (matter_get_attribute_value(endpoint_id, cluster_id, attribute_id, &current_value)) {
            if (current_value.type != target_value.type) {
                GetLogger(eLogType::Error)->Log("Value type mismatch (cluster_id: 0x%04X, attribute_id: 0x%04X, from server: %d, argument: %d)",
                    cluster_id, attribute_id, current_value.type, target_value.type);
                return;
            }

            switch (current_value.type) {
            case ESP_MATTER_VAL_TYPE_INVALID:
                value_diff = false;
                break;
            case ESP_MATTER_VAL_TYPE_BOOLEAN:
            case ESP_MATTER_VAL_TYPE_NULLABLE_BOOLEAN:
                value_diff = current_value.val.b != target_value.val.b;
                break;
            case ESP_MATTER_VAL_TYPE_INTEGER:
            case ESP_MATTER_VAL_TYPE_NULLABLE_INTEGER:
                value_diff = current_value.val.i != target_value.val.i;
                break;
            case ESP_MATTER_VAL_TYPE_FLOAT:
            case ESP_MATTER_VAL_TYPE_NULLABLE_FLOAT:
                value_diff = current_value.val.f != target_value.val.f;
                break;
            case ESP_MATTER_VAL_TYPE_INT8:
            case ESP_MATTER_VAL_TYPE_NULLABLE_INT8:
                value_diff = current_value.val.i8 != target_value.val.i8;
                break;
            case ESP_MATTER_VAL_TYPE_UINT8:
            case ESP_MATTER_VAL_TYPE_NULLABLE_UINT8:
            case ESP_MATTER_VAL_TYPE_ENUM8:
            case ESP_MATTER_VAL_TYPE_NULLABLE_ENUM8:
            case ESP_MATTER_VAL_TYPE_BITMAP8:
            case ESP_MATTER_VAL_TYPE_NULLABLE_BITMAP8:
                value_diff = current_value.val.u8 != target_value.val.u8;
                break;
            case ESP_MATTER_VAL_TYPE_INT16:
            case ESP_MATTER_VAL_TYPE_NULLABLE_INT16:
                value_diff = current_value.val.i16 != target_value.val.i16;
                break;
            case ESP_MATTER_VAL_TYPE_UINT16:
            case ESP_MATTER_VAL_TYPE_NULLABLE_UINT16:
            case ESP_MATTER_VAL_TYPE_BITMAP16:
            case ESP_MATTER_VAL_TYPE_NULLABLE_BITMAP16:
            case ESP_MATTER_VAL_TYPE_ENUM16:
            case ESP_MATTER_VAL_TYPE_NULLABLE_ENUM16:
                value_diff = current_value.val.u16 != target_value.val.u16;
                break;
            case ESP_MATTER_VAL_TYPE_INT32:
            case ESP_MATTER_VAL_TYPE_NULLABLE_INT32:
                value_diff = current_value.val.i32 != target_value.val.i32;
                break;
            case ESP_MATTER_VAL_TYPE_UINT32:
            case ESP_MATTER_VAL_TYPE_NULLABLE_UINT32:
            case ESP_MATTER_VAL_TYPE_BITMAP32:
            case ESP_MATTER_VAL_TYPE_NULLABLE_BITMAP32:
                value_diff = current_value.val.u32 != target_value.val.u32;
                break;
            case ESP_MATTER_VAL_TYPE_INT64:
            case ESP_MATTER_VAL_TYPE_NULLABLE_INT64:
                value_diff = current_value.val.i64 != target_value.val.i64;
                break;
            case ESP_MATTER_VAL_TYPE_UINT64:
            case ESP_MATTER_VAL_TYPE_NULLABLE_UINT64:
                value_diff = current_value.val.u64 != target_value.val.u64;
                break;
            case ESP_MATTER_VAL_TYPE_CHAR_STRING:
            case ESP_MATTER_VAL_TYPE_OCTET_STRING:
                value_diff = strcmp((char *)current_value.val.a.b, (char *)target_value.val.a.b) != 0;
                break;
            case ESP_MATTER_VAL_TYPE_LONG_CHAR_STRING:
            case ESP_MATTER_VAL_TYPE_LONG_OCTET_STRING:
                // TODO: wide character handling
                value_diff = strcmp((char *)current_value.val.a.b, (char *)target_value.val.a.b) != 0;
                break;
            case ESP_MATTER_VAL_TYPE_ARRAY:
                break;
            }
        }
    }

    if (value_diff) {
        *updating_flag = true;

        esp_err_t ret = esp_matter::attribute::update(endpoint_id, cluster_id, attribute_id, &target_value);
        if (ret != ESP_OK) {
            GetLogger(eLogType::Error)->Log("Failed to update matter attribute (ret: %d)", ret);
        }
    }
}

void CDevice::update_occupancy(uint8_t value)
{
    m_occupancy = value;
}
