#include "occupancysensor.h"
#include "system.h"
#include "logger.h"

COccupancySensor::COccupancySensor()
{
    m_matter_update_by_client_clus_occupancy_attr_occupancy = false;
}

bool COccupancySensor::matter_init_endpoint()
{
    esp_matter::node_t *root = GetSystem()->get_root_node();
    esp_matter::endpoint::occupancy_sensor::config_t config_endpoint;
    config_endpoint.occupancy_sensing.occupancy = 0;
    config_endpoint.occupancy_sensing.occupancy_sensor_type = 0;  // 0 = PIR, 1 = Ultrasonic, 2 = PIRAndUltrasonic, 3 = PhysicalContact
    config_endpoint.occupancy_sensing.occupancy_sensor_type_bitmap = 0x01; // Bit 0 = PIR, Bit 1 = Ultrasonic, Bit 2 = PhysicalContact
    uint8_t flags = esp_matter::ENDPOINT_FLAG_DESTROYABLE;
    m_endpoint = esp_matter::endpoint::occupancy_sensor::create(root, &config_endpoint, flags, nullptr);
    if (!m_endpoint) {
        GetLogger(eLogType::Error)->Log("Failed to create endpoint");
        return false;
    }
    return CDevice::matter_init_endpoint();
}

bool COccupancySensor::matter_config_attributes()
{
    return true;
}

void COccupancySensor::matter_on_change_attribute_value(esp_matter::attribute::callback_type_t type, uint32_t cluster_id, uint32_t attribute_id, esp_matter_attr_val_t *value)
{
    if (cluster_id == chip::app::Clusters::OccupancySensing::Id) {
        if (attribute_id == chip::app::Clusters::OccupancySensing::Attributes::Occupancy::Id) {
            if (m_matter_update_by_client_clus_occupancy_attr_occupancy) {
                m_matter_update_by_client_clus_occupancy_attr_occupancy = false;
            }
        }
    }
}

void COccupancySensor::matter_update_all_attribute_values()
{
    matter_update_clus_occupancy_attr_occupancy();
}

void COccupancySensor::update_occupancy(uint8_t value)
{
    m_occupancy = value;
    if (m_occupancy != m_occupancy_prev) {
        GetLogger(eLogType::Info)->Log("Update occupancy as %u", value);
        matter_update_clus_occupancy_attr_occupancy();
    }
    m_occupancy_prev = m_occupancy;
}

void COccupancySensor::matter_update_clus_occupancy_attr_occupancy(bool force_update/*=false*/)
{
    esp_matter_attr_val_t target_value = esp_matter_bitmap8(m_occupancy);
    matter_update_cluster_attribute_common(
        m_endpoint_id,
        chip::app::Clusters::OccupancySensing::Id,
        chip::app::Clusters::OccupancySensing::Attributes::Occupancy::Id,
        target_value,
        &m_matter_update_by_client_clus_occupancy_attr_occupancy,
        force_update
    );
}