#include "hid_node.h"
#include "hid_core.h"

/**
 * @brief initialize a list
 *
 * @param list list to be initialize
 *
 * @return none
 */
static inline void hid_list_init(hid_list_t *list)
{
	list->next = list->prev = list;
}

/**
 * @brief insert a node after a list
 *
 * @param list list to insert it
 * @param node new node to be inserted
 *
 * @return none
 */
static inline void hid_list_insert_after(hid_list_t *list, hid_list_t *node)
{
	list->next->prev = node;
	node->next = list->next;

	list->next = node;
	node->prev = list;
}

/**
 * @brief insert a node before a list
 *
 * @param list list ti insert it
 * @param node new node to be inserted
 *
 * @return none
 */
static inline void hid_list_insert_before(hid_list_t *list, hid_list_t *node)
{
	list->prev->next = node;
	node->prev = list->prev;

	list->prev = node;
	node->next = list;
}

/**
 * @brief remove node from list
 *
 * @param node the node to remove from the list
 *
 * @return none
 */
static inline void hid_list_remove(hid_list_t *node)
{
	node->next->prev = node->prev;
	node->prev->next = node->next;

	node->next = node->prev = node;
}

/**
 * @brief get the list length
 *
 * @param list the list to get
 *
 * @return the list length
 */
static inline uint16_t hid_list_len(hid_list_t *list)
{
	uint16_t len = 0;
	const hid_list_t *p = list;

	while (p->next != list) {
	       p = p->next;
	       len++;
	}
	return len;
}

/**
 * @brief test whether a list is empty
 *
 * @param list the list to test
 *
 * @return whether the list is empty boolean value
 */
static inline uint8_t hid_list_isempty(const hid_list_t *list)
{
	return list->next == list;
}


/**
 * This function will copy string no more than n bytes.
 *
 * @param dst the string to copy
 * @param src the string to be copied
 * @param n the maximum copied length
 *
 * @return the result
 */
char *hid_strncpy(char *dst, const char *src, int n)
{
    if (n != 0)
    {
        char *d = dst;
        const char *s = src;

	while (n--) {
		*d++ = *s++;
	}
	*d = 0;
    }
    return (dst);
}

/**
 * This function will compare two strings with specified maximum length
 *
 * @param cs the string to be compared
 * @param ct the string to be compared
 * @param count the maximum compare length
 *
 * @return the result
 */
uint32_t hid_strncmp(const char *cs, const char *ct, uint16_t count)
{
    register signed char __res = 0;

    while (count)
    {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count --;
    }

    return __res;
}

#define _OBJ_CONTAINER_LIST_INIT(c)	\
		{&(hid_object_container[c].object_list), &(hid_object_container[c].object_list)}
static struct hid_object_info hid_object_container[HID_Object_Info_Unknow] =
{
	{HID_Object_Class_Device, _OBJ_CONTAINER_LIST_INIT(HID_Object_Info_Device), sizeof(struct hid_device)},
	{HID_Object_Class_Memory, _OBJ_CONTAINER_LIST_INIT(HID_Object_Info_Memory), sizeof(struct hid_device)},
};

struct hid_object_info *hid_object_get_info(enum hid_object_class_type type)
{
	int index;

	for (index = 0; index < HID_Object_Info_Unknow; index++)
		if (hid_object_container[index].type == type)
			return &hid_object_container[index];
	return HID_NULL;
}

uint8_t hid_object_init(struct hid_object *object,
				      enum hid_object_class_type type,
				      const char *name)
{
	struct hid_object_info *info;
	struct hid_list_node *node = HID_NULL;

	info = hid_object_get_info(type);
	if (info == HID_NULL) return HID_NULL;

	for (node  = info->object_list.next;
	     node != &(info->object_list);
	     node  = node->next)
	{
		struct hid_object *obj;
		obj = hid_list_entry(node, struct hid_object, list);
		/* TODO: assert there was not duplication of equipment definition */
		if (obj) {
			if (object == obj) return HID_NULL;
		}
	}

	object->type = type | HID_Object_Class_Static;

	hid_strncpy(object->name, name, HID_NAME_MAX);

	hid_list_insert_after(&(info->object_list), &(object->list));
	return HID_OK;
}

struct hid_object *hid_object_find(const char *name, uint8_t type)
{
	struct hid_object *object  = HID_NULL;
	struct hid_list_node *node = HID_NULL;
        struct hid_object_info *info = HID_NULL;

	info = hid_object_get_info((enum hid_object_class_type)type);

	if ((name == HID_NULL) || (info == HID_NULL)) return HID_NULL;

	hid_list_for_each(node, &(info->object_list)) {
		object = hid_list_entry(node, struct hid_object, list);
		if (hid_strncmp(object->name, name, HID_NAME_MAX) == 0)
			return object;
	}
	return HID_NULL;
}

uint16_t get_object_number(enum hid_object_class_type type)
{
	struct hid_object_info *info = HID_NULL;

	info = hid_object_get_info(type);
	if (info == HID_NULL) return HID_NULL;

	return hid_list_len(&(info->object_list));
}

uint16_t get_object_name(enum hid_object_class_type type, char *name, uint16_t idx)
{
	struct hid_object *object  = HID_NULL;
	struct hid_list_node *node = HID_NULL;
        struct hid_object_info *info = HID_NULL;
	uint16_t i = 0;

	info = hid_object_get_info((enum hid_object_class_type)type);

	if (info == HID_NULL) return HID_NULL;

	hid_list_for_each(node, &(info->object_list)) {
		if (i++ == idx) {
			object = hid_list_entry(node, struct hid_object, list);
			hid_strncpy(name, object->name, HID_NAME_MAX);
			return HID_OK;
		}
	}

	return HID_NULL;
}

struct hid_object *hid_object_get(enum hid_object_class_type type, uint16_t idx)
{
	struct hid_object *object    = HID_NULL;
	struct hid_list_node *node   = HID_NULL;
        struct hid_object_info *info = HID_NULL;
	uint16_t i = 0;

	info = hid_object_get_info((enum hid_object_class_type)type);
	if (info == HID_NULL) return HID_NULL;
	hid_list_for_each(node, &(info->object_list)) {
		if (i++ == idx) {
			object = hid_list_entry(node, struct hid_object, list);
			return object;
		}
	}

	return HID_NULL;
}

