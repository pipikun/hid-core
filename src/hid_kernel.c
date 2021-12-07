#include "hid_kernel.h"
#include "hid_common.h"
#include "hid_node.h"

struct hid_kernel_device kernel_dev;

static uint8_t dump_device_config(uint8_t *data, uint8_t type)
{
	hid_device_t device;

	device = (hid_device_t)hid_object_find((char *)&data[1], type);

	if (device == HID_NULL) return HID_UNKNOW_NAME;

	data[0] = device->flag;
	data[1] = device->open_flag;
	data[2] = device->parent.type;
	data[3] = device->parent.flag;

	return HID_OK;
}

uint8_t hid_kernel_dump(uint8_t *data)
{
	uint8_t mask, type, number;
	uint8_t hid_state = HID_OK;

	mask   = data[0];
	type   = (enum hid_object_class_type)data[1];

	switch (mask) {
	case 0:	/* get device number */
		number = get_object_number(type);
		data[0] = 1;
		data[1] = number;
		break;
	case 1: /* get device name */
		data[0] = HID_NAME_MAX;
		number = data[2];
		hid_state = get_object_name(type, (char *)&data[1], number);
		break;
	case 2: /* dump device config*/
		hid_state = dump_device_config(data, mask);
		break;
	default:
		hid_state = HID_UNKNOW_DEVICE_OPT_MASK;
		break;
	}

	return hid_state;
}

uint8_t hid_kernel_entry(uint8_t mask, uint8_t *data)
{
	uint8_t state = HID_OK;

	switch (mask) {
	case HID_CONFIG:
		break;
	case HID_READ:
		break;
	case HID_WRITE:
		break;
	case HID_DUMP:
		state = hid_kernel_dump(data);
		break;
	default:
		break;
	}

	return state;
}

int kernel_device_register(void)
{
	kernel_dev.parent.msg_entry = hid_kernel_entry;
	hid_device_register(&kernel_dev.parent,
			    HID_Object_Class_Device,
			    "hid_kernel");
	return 1;
}

/* register kernel device */
INIT_DEVICE_REGISTER_EXPORT(kernel_device_register);

