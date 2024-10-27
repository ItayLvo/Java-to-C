#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, size_t */

#include "metadata_class.h"
#include "Object.h"


static void Object_init(Object* obj);


/* Object vtable assignment */
/*
Object_vtable_t object_vtable = 
{
	Object_constructor,
	Object_equals,
	Object_hashCode,
	Object_getClass
};
*/

vfunc_t object_vtable[] = {
    (void (*)(void))Object_constructor,
    (void (*)(void))Object_equals,
    (void (*)(void))Object_hashCode,
    (void (*)(void))Object_getClass
};

/* Object metadata initialization */
class_metadata Object_metadata ={"Object", sizeof(Object), &object_vtable, NULL};

static void Object_init(Object* obj)
{
	obj->metadata = &Object_metadata;
}



Object *Object_constructor(void)
{
    Object *obj = (Object *)malloc(sizeof(Object));
	if (obj == NULL)
	{
		return NULL;
	}

	Object_init(obj);

	return obj;
}

int Object_equals(Object *self, Object *other)
{
	return (self == other);
}


int Object_hashCode(Object* self)
{
	return (int)(size_t)self;
}


class_metadata* Object_getClass(Object* self)
{
	return self->metadata;
}



