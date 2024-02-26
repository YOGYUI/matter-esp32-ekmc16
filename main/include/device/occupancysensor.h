#pragma once
#ifndef _OCCUPANCY_SENSOR_H_
#define _OCCUPANCY_SENSOR_H_

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

class COccupancySensor : public CDevice
{
public:
    COccupancySensor();

    bool matter_init_endpoint() override;
    bool matter_config_attributes() override;
    void matter_on_change_attribute_value(
        esp_matter::attribute::callback_type_t type,
        uint32_t cluster_id,
        uint32_t attribute_id,
        esp_matter_attr_val_t *value
    ) override;
    void matter_update_all_attribute_values() override;

public:
    void update_occupancy(uint8_t value) override;

private:
    bool m_matter_update_by_client_clus_occupancy_attr_occupancy;

    void matter_update_clus_occupancy_attr_occupancy(bool force_update = false);
};

#ifdef __cplusplus
};
#endif
#endif