#pragma once
#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <stdint.h>
#include <esp_matter.h>
#include <esp_matter_core.h>

#ifdef __cplusplus
extern "C" {
#endif

class CDevice
{
public:
    CDevice();
    virtual ~CDevice();

protected:
    esp_matter::endpoint_t *m_endpoint;
    uint16_t m_endpoint_id;

public:
    virtual bool matter_init_endpoint();
    virtual bool matter_config_attributes();
    bool matter_destroy_endpoint();
    esp_matter::endpoint_t* matter_get_endpoint();
    uint16_t matter_get_endpoint_id();
    virtual void matter_on_change_attribute_value(
        esp_matter::attribute::callback_type_t type,
        uint32_t cluster_id,
        uint32_t attribute_id,
        esp_matter_attr_val_t *value
    );
    virtual void matter_update_all_attribute_values();

protected:
    bool matter_get_attribute_value(
        uint16_t endpoint_id,
        uint32_t cluster_id,
        uint32_t attribute_id,
        esp_matter_attr_val_t *value
    );

    void matter_update_cluster_attribute_common(
        uint16_t endpoint_id, 
        uint32_t cluster_id, 
        uint32_t attribute_id, 
        esp_matter_attr_val_t target_value,
        bool* updating_flag,
        bool force_update = false
    );

public:
    virtual void update_occupancy(uint8_t value);

protected:
    uint8_t m_occupancy;
    uint8_t m_occupancy_prev;
};

#ifdef __cplusplus
};
#endif
#endif