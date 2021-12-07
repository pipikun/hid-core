#ifndef __HID_DEVICE_H__
#define __HID_DEVICE_H__

#include "hid_config.h"
#include "hid_common.h"

enum hid_config_type
{
	HID_PARAM_CFG = 0,
	HID_FIND_CFG,
};

uint8_t hid_find_device(uint8_t *data);

#endif /*__HID_DEVICE_H__ */
