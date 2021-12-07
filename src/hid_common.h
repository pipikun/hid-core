#ifndef __HID_COMMON_H__
#define __HID_COMMON_H__

#include "stdio.h"
#include "stdlib.h"
#include "hid_device.h"

#ifdef USE_FAST_HID_CORE
#define core_log(x)
#else
#define core_log(x) 	printf(x)
#endif

#define SECTION(x)	__attribute__((section(x)))

typedef int (*init_fn_t)(void);

/* hid init  */
#define INIT_EXPORT(fn, level) 		\
	__attribute__((used))		\
 	const init_fn_t __hid_init_##fn	\
	SECTION(".hidinit_fn."level) = fn

/* Implement user API define */
/* boadr init routines will be called in board_init() function */
#define INIT_BOARD_EXPORT(fn)		INIT_EXPORT(fn,"1")
/* device init */
#define INIT_DEVICE_REGISTER_EXPORT(fn)	INIT_EXPORT(fn,"2")

/**
 * @brief hid_list_for_each - iterate over a list
 * @param pos Pointer to the host structure, in the for loop is an iterative variable
 * @param head listNode head
 **/
#define hid_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/*@brief Through structure variables in a member's first address then the whole
          structure of the variable is obtained first address */
#define hid_container_of(ptr, type, member)	\
	((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))

/**
 * @brief getthe struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define hid_list_entry(node, type, member) \
	hid_container_of(node, type, member)

struct hid_list_node {
	struct hid_list_node *next;		/* point to next node */
	struct hid_list_node *prev;		/* point to prev node */
};

typedef struct hid_list_node hid_list_t;	/* type for hid list */

struct hid_object {
	char name[HID_NAME_MAX];
	uint8_t type;
	uint8_t flag;

	hid_list_t list;			/* list object of kernel object */
};

typedef struct hid_object *hid_object_t;

enum hid_object_class_type {
	HID_Object_Class_Null 	= 0x00,
	HID_Object_Class_Device = 0x01,
       	HID_Object_Class_Memory = 0x02,
	HID_Object_Class_Unknow = 0x03,
	HID_Object_Class_Static = 0x80,
};

enum hid_object_info_type
{
	HID_Object_Info_Device = 0,
	HID_Object_Info_Memory,
	HID_Object_Info_Unknow,
};

enum hid_device_state
{
	HID_DEV_OPEN = 0,
	HID_DEV_INIT,
	HID_DEV_CLOSE,
	HID_DEV_SUSPEND,
	HID_DEV_FAIL,
	HID_DEV_BUSY,
};

struct hid_object_info
{
	enum hid_object_class_type type;
	struct hid_list_node object_list;
	unsigned long object_size;
};

struct hid_device {
	struct hid_object parent;
	uint16_t flag;
	uint16_t open_flag;

	uint8_t (*msg_entry)(uint8_t mask, uint8_t *data);

	void *user_data;
};

typedef struct hid_device *hid_device_t;

hid_device_t hid_device_find(const char *name, uint8_t type);
hid_device_t hid_device_get(uint8_t index, uint8_t type);
uint8_t hid_device_register(struct hid_device *device,
			    enum hid_object_class_type type,
		            const char *name);
void hid_common_board_init(void);
void hid_device_register_init(void);
void smart_data(uint8_t *data, uint32_t *store, uint8_t *offset);
void hid_startup(void);

#endif /*__HID_COMMON_H__ */
