#include <stdlib.h> /* malloc */
#include <stdio.h> /* printf, size_t */

#include "metadata_class.h"

/* "constructor" for metadata class */
class_metadata *CreateClassMetadata(
	const char *class_name,
	size_t object_size,
	void *vtable,
	class_metadata *super_class)
{
	/* allocating memory in the heap for the new object */
	class_metadata* metadata = (class_metadata*)malloc(sizeof(class_metadata));
	if (metadata == NULL) {
		perror("malloc failed\n");
		return NULL;
	}

	metadata->class_name = class_name;
	metadata->object_size = object_size;
	metadata->vtable = vtable;
	metadata->super_class = super_class;

	return metadata;
}



