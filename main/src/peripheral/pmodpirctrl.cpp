#include "pmodpirctrl.h"
#include "logger.h"
#include "driver/gpio.h"

CPmodPIRCtrl* CPmodPIRCtrl::_instance = nullptr;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    CPmodPIRCtrl *obj = reinterpret_cast<CPmodPIRCtrl*>(arg);
    uint8_t gpio_num = obj->get_gpio_num();
    uint8_t level = (uint8_t)gpio_get_level((gpio_num_t)gpio_num);
    obj->set_callback(level);
}

CPmodPIRCtrl::CPmodPIRCtrl()
{
    m_gpio_num = 0;
    m_callback_ptr = nullptr;
}

CPmodPIRCtrl::~CPmodPIRCtrl()
{
    gpio_isr_handler_remove((gpio_num_t)m_gpio_num);
}

CPmodPIRCtrl* CPmodPIRCtrl::Instance()
{
    if (!_instance) {
        _instance = new CPmodPIRCtrl();
    }

    return _instance;
}

bool CPmodPIRCtrl::initialize(uint8_t gpio_num)
{
    esp_err_t ret;
    m_gpio_num = gpio_num;

    gpio_config_t cfg = {};
    cfg.pin_bit_mask = 1ULL << m_gpio_num;
    cfg.mode = GPIO_MODE_INPUT;
    cfg.pull_up_en = GPIO_PULLUP_DISABLE;
    cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
    cfg.intr_type = GPIO_INTR_ANYEDGE;  // detect both occupancy and no occupancy
    ret = gpio_config(&cfg);
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to configure GPIO (ret: %d)", ret);
        return false;
    }

    ret = gpio_install_isr_service(0);
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to install ISR service (ret: %d)", ret);
        return false;
    }

    ret = gpio_isr_handler_add((gpio_num_t)m_gpio_num, gpio_isr_handler, (void*)this);
    if (ret != ESP_OK) {
        GetLogger(eLogType::Error)->Log("Failed to add ISR handler (ret: %d)", ret);
        return false;
    }

    return true;
}

void CPmodPIRCtrl::register_callback_gpio_change(fn_gpio_change_callback callback)
{
    m_callback_ptr = std::move(callback);
}

uint8_t CPmodPIRCtrl::get_gpio_num()
{
    return m_gpio_num;
}

void CPmodPIRCtrl::set_callback(uint8_t level)
{
    if (m_callback_ptr) {
        m_callback_ptr(level);
    }
}