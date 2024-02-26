#include "util.h"
#include "system.h"
#include <esp_matter_core.h>

const char* get_matter_device_name(uint16_t device_id)
{
    switch(device_id) {
    /* Utility Device Types */
    case 0x0016: return "Root Node";
    case 0x0011: return "Power Source";
    case 0x0012: return "OTA Requestor";
    case 0x0014: return "OTA Provider";
    case 0x000E: return "Aggregator";
    case 0x0013: return "Bridged Node";
    /* Application Device Types */
    /* lighting */
    case 0x0100: return "On/Off Light";
    case 0x0101: return "Dimmable Light";
    case 0x010C: return "Color Temperature Light";
    case 0x010D: return "Extended Color Light";
    /* smart plugs/outlets and other actuators */
    case 0x010A: return "On/Off Plug-in Unit";
    case 0x010B: return "Dimmable Plug-In Unit";
    case 0x0303: return "Pump";
    /* switched and controls */
    case 0x0103: return "On/Off Light Switch";
    case 0x0104: return "Dimmer Switch";
    case 0x0105: return "Color Dimmer Switch";
    case 0x0840: return "Control Bridge";
    case 0x0304: return "Pump Controller";
    case 0x000F: return "Generic Switch";
    /* sensors */
    case 0x0015: return "Contact Sensor";
    case 0x0106: return "Light Sensor";
    case 0x0107: return "Occupancy Sensor";
    case 0x0302: return "Temperature Sensor";
    case 0x0305: return "Pressure Sensor";
    case 0x0306: return "Flow Sensor";
    case 0x0307: return "Humidity Sensor";
    case 0x0850: return "On/Off Sensor";
    /* clusures */
    case 0x000A: return "Door Lock";
    case 0x000B: return "Door Lock Controller";
    case 0x0202: return "Window Covering";
    case 0x0203: return "Window Covering Controller";
    /* HVAC */
    case 0x0300: return "Heating/Cooling Unit";
    case 0x0301: return "Thermostat";
    case 0x002B: return "Fan";
    /* media */
    case 0x0028: return "Basic Video Player";
    case 0x0023: return "Casting Video Player";
    case 0x0022: return "Speaker";
    case 0x0024: return "Content App";
    case 0x0029: return "Casting Video Client";
    case 0x002A: return "Video Remote Control";
    /* generic */
    case 0x0027: return "Mode Select";
    default: return "?";
    }
}

const char* get_matter_cluster_name(uint32_t cluster_id)
{
    switch(cluster_id) {
    case 0x0003: return "Identify";
    case 0x0004: return "Groups";
    case 0x0005: return "Scenes";
    case 0x0006: return "On/Off";
    case 0x0008: return "Level Control";
    case 0x0009: return "Alarms";
    case 0x000A: return "Time";
    case 0x001D: return "Descriptor";
    case 0x001E: return "Binding";
    case 0x001F: return "Access Control";
    case 0x0020: return "Poll Control";
    case 0x0028: return "Basic Information";
    case 0x0029: return "OTA Software Update Provider";
    case 0x002A: return "OTA Software Update Requestor";
    case 0x002B: return "Localization Configuration";
    case 0x002C: return "Time Format Localization";
    case 0x002D: return "Unit Localization";
    case 0x002E: return "Power Source Configuration";
    case 0x002F: return "Power Source";
    case 0x0030: return "General Commisioning";
    case 0x0031: return "Network Commisioning";
    case 0x0032: return "diagnostic Logs";
    case 0x0033: return "General Diagnostics";
    case 0x0034: return "Software Diagnostics";
    case 0x0035: return "Thread Network Diagnostics";
    case 0x0036: return "Wi-Fi Network Diagnostics";
    case 0x0037: return "Ethernet Network Diagnostics";
    case 0x0038: return "Time Synchronization";
    case 0x003C: return "Administrator Commisioning";
    case 0x003B: return "Switch";
    case 0x003E: return "Node Operational Credentials";
    case 0x003F: return "Group Key Management";
    case 0x0039: return "Bridged Device Basic Information";
    case 0x0040: return "Fixed Label";
    case 0x0041: return "User Label";
    case 0x0045: return "Boolean State";
    case 0x0050: return "Mode Select";
    case 0x0101: return "Door Lock";
    case 0x0102: return "Window Covering";
    case 0x0200: return "Pump Configuration and Control";
    case 0x0201: return "Thermostat";
    case 0x0202: return "Fan Control";
    case 0x0204: return "Thermostat User Interface Configuration";
    case 0x0300: return "Color Control";
    case 0x0400: return "Illuminance Measurement";
    case 0x0402: return "Temperature Measurement";
    case 0x0403: return "Pressure Measurement";
    case 0x0404: return "Flow Measurement";
    case 0x0405: return "Relative Humidity Measurement";
    case 0x0406: return "Occupancy Sensing";
    case 0x0503: return "Wake On LAN";
    case 0x0504: return "Channel";
    case 0x0505: return "Target Navigator";
    case 0x0506: return "Media Playback";
    case 0x0507: return "Media Input";
    case 0x0508: return "Low Power";
    case 0x0509: return "Keypad Input";
    case 0x050A: return "Content Launcher";
    case 0x050B: return "Audio Output";
    case 0x050C: return "Application Launcher";
    case 0x050D: return "Application Basic";
    case 0x050E: return "Account Login";
    default: return "?";
    }
}

const char* get_matter_attribute_name(uint32_t cluster_id, uint32_t attribute_id)
{
    switch(attribute_id) {
    case 0x00FE: return "FabricIndex";
    case 0xFFF8: return "GeneratedCommandList";
    case 0xFFF9: return "AcceptedCommandList";
    case 0xFFFA: return "EventList";
    case 0xFFFB: return "AttributeList";
    case 0xFFFC: return "FeatureMap";
    case 0xFFFD: return "ClusterRevision";
    default:
        switch(cluster_id) {
        /* Identify */
        case 0x0003:
            switch(attribute_id) {
            case 0x0000: return "IdentifyTime";
            case 0x0001: return "IdentifyType";
            }
            break;
        /* Groups */
        case 0x0004:
            switch(attribute_id) {
            case 0x0000: return "NameSupport";
            }
            break;
        /* Scenes */
        case 0x0005:
            switch(attribute_id) {
            case 0x0000: return "SceneCount";
            case 0x0001: return "CurrentScene";
            case 0x0002: return "CurrentGroup";
            case 0x0003: return "SceneValid";
            case 0x0004: return "NameSupport";
            case 0x0005: return "LastConfiguredBy";            
            }
            break;
        /* On/Off */
        case 0x0006:
            switch(attribute_id) {
            case 0x0000: return "OnOff";
            case 0x4000: return "GlobalSceneControl";
            case 0x4001: return "OnTime";
            case 0x4002: return "OffWaitTime";
            case 0x4003: return "StartUpOnOff";
            }
            break;
        /* Level Control */
        case 0x0008:
            switch(attribute_id) {
            case 0x0000: return "CurrentLevel";
            case 0x0001: return "RemainingTime";
            case 0x0002: return "MinLevel";
            case 0x0003: return "MaxLevel";
            case 0x0004: return "CurrentFrequency";
            case 0x0005: return "MinFrequency";
            case 0x0006: return "MaxFrequency";
            case 0x000F: return "Options";
            case 0x0010: return "OnOffTransitionTime";
            case 0x0011: return "OnLevel";
            case 0x0012: return "OnTransitionTime";
            case 0x0013: return "OffTransitionTime";
            case 0x0014: return "DefaultMoveRate";
            case 0x4000: return "StartUpCurrentLevel";
            }
            break;
        /* Descriptor */
        case 0x001D:
            switch(attribute_id) {
            case 0x0000: return "DeviceTypeList";
            case 0x0001: return "ServerList";
            case 0x0002: return "ClientList";
            case 0x0003: return "PartsList";
            }
            break;
        /* Access Control */
        case 0x001F:
            switch(attribute_id) {
            case 0x0000: return "ACL";
            case 0x0001: return "Extension";
            case 0x0002: return "SubjectsPerAccessControlEntry";
            case 0x0003: return "TargetsPerAccessControlEntry";
            case 0x0004: return "AccessControlEntriesPerFabric";
            }
            break;
        /* Basic Information */
        /* Bridged Device Basic Information */
        case 0x0028:
        case 0x0039:
            switch(attribute_id) {
            case 0x0000: return "DataModelRevision";
            case 0x0001: return "VendorName";
            case 0x0002: return "VendorID";
            case 0x0003: return "ProductName";
            case 0x0004: return "ProductID";
            case 0x0005: return "NodeLabel";
            case 0x0006: return "Location";
            case 0x0007: return "HardwareVersion";
            case 0x0008: return "HardwareVersionString";
            case 0x0009: return "SoftwareVersion";
            case 0x000A: return "SoftwareVersionString";
            case 0x000B: return "ManufacturingDate";
            case 0x000C: return "PartNumber";
            case 0x000D: return "ProductURL";
            case 0x000E: return "ProductLabel";
            case 0x000F: return "SerialNumber";
            case 0x0010: return "LocalConfigDisabled";
            case 0x0011: return "Reachable";
            case 0x0012: return "UniqueID";
            case 0x0013: return "CapabilityMinima";
            }
            break;
        /* General Commisioning */
        case 0x0030:
            switch(attribute_id) {
            case 0x0000: return "Breadcrumb";
            case 0x0001: return "BasicCommisioningInfo";
            case 0x0002: return "RegulatoryConfig";
            case 0x0003: return "LocationCapability";
            case 0x0004: return "SupportsConcurrentConnection";
            }
            break;
        /* Color Control */
        case 0x0300:
            switch(attribute_id) {
            case 0x0000: return "CurrentHue";
            case 0x0001: return "CurrentSaturation";
            case 0x0002: return "RemainingTime";
            case 0x0003: return "CurrentX";
            case 0x0004: return "CurrentY";
            case 0x0005: return "DriftCompensation";
            case 0x0006: return "CompensationText";
            case 0x0007: return "ColorTemperatureMireds";
            case 0x0008: return "ColorMode";
            case 0x000F: return "Options";
            case 0x0010: return "NumberOfPrimaries";
            case 0x0011: return "Primary1X";
            case 0x0012: return "Primary1Y";
            case 0x0013: return "Primary1Intensity";
            case 0x0015: return "Primary2X";
            case 0x0016: return "Primary2Y";
            case 0x0017: return "Primary2Intensity";
            case 0x0019: return "Primary3X";
            case 0x001A: return "Primary3Y";
            case 0x001B: return "Primary3Intensity";
            case 0x0020: return "Primary4X";
            case 0x0021: return "Primary4Y";
            case 0x0022: return "Primary4Intensity";
            case 0x0024: return "Primary5X";
            case 0x0025: return "Primary5Y";
            case 0x0026: return "Primary5Intensity";
            case 0x0028: return "Primary6X";
            case 0x0029: return "Primary6Y";
            case 0x002A: return "Primary6Intensity";
            case 0x0030: return "WhitePointX";
            case 0x0031: return "WhitePointY";
            case 0x0032: return "ColorPointRX";
            case 0x0033: return "ColorPointRY";
            case 0x0034: return "ColorPointRIntensity";
            case 0x0036: return "ColorPointGX";
            case 0x0037: return "ColorPointGY";
            case 0x0038: return "ColorPointGIntensity";
            case 0x003A: return "ColorPointBX";
            case 0x003B: return "ColorPointBY";
            case 0x003C: return "ColorPointBIntensity";
            case 0x4000: return "EnhancedCurrentHue";
            case 0x4001: return "EnhancedColorMode";
            case 0x4002: return "ColorLoopActive";
            case 0x4003: return "ColorLoopDirection";
            case 0x4004: return "ColorLoopTime";
            case 0x4005: return "ColorLoopStartEnhancedHue";
            case 0x4006: return "ColorLoopStoredEnhancedHue";
            case 0x400A: return "ColorCapabilities";
            case 0x400B: return "ColorTempPhysicalMinMireds";
            case 0x400C: return "ColorTempPhysicalMaxMireds";
            case 0x400D: return "CoupleColorTempToLevelMinMireds";
            case 0x4010: return "StartUpColorTemperatureMireds";
            }
            break;
        /* Ballast Configuration */
        case 0x0301:
            switch(attribute_id) {
            case 0x0000: return "PhysicalMinLevel";
            case 0x0001: return "PhysicalMaxLevel";
            case 0x0002: return "BallastStatus";
            case 0x0010: return "MinLevel";
            case 0x0011: return "MaxLevel";
            case 0x0012: return "PowerOnLevel";
            case 0x0013: return "PowerOnFadeTime";
            case 0x0014: return "IntrinsicBallastFactor";
            case 0x0015: return "BallastFactorAdjustment";
            case 0x0020: return "LampQuantity";
            case 0x0030: return "LampType";
            case 0x0031: return "LampManufacturer";
            case 0x0032: return "LampRatedHours";
            case 0x0033: return "LampBurnHours";
            case 0x0034: return "LampAlarmMode";
            case 0x0035: return "LampBurnHoursTripPoint";
            }
            break;
        /* Illuminance Measurement */
        case 0x0400:
            switch(attribute_id) {
            case 0x0000: return "MeasuredValue";
            case 0x0001: return "MinMeasuredValue";
            case 0x0002: return "MaxMeasuredValue";
            case 0x0003: return "Tolerance";
            case 0x0004: return "LightSensorType";
            }
            break;
        /* Temperature Measurement */
        /* Flow Measurement */
        /* Relative Humidity Measurement */
        /* Leaf Wetness Measurement */
        /* Soil Moisture Measurement */
        case 0x0402:
        case 0x0404:
        case 0x0405:
        case 0x0407:
        case 0x0408:
            switch(attribute_id) {
            case 0x0000: return "MeasuredValue";
            case 0x0001: return "MinMeasuredValue";
            case 0x0002: return "MaxMeasuredValue";
            case 0x0003: return "Tolerance";
            }
            break;
        /* Pressure Measurement */
        case 0x0403:
            switch(attribute_id) {
            case 0x0000: return "MeasuredValue";
            case 0x0001: return "MinMeasuredValue";
            case 0x0002: return "MaxMeasuredValue";
            case 0x0003: return "Tolerance";
            case 0x0010: return "ScaledValue";
            case 0x0011: return "MinScaledValue";
            case 0x0012: return "MaxScaledValue";
            case 0x0013: return "ScaledTolerance";
            case 0x0014: return "Scale";
            }
            break;
        /* Occupancy Sensing */
        case 0x0406:
            switch(attribute_id) {
            case 0x0000: return "Occupancy";
            case 0x0001: return "OccupancySensorType";
            case 0x0002: return "OccupancySensorTypeBitmap";
            case 0x0010: return "PIROccupiedToUnoccupiedDelay";
            case 0x0011: return "PIRUnoccupiedToOccupiedDelay";
            case 0x0012: return "PIRUnoccupiedToOccupiedThreshold";
            case 0x0020: return "UltrasonicOccupiedToUnoccupiedDelay";
            case 0x0021: return "UltrasonicUnoccupiedToOccupiedDelay";
            case 0x0022: return "UltrasonicUnoccupiedToOccupiedThreshold";
            case 0x0030: return "PhysicalContactOccupiedToUnoccupiedDelay";
            case 0x0031: return "PhysicalContactUnoccupiedToOccupiedDelay";
            case 0x0032: return "PhysicalContactUnoccupiedToOccupiedThreshold";
            }
            break;
        }
    }

    return "?";
}

const char* get_matter_command_name(uint32_t cluster_id, uint32_t command_id)
{
    /* Identify */
    if (cluster_id == 0x0003) {
        if (command_id == 0x00) {
            return "Itentify";
        } else if (command_id == 0x01) {
            return "IdentifyQuery";
        } else if (command_id == 0x40) {
            return "TriggerEffect";
        }
        /* Direction... 
        else if (command_id == 0x00) {
            return "IdentifyQueryResponse";
        } 
        */
    }
    /* On/Off */
    else if (cluster_id == 0x0006) {
        if (command_id == 0x00) {
            return "Off";
        } else if (command_id == 0x01) {
            return "On";
        } else if (command_id == 0x02) {
            return "Toggle";
        } else if (command_id == 0x40) {
            return "OffWithEffect";
        } else if (command_id == 0x41) {
            return "OnWithRecallGlobalScene";
        } else if (command_id == 0x42) {
            return "OnWithTimedOff";
        }
    }

    /* Level Control */
    else if (cluster_id == 0x0008) {
        if (command_id == 0x00) {
            return "MoveToLevel";
        } else if (command_id == 0x01) {
            return "Move";
        } else if (command_id == 0x02) {
            return "Step";
        } else if (command_id == 0x03) {
            return "Stop";
        } else if (command_id == 0x04) {
            return "MoveToLevelWithOnOff";
        } else if (command_id == 0x05) {
            return "MoveWithOnOff";
        } else if (command_id == 0x06) {
            return "StepWithOnOff";
        } else if (command_id == 0x07) {
            return "StopWithOnOff";
        } else if (command_id == 0x08) {
            return "MoveToClosestFrequency";
        } 
    }
    /* Color Control */
    else if (cluster_id == 0x0300) {
        if (command_id == 0x00) {
            return "MoveToHue";
        } else if (command_id == 0x01) {
            return "MoveHue";
        } else if (command_id == 0x02) {
            return "StepHue";
        } else if (command_id == 0x03) {
            return "MoveToSaturation";
        } else if (command_id == 0x04) {
            return "MoveSaturation";
        } else if (command_id == 0x05) {
            return "StepSaturation";
        } else if (command_id == 0x06) {
            return "MoveToHueAndSaturation";
        } else if (command_id == 0x07) {
            return "MoveToColor";
        } else if (command_id == 0x08) {
            return "MoveColor";
        } else if (command_id == 0x09) {
            return "StepColor";
        } else if (command_id == 0x0A) {
            return "MoveToColorTemperature";
        } else if (command_id == 0x40) {
            return "EnhancedMoveToHue";
        } else if (command_id == 0x41) {
            return "EnhancedMoveHue";
        } else if (command_id == 0x42) {
            return "EnhancedStepHue";
        } else if (command_id == 0x43) {
            return "EnhancedMoveToHueAndSaturation";
        } else if (command_id == 0x44) {
            return "ColorLoopSet";
        } else if (command_id == 0x47) {
            return "StopMoveStep";
        } else if (command_id == 0x4B) {
            return "MoveColorTemperature";
        } else if (command_id == 0x4C) {
            return "StepColorTemperature";
        }
    }
    return "?";
}

cJSON* get_matter_value_item(esp_matter_attr_val_t value)
{
    cJSON *item = nullptr;
    switch (value.type) {
    case ESP_MATTER_VAL_TYPE_INVALID:
        break;
    case ESP_MATTER_VAL_TYPE_BOOLEAN:
        item = cJSON_CreateBool(value.val.b);
        break;
    case ESP_MATTER_VAL_TYPE_INTEGER:
    case ESP_MATTER_VAL_TYPE_NULLABLE_INTEGER:
        item = cJSON_CreateNumber(value.val.i);
        break;
    case ESP_MATTER_VAL_TYPE_FLOAT:
    case ESP_MATTER_VAL_TYPE_NULLABLE_FLOAT:
        item = cJSON_CreateNumber(value.val.f);
        break;
    case ESP_MATTER_VAL_TYPE_ARRAY:
        {
            item = cJSON_CreateArray();
            /*
            // TODO:
            uint16_t size = value.val.a.s;
            uint16_t count = value.val.a.n;
            for (uint16_t i = 0; i < count; i++) {

            }
            */
        }
        break;
    case ESP_MATTER_VAL_TYPE_CHAR_STRING:
    case ESP_MATTER_VAL_TYPE_OCTET_STRING:
        {
            char temp[64];
            snprintf(temp, sizeof(temp), "%.*s", value.val.a.s, (char *)value.val.a.b);
            item = cJSON_CreateString(temp);
        }
        break;
    case ESP_MATTER_VAL_TYPE_INT8:
    case ESP_MATTER_VAL_TYPE_NULLABLE_INT8:
        item = cJSON_CreateNumber(value.val.i8);
        break;
    case ESP_MATTER_VAL_TYPE_UINT8:
    case ESP_MATTER_VAL_TYPE_NULLABLE_UINT8:
        item = cJSON_CreateNumber(value.val.u8);
        break;
    case ESP_MATTER_VAL_TYPE_INT16:
    case ESP_MATTER_VAL_TYPE_NULLABLE_INT16:
        item = cJSON_CreateNumber(value.val.i16);
        break;
    case ESP_MATTER_VAL_TYPE_UINT16:
    case ESP_MATTER_VAL_TYPE_NULLABLE_UINT16:
        item = cJSON_CreateNumber(value.val.u16);
        break;
    case ESP_MATTER_VAL_TYPE_INT32:
    case ESP_MATTER_VAL_TYPE_NULLABLE_INT32:
        item = cJSON_CreateNumber(value.val.i32);
        break;
    case ESP_MATTER_VAL_TYPE_UINT32:
    case ESP_MATTER_VAL_TYPE_NULLABLE_UINT32:
        item = cJSON_CreateNumber(value.val.u32);
        break;
    case ESP_MATTER_VAL_TYPE_INT64:
    case ESP_MATTER_VAL_TYPE_NULLABLE_INT64:
        item = cJSON_CreateNumber(value.val.i64);
        break;
    case ESP_MATTER_VAL_TYPE_UINT64:
    case ESP_MATTER_VAL_TYPE_NULLABLE_UINT64:
        item = cJSON_CreateNumber(value.val.u64);
        break;
    case ESP_MATTER_VAL_TYPE_ENUM8:
    case ESP_MATTER_VAL_TYPE_NULLABLE_ENUM8:
        item = cJSON_CreateNumber(value.val.u8);
        break;
    case ESP_MATTER_VAL_TYPE_BITMAP8:
    case ESP_MATTER_VAL_TYPE_NULLABLE_BITMAP8:
        {
            char temp[16];
            snprintf(temp, sizeof(temp), "0x%02x", value.val.u8);
            item = cJSON_CreateString(temp);
        }
        break;
    case ESP_MATTER_VAL_TYPE_BITMAP16:
    case ESP_MATTER_VAL_TYPE_NULLABLE_BITMAP16:
        {
            char temp[16];
            snprintf(temp, sizeof(temp), "0x%04x", value.val.u16);
            item = cJSON_CreateString(temp);
        }
        break;
    case ESP_MATTER_VAL_TYPE_BITMAP32:
    case ESP_MATTER_VAL_TYPE_NULLABLE_BITMAP32:
        {
            char temp[16];
            snprintf(temp, sizeof(temp), "0x%08x", value.val.u32);
            item = cJSON_CreateString(temp);
        }
        break;
    default:
        break;
    }

    return item;
}

cJSON* dump_matter_endpoint_info(uint16_t endpoint_id)
{
    char temp[64]{};
    uint8_t dev_type_count;
    uint32_t cluster_id, attr_id, cmd_id;
    uint32_t *dev_type_ids;
    esp_matter::node_t* root_node = GetSystem()->get_root_node();

    if (root_node) {
        cJSON *item, *item2;
        cJSON *root = cJSON_CreateObject();
        esp_matter::endpoint_t *endpoint = esp_matter::endpoint::get(root_node, endpoint_id);
        if (endpoint) {
            cJSON_AddNumberToObject(root, "endpoint_id", endpoint_id);

            // device type
            cJSON *array_device_type = cJSON_CreateArray();
            cJSON_AddItemToObject(root, "device_type", array_device_type);
            dev_type_ids = esp_matter::endpoint::get_device_type_ids(endpoint, &dev_type_count);
            for (uint8_t cnt = 0; cnt < dev_type_count; cnt++) {
                item = cJSON_CreateObject();
                cJSON_AddItemToArray(array_device_type, item);
                snprintf(temp, sizeof(temp), "0x%04X", dev_type_ids[cnt]);
                cJSON_AddStringToObject(item, "id", temp);
                cJSON_AddStringToObject(item, "name", get_matter_device_name(dev_type_ids[cnt]));
            }

            // clusters
            cJSON *array_cluster = cJSON_CreateArray();
            cJSON_AddItemToObject(root, "clusters", array_cluster);
            esp_matter::cluster_t *cluster = esp_matter::cluster::get_first(endpoint);
            while (cluster != nullptr) {
                item = cJSON_CreateObject();
                cJSON_AddItemToArray(array_cluster, item);
                cluster_id = esp_matter::cluster::get_id(cluster);
                snprintf(temp, sizeof(temp), "0x%08X", cluster_id);
                cJSON_AddStringToObject(item, "id", temp);
                cJSON_AddStringToObject(item, "name", get_matter_cluster_name(cluster_id));
                
                // attributes
                cJSON *array_attribute = cJSON_CreateArray();
                cJSON_AddItemToObject(item, "attributes", array_attribute);
                esp_matter::attribute_t *attr = esp_matter::attribute::get_first(cluster);
                while (attr != nullptr) {
                    item2 = cJSON_CreateObject();
                    cJSON_AddItemToArray(array_attribute, item2);
                    attr_id = esp_matter::attribute::get_id(attr);
                    snprintf(temp, sizeof(temp), "0x%08X", attr_id);
                    cJSON_AddStringToObject(item2, "id", temp);
                    cJSON_AddStringToObject(item2, "name", get_matter_attribute_name(cluster_id, attr_id));
                    // value
                    esp_matter_attr_val_t val = esp_matter_invalid(NULL);
                    esp_matter::attribute::get_val(attr, &val);
                    cJSON *item3 = get_matter_value_item(val);
                    cJSON_AddItemToObject(item2, "value", item3);
                    attr = esp_matter::attribute::get_next(attr);
                }

                // commands
                cJSON *array_command = cJSON_CreateArray();
                cJSON_AddItemToObject(item, "commands", array_command);
                esp_matter::command_t *cmd = esp_matter::command::get_first(cluster);
                while (cmd != nullptr) {
                    item2 = cJSON_CreateObject();
                    cJSON_AddItemToArray(array_command, item2);
                    cmd_id = esp_matter::command::get_id(cmd);
                    snprintf(temp, sizeof(temp), "0x%02X", cmd_id);
                    cJSON_AddStringToObject(item2, "id", temp);
                    cJSON_AddStringToObject(item2, "name", get_matter_command_name(cluster_id, cmd_id));
                    cmd = esp_matter::command::get_next(cmd);
                }

                cluster = esp_matter::cluster::get_next(cluster);
            }
        }
        return root;
    }

    return nullptr;
}
