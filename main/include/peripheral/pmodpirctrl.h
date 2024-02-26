#pragma once
#ifndef _PMOD_PIR_H_
#define _PMOD_PIR_H_

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include <stdint.h>
#include <functional>

#ifdef __cplusplus
extern "C" {
#endif

typedef std::function<void(uint8_t)> fn_gpio_change_callback;

class CPmodPIRCtrl
{
public:
    CPmodPIRCtrl();
    virtual ~CPmodPIRCtrl();
    static CPmodPIRCtrl* Instance();

public:
    bool initialize(uint8_t gpio_num);
    void register_callback_gpio_change(fn_gpio_change_callback callback);
    void set_callback(uint8_t level);
    QueueHandle_t get_queue_isr();

private:
    static CPmodPIRCtrl* _instance;
    bool m_keepalive;
    uint8_t m_gpio_num;
    fn_gpio_change_callback m_callback_ptr;
    QueueHandle_t m_queue_isr;
    TaskHandle_t m_task_handle_isr;

    static void task_gpio_isr_function(void *param);
};

inline CPmodPIRCtrl* GetPmodPIRCtrl() {
    return CPmodPIRCtrl::Instance();
}

#ifdef __cplusplus
};
#endif
#endif