#pragma once
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_matter.h>
#include <esp_matter_core.h>
#include <iot_button.h>
#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

class CSystem
{
public:
    CSystem();
    virtual ~CSystem();
    static CSystem* Instance();

public:
    bool initialize();
    void release();

    esp_matter::node_t* get_root_node() { return m_root_node; }
    void factory_reset();
    bool matter_set_min_endpoint_id(uint16_t endpoint_id);
    bool matter_align_endpoint_id();
    uint16_t matter_get_vendor_id();
    uint16_t matter_get_product_id();
    uint32_t matter_get_setup_passcode();
    uint16_t matter_get_setup_discriminator();

    CDevice* find_device_by_endpoint_id(uint16_t endpoint_id);

private:
    static CSystem* _instance;
    bool m_initialized;

    esp_matter::node_t* m_root_node;
    std::vector<CDevice*> m_device_list;

    button_handle_t m_handle_default_btn;
    static bool m_default_btn_pressed_long;
    static bool m_commisioning_session_working;
    
    bool init_default_button();
    bool deinit_default_button();
    static void callback_default_button(void *arg, void *data);
    void print_system_info();
    void print_matter_endpoints_info();

    static void matter_event_callback(const ChipDeviceEvent *event, intptr_t arg);
    static esp_err_t matter_identification_callback(
        esp_matter::identification::callback_type_t type, 
        uint16_t endpoint_id, 
        uint8_t effect_id, 
        uint8_t effect_variant, 
        void *priv_data
    );
    static esp_err_t matter_attribute_update_callback(
        esp_matter::attribute::callback_type_t type, 
        uint16_t endpoint_id, 
        uint32_t cluster_id, 
        uint32_t attribute_id, 
        esp_matter_attr_val_t *val, 
        void *priv_data
    );

    void on_pmod_pir_level_changed(uint8_t level);
};

inline CSystem* GetSystem() {
    return CSystem::Instance();
}

#ifdef __cplusplus
};
#endif
#endif