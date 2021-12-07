#ifndef __HID_CORE_H__
#define __HID_CORE_H__

#include "stdint.h"
#include "hid_common.h"

enum hid_state {
	HID_ERROR = 0,
	HID_OK,
	HID_UNKNOW_HEAD,
	HID_UNKNOW_MASK,
	HID_UNKNOW_NAME,
	HID_UNKNOW_DEVICE_OPT_MASK,
	HID_MORE_THEN_LEN,
};

enum hid_event {
	HID_EMPTY  = 0,
	HID_CONFIG = 1,
	HID_READ   = 2,
	HID_WRITE  = 3,
	HID_DUMP   = 4,
};

union hid_message {
	struct {
		uint8_t valid:1;
		uint8_t back:1;
		uint8_t mask:6;
		uint8_t head:8;
	};

	uint16_t val;
};

union hid_payload {
	struct {
		uint16_t head;
		uint8_t  data[62];
	};
	uint8_t payload[64];
};

typedef union hid_message *msg_t;
typedef union hid_payload *pld_t;


uint8_t fast_hid_entry(uint8_t *payload, uint8_t len);

#endif /*__HID_CORE_H__ */
