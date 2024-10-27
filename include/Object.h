#ifndef __OBJECT_CLASS_H__
#define __OBJECT_CLASS_H__

#include <stddef.h> /* size_t */
#include "metadata_class.h"


typedef struct Object {
    class_metadata *metadata;
} Object;



/* function prototypes */
Object *Object_constructor(void);
int Object_equals(Object* self, Object* other);
int Object_hashCode(Object* self);
class_metadata* Object_getClass(Object* self);


/* Object vtable typedef */
/*
typedef struct Object_vtable_t
{
	Object *(*constructor)(void);
	int (*equals)(Object *obj1, Object *obj2);
	int (*hashCode)(Object *obj);
	class_metadata *(*getClass)(Object *obj);
} Object_vtable_t;
*/


#endif /* __OBJECT_CLASS_H__ */