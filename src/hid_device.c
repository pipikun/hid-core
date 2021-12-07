#include "hid_device.h"
#include "hid_core.h"
#include "hid_common.h"
#include "hid_node.h"


static uint8_t hid_device_search(uint8_t *name, uint8_t type)
{
	struct hid_device *device = NULL;

	device = hid_device_find((const char*)name,  type);
	return (device == HID_NULL);
}

uint8_t hid_find_device(uint8_t *data)
{
	uint8_t len, type;
	uint8_t name[HID_NAME_MAX + 1];

	len  = data[0];

	if (len > HID_NAME_MAX) {
		return HID_MORE_THEN_LEN;
	}

	type = data[len+1];
	hid_strncpy((char *)name, (const char *)&data[1], len);

	if (hid_device_search(name,(enum hid_object_class_type)type)) {
		return HID_UNKNOW_NAME;
	}
	return HID_OK;
}
