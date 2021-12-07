#include "hid_common.h"
#include "hid_node.h"

hid_device_t hid_device_find(const char *name, uint8_t type)
{
	return (hid_device_t)hid_object_find(name, (enum hid_object_class_type) type);
}

hid_device_t hid_device_get(uint8_t index, uint8_t type)
{
	return (hid_device_t)hid_object_get(type, index);
}

uint8_t hid_device_register(struct hid_device *device,
			    enum hid_object_class_type type,
		            const char *name)
{
	uint8_t state;

	/* TODO: check device name whether there is */
	state = hid_object_init(&device->parent, type, name);

	device->flag      = 0;
	device->open_flag = HID_DEV_INIT;

	return state;
}

static int hid_start(void)
{
	return 0;
}
INIT_EXPORT(hid_start, "0");

static int hid_board_start(void)
{
	return 0;
}
INIT_EXPORT(hid_board_start, "0.end");

static int hid_board_end(void)
{
	return 0;
}
INIT_EXPORT(hid_board_end, "1.end");

static int hid_device_start(void)
{
	return 0;
}
INIT_EXPORT(hid_device_start, "1.end");

static int hid_device_end(void)
{
	return 0;
}
INIT_EXPORT(hid_device_end, "2.end");

static int hid_end(void)
{
	return 0;
}
INIT_EXPORT(hid_end, "5.end");

void hid_common_board_init(void)
{
	const init_fn_t *fn_ptr;

	core_log("hid_board_init\r\n");

	for (fn_ptr = &__hid_init_hid_board_start; fn_ptr<&__hid_init_hid_board_end;
			fn_ptr++)
	{
		(*fn_ptr)();
	}
}

void hid_device_register_init(void)
{
	const init_fn_t *fn_ptr;

	for (fn_ptr = &__hid_init_hid_device_start; fn_ptr<&__hid_init_hid_device_end;
			fn_ptr++)
	{
		(*fn_ptr)();
	}
}

void hid_startup(void)
{
	hid_common_board_init();
	hid_device_register_init();
}

void smart_data(uint8_t *data, uint32_t *store, uint8_t *offset)
{
	uint8_t len, offs;
	uint32_t addr;

	offs = *offset;
	len = data[offs];
	addr= 0;

	for (int i=0; i<len; i++) {
		addr <<= 8;
		addr  += data[len - i + offs];
	}
	*store  = addr;
	*offset = offs + len + 1;
}

