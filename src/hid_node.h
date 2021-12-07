#ifndef __HID_NODE_H__
#define __HID_NODE_H__

#include "stdio.h"
#include "stdint.h"
#include "hid_config.h"
#include "hid_common.h"

uint32_t hid_strncmp(const char *cs, const char *ct, uint16_t count);
char *hid_strncpy(char *dst, const char *src, int n);
struct hid_object_info *hid_object_get_info(enum hid_object_class_type type);
uint8_t hid_object_init(struct hid_object *object, enum hid_object_class_type type, const char *name);
struct hid_object *hid_object_find(const char *name, uint8_t type);
struct hid_object *hid_object_get(enum hid_object_class_type type, uint16_t idx);
uint16_t get_object_number(enum hid_object_class_type type);
uint16_t get_object_name(enum hid_object_class_type type, char *name, uint16_t idx);

#endif /*__HID_NODE_H__ */
