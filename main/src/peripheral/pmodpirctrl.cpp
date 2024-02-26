#include "pmodpirctrl.h"
#include "logger.h"
#include "driver/gpio.h"
#include "definition.h"

CPmodPIRCtrl* CPmodPIRCtrl::_instance = nullptr;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    CPmodPIRCtrl *obj = reinterpret_cast<CPmodPIRCtrl*>(arg);
    QueueHandle_t queue = obj->get_queue_isr();
    uint8_t temp = 0;
    xQueueSendFromISR(queue, &temp, nullptr);
}

CPmodPIRCtrl::CPmodPIRCtrl()
{
    m_gpio_num = 0;
    m_callback_ptr = nullptr;
    m_queue_isr = xQueueCreate(10, sizeof(uint8_t));
    m_keepalive = true;
    xTaskCreate(task_gpio_isr_function, "PMOD_PIR_TASK", TASK_STACK_DEPTH, this, 10, &m_task_handle_isr);
}

CPmodPIRCtrl::~CPmodPIRCtrl()
{
    m_keepalive = false;
    gpio_isr_handler_remove((gpio_num_t)m_gpio_num);
    m_callback_ptr = nullptr;
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
    cfg.pull_up_en = GPIO_PULLUP_ENABLE;
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

QueueHandle_t CPmodPIRCtrl::get_queue_isr()
{
    return m_queue_isr;
}

void CPmodPIRCtrl::set_callback(uint8_t level)
{
    if (m_callback_ptr) {
        m_callback_ptr(level);
    }
}

void CPmodPIRCtrl::task_gpio_isr_function(void *param)
{
    CPmodPIRCtrl *obj = reinterpret_cast<CPmodPIRCtrl*>(param);
    uint8_t temp = 0;

    while (obj->m_keepalive) {
        if (xQueueReceive(obj->m_queue_isr, (void *)&temp, 1) == pdTRUE) {
            uint8_t level = (uint8_t)gpio_get_level((gpio_num_t)obj->m_gpio_num);
            GetLogger(eLogType::Info)->Log("GPIO Level changed to %u", level);
            obj->set_callback(level);
        }
    }
    vTaskDelete(nullptr);
}