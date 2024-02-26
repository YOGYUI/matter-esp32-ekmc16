#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>
#include "cJSON.h"
#include <esp_matter_attribute_utils.h>

#ifdef __cplusplus
extern "C" {
#endif

const char* get_matter_device_name(uint16_t device_id);
const char* get_matter_cluster_name(uint32_t cluster_id);
const char* get_matter_attribute_name(uint32_t cluster_id, uint32_t attribute_id);
const char* get_matter_command_name(uint32_t cluster_id, uint32_t command_id);
cJSON* get_matter_value_item(esp_matter_attr_val_t value);
cJSON* dump_matter_endpoint_info(uint16_t endpoint_id);

#ifdef __cplusplus
};
#endif
#endif