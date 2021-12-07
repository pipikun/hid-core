/*-----------------------------------------------------------------------
 *   File name:	main.c
 *      Author:	carson
 * Create time:	2021-02-27
 *    Describe:	none
 *----------------------------------------------------------------------*/
#include "main.h"
#include "stdio.h"
#include "hid_device.h"
#include "hid_common.h"
#include "hid_node.h"
#include "hid_core.h"

void dump_msg(uint8_t *data)
{
	for (int i=0 ;i<8; i++) {
		for (int j=0; j<8; j++) {
			printf("%02x ", data[i*8+j]);
		}
		printf("\t");
		for (int j=0; j<8; j++) {
			uint8_t val;
			val = data[i*8+j];
			if (val <32 || val > 127) {
				printf("- ");
			} else{
				printf("%c ", val);
			}
		}
		printf("\n");
	}
}

void test_kernel(void)
{
	union hid_payload packet;
	union hid_message msg;

	msg.head  = 0;
	msg.valid = 0;
	msg.back  = 1;
	msg.mask  = HID_DUMP;

	packet.head = msg.val;

	for (int i=0; i<62; i++) {
		packet.data[i] = 0;
	}
	packet.data[1] = 1;

	printf("- msg.val: %d\n", msg.val);
	printf("- msg.head: %d\n", msg.head);


	fast_hid_entry(packet.payload, 64);
	dump_msg(packet.payload);
}

void check_device(const char *name)
{
	hid_device_t device;

	device = hid_device_find(name, HID_Object_Class_Device);
	printf("- device: %s \t: %s \n", name, device->parent.name);
}

void get_device_check(uint8_t num)
{
	hid_device_t device;

	device = hid_device_get(num, HID_Object_Class_Device);
	printf("- device index: %d \t: %s \n", num, device->parent.name);
}

int main (void)
{
	hid_startup();

	printf("== register check ==\n");
	check_device("hid_kernel");
	check_device("boot_loader");

	printf("== kernel test ==\n");
	test_kernel();

	printf("== device get ==\n");
	get_device_check(0);
	get_device_check(1);

	return 0;
}

