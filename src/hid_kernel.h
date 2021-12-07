#ifndef __HID_KERNEL_H__
#define __HID_KERNEL_H__

#include "hid_device.h"
#include "hid_config.h"
#include "hid_core.h"

struct hid_kernel_device {
	struct hid_device parent;
};

#endif /*__HID_KERNEL_H__ */
