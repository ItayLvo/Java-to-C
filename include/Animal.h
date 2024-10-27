#ifndef __ANIMAL_CLASS_H__
#define __ANIMAL_CLASS_H__

#include <stddef.h> /* size_t */
#include "metadata_class.h"
#include "Object.h"

typedef struct Animal {
    class_metadata *metadata;
    Object obj;
	int ID;
} Animal;



/* function prototypes */
Animal *Animal_constructor(void);
int Object_equals(Object* self, Object* other);
int Object_hashCode(Object* self);
class_metadata* Object_getClass(Object* self);





#endif /* __ANIMAL_CLASS_H__ */