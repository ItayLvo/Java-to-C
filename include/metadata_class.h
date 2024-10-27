#ifndef __METADATA_H__
#define __METADATA_H__

#include <stddef.h> /* size_t */


typedef struct class_metadata
{
	const char* class_name;			/* class name */
	size_t object_size;				/* size of object */
	void **vtable;					/* pointer to a virtual table (array of function pointers) */
	struct class_metadata *super_class;	/* pointer to superclass metadata */
} class_metadata;


/* vtable function typedef */
/* pointer to function that receive and return void */
typedef void (*vfunc_t)(void);


#endif /* __METADATA_H__ */
