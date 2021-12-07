#include "hid_core.h"
#include "hid_config.h"
#include "hid_device.h"
#include "hid_node.h"

/*****************************************************************
 * @name:	fast_hid_entry
 * @brief:	This is the command parsing fast hid entrance.
 * @param:
 *    	#data	Messige header address.
 *	#len	Messige length.
 * @return:	The kernel state code.
*****************************************************************/
uint8_t fast_hid_entry(uint8_t *payload, uint8_t len)
{
	union hid_message *msg;
	uint8_t *data;
	hid_device_t device;

	msg  = (union hid_message*)payload;
	data = &payload[sizeof(union hid_message)];
	len -= sizeof(union hid_message);

	if (len > HID_DATA_MAX_LEN) {
		msg->mask = HID_MORE_THEN_LEN;
		return msg->back;
	}

	device = hid_device_get(msg->head, HID_Object_Class_Device);

	if (device == HID_NULL) {
		msg->mask = HID_UNKNOW_HEAD;
	} else {
		msg->mask = device->msg_entry(msg->mask, data);
	}

	return msg->back;
}
