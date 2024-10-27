#include <stddef.h> /* size_t */
#include <stdio.h>  /* sprintf */
#include <stdlib.h> /* malloc, free */

#include "metadata_class.h"

#define INIT 1
#define NOT_INIT 0

enum virtual_functions_enum
{
    TO_STRING_FUNC = 0,
    FINALIZE_FUNC = 1,
    SAY_HELLO_FUNC = 2,
    GET_NUM_MASTERS_FUNC = 3
};

/****** Object class ******/

typedef struct Object_t
{
    class_metadata *metadata;
} Object_t;

static int is_object_init = NOT_INIT;

typedef void (*finalize_func_t)(Object_t *);
typedef char *(*tostring_func_t)(Object_t *, char *);

/* function prototypes */
char *Object_toString(Object_t *self, char *buffer);
void Object_finalize(Object_t *self);

static class_metadata Object_metadata;

void Object_static_init()
{
    if (is_object_init == NOT_INIT)
    {
        static vfunc_t object_vtable[] = {
            (void (*)(void))Object_toString,
            (void (*)(void))Object_finalize,
        };

        Object_metadata.class_name = "Object_t";
        Object_metadata.object_size = sizeof(Object_t);
        Object_metadata.super_class = NULL;
        Object_metadata.vtable = (void **)&object_vtable;

        is_object_init = INIT;
    }
}

void Object_instance_init_object(Object_t *obj)
{
    obj->metadata = &Object_metadata;
}

Object_t *Object_new(void)
{
    Object_static_init();

    Object_t *obj = (Object_t *)malloc(sizeof(Object_t));
    if (obj == NULL)
    {
        return NULL;
    }

    Object_instance_init_object(obj);

    return obj;
}

char *Object_toString(Object_t *self, char *buffer)
{
    sprintf(buffer, "%s@%lu\n", self->metadata->class_name, (size_t)self);

    return buffer;
}

void Object_finalize(Object_t *self)
{
    return;
}
/********* end of Object *********/

/********* Animal *********/

typedef struct Animal_t
{
    Object_t obj;
    int animal_num_legs;
    int animal_num_masters;
    int ID;

} Animal_t;

typedef void (*sayhello_func_t)(Animal_t *);
typedef int (*get_num_masters_func_t)(Animal_t *);

static class_metadata Animal_metadata;
static int is_animal_init = NOT_INIT;

static int animal_counter;

/* Animal function prototypes */
void Animal_static_init(void);
char *Animal_toString(Object_t *self, char *buffer);
void Animal_finalize(Object_t *self);
void Animal_sayHello(Animal_t *animal);
int Animal_getNumMasters(Animal_t *animal);
void Animal_showCounter(void);
void Animal_non_static_init(Animal_t *animal);
void Animal_instance_init_int(Animal_t *animal, int num_masters);
void Animal_instance_init_void(Animal_t *animal);

void Animal_static_init(void)
{
    Object_static_init();

    if (is_animal_init == NOT_INIT)
    {
        /* class initialization */
        static vfunc_t animal_vtable[] = {
            (void (*)(void))Animal_toString,
            (void (*)(void))Animal_finalize,
            (void (*)(void))Animal_sayHello,
            (void (*)(void))Animal_getNumMasters};

        Animal_metadata.class_name = "Animal_t";
        Animal_metadata.object_size = sizeof(Animal_t);
        Animal_metadata.super_class = &Object_metadata;
        Animal_metadata.vtable = (void **)&animal_vtable;

        /* static init blocks */
        printf("Static block Animal 1\n");
        printf("Static block Animal 2\n");

        /* static variables init */
        animal_counter = 0;

        is_animal_init = INIT;
    }
}

void Animal_non_static_init(Animal_t *animal)
{
    animal->animal_num_legs = 5;
    animal->animal_num_masters = 1;
    printf("Instance initialization block Animal\n");
}

void Animal_instance_init_int(Animal_t *animal, int num_masters)
{
    /* first execute non-static initialization block, then instance variables*/
    Animal_non_static_init(animal);

    animal->animal_num_masters = num_masters;
    animal->ID = ++animal_counter;

    /* init animal metadata (vtable, class_name, parent_class)*/
    if (animal->obj.metadata == NULL || animal->obj.metadata->class_name == "Object_t")
    {
        animal->obj.metadata = &Animal_metadata;
    }

    printf("Animal Ctor int\n");
}

void Animal_instance_init_void(Animal_t *animal)
{

    Animal_non_static_init(animal);

    printf("Animal Ctor\n");
    animal->ID = ++animal_counter;

    /* update current object's metadata only if it's NOT a child of Animal */
    /* otherwise, Animal's child classes's vtables will be overriden back to Animal_t vtable */
    if (animal->obj.metadata == NULL || animal->obj.metadata->class_name == "Object_t")
    {
        animal->obj.metadata = &Animal_metadata;
    }

    ((sayhello_func_t)(animal->obj.metadata->vtable[SAY_HELLO_FUNC]))(animal);

    Animal_showCounter();

    char tmp_to_string1[64] = {0};
    ((tostring_func_t)(animal->obj.metadata->vtable[TO_STRING_FUNC]))((Object_t *)animal, tmp_to_string1);
    printf("%s\n", tmp_to_string1);

    char tmp_to_string2[64] = {0};
    ((tostring_func_t)(Animal_metadata.super_class->vtable[TO_STRING_FUNC]))((Object_t *)animal, tmp_to_string2);
    printf("%s\n", tmp_to_string2);
}

Animal_t *Animal_new_int(int num_masters)
{
    /* ensure static initialization is done */
    Animal_static_init();

    Animal_t *animal = (Animal_t *)malloc(sizeof(Animal_t));
    if (!animal)
    {
        return NULL;
    }

    Object_instance_init_object(&(animal->obj));

    Animal_instance_init_int(animal, num_masters);

    return animal;
}

Animal_t *Animal_new_void(void)
{
    /* ensure static initialization is done */
    Animal_static_init();

    Animal_t *animal = (Animal_t *)malloc(sizeof(Animal_t));
    if (!animal)
    {
        return NULL;
    }

    /* initialize the Object part of Animal */
    Object_instance_init_object((&(animal->obj)));

    Animal_instance_init_void(animal);

    return animal;
}

/* Animal function implementations */
char *Animal_toString(Object_t *self, char *buffer)
{
    Animal_t *animal = (Animal_t *)self;
    sprintf(buffer, "Animal with ID: %d\n", animal->ID);

    return buffer;
}

void Animal_finalize(Object_t *self)
{
    Animal_t *animal = (Animal_t *)self;
    printf("finalize Animal with ID: %d\n", animal->ID);

    ((finalize_func_t)(animal->obj.metadata->super_class->vtable[FINALIZE_FUNC]))((Object_t *)animal);
}

void Animal_sayHello(Animal_t *animal)
{
    printf("Animal Hello!\n");
    printf("I have %d legs\n", animal->animal_num_legs);
}

int Animal_getNumMasters(Animal_t *animal)
{
    return animal->animal_num_masters;
}

void Animal_showCounter(void)
{
    printf("%d\n", animal_counter);
}

/*********** end of animal class ***********/

/*********** class Dog ***********/

typedef struct Dog_t
{
    Animal_t animal;
    int dog_num_legs;
} Dog_t;

static class_metadata Dog_metadata;
static int is_dog_init = NOT_INIT;

/* Dog function prototypes */
void Dog_static_init(void);
char *Dog_toString(Object_t *self, char *buffer);
void Dog_finalize(Object_t *self);
void Dog_sayHello(Animal_t *animal);
void Dog_non_static_init(Dog_t *animal);
void Dog_instance_init_void(Dog_t *animal);

void Dog_static_init(void)
{
    Animal_static_init();

    if (is_dog_init == NOT_INIT)
    {
        /* class initialization */
        static vfunc_t dog_vtable[] = {/* TODO */
                                       (void (*)(void))Dog_toString,
                                       (void (*)(void))Dog_finalize,
                                       (void (*)(void))Dog_sayHello,
                                       (void (*)(void))Animal_getNumMasters};

        Dog_metadata.class_name = "Dog_t";
        Dog_metadata.object_size = sizeof(Dog_t);
        Dog_metadata.super_class = &Animal_metadata;
        Dog_metadata.vtable = (void **)&dog_vtable;

        /* static init blocks */
        printf("Static block Dog\n");

        is_dog_init = INIT;
    }
}

void Dog_non_static_init(Dog_t *dog)
{
    dog->dog_num_legs = 4;
    printf("Instance initialization block Dog\n");
}

void Dog_instance_init_void(Dog_t *dog)
{
    printf("Dog Ctor\n");
}

Dog_t *Dog_new_void(void)
{
    /* ensure static initialization is done */
    Dog_static_init();

    Dog_t *dog = (Dog_t *)malloc(sizeof(Dog_t));
    if (!dog)
    {
        return NULL;
    }

    /* update Dog's metadata, vtable, etc */
    dog->animal.obj.metadata = &Dog_metadata;

    /* initialize the Animal part of Dog super(2) */
    Animal_instance_init_int((&(dog->animal)), 2);

    Dog_non_static_init(dog);

    Dog_instance_init_void(dog);

    return dog;
}

/* Dog function implementations */
char *Dog_toString(Object_t *self, char *buffer)
{
    Dog_t *dog = (Dog_t *)self;
    sprintf(buffer, "Dog with ID: %d\n", dog->animal.ID);

    return buffer;
}

void Dog_finalize(Object_t *self)
{
    Dog_t *dog = (Dog_t *)self;
    printf("finalize Dog with ID: %d\n", dog->animal.ID);

    ((finalize_func_t)(dog->animal.obj.metadata->super_class->vtable[FINALIZE_FUNC]))(self);
}

void Dog_sayHello(Animal_t *animal)
{
    Dog_t *dog = (Dog_t *)animal;
    printf("Dog Hello!\n");
    printf("I have %d legs\n", dog->dog_num_legs);
}

/*********** end of class Dog ***********/

/*********** class Cat ***********/

typedef struct Cat_t
{
    Animal_t animal;
    char *colors;
    int cat_num_masters;
} Cat_t;

static class_metadata Cat_metadata;
static int is_cat_init = NOT_INIT;

/* Cat function prototypes */
void Cat_static_init(void);
char *Cat_toString(Object_t *self, char *buffer);
void Cat_finalize(Object_t *self);
void Cat_non_static_init(Cat_t *cat);
void Cat_instance_init_void(Cat_t *cat);
void Cat_instance_init_string(Cat_t *cat, char *colors);

void Cat_static_init(void)
{
    Animal_static_init();

    if (is_cat_init == NOT_INIT)
    {
        /* class initialization */
        static vfunc_t cat_vtable[] = {
            (void (*)(void))Cat_toString,
            (void (*)(void))Cat_finalize,
            (void (*)(void))Animal_sayHello,
            (void (*)(void))Animal_getNumMasters};

        Cat_metadata.class_name = "Cat_t";
        Cat_metadata.object_size = sizeof(Cat_t);
        Cat_metadata.super_class = &Animal_metadata;
        Cat_metadata.vtable = (void **)&cat_vtable;

        /* static init blocks */
        printf("Static block Cat\n");

        is_cat_init = INIT;
    }
}

void Cat_non_static_init(Cat_t *cat)
{
    cat->cat_num_masters = 5;
}

void Cat_instance_init_void(Cat_t *cat)
{
    Animal_instance_init_void(&(cat->animal));

    Cat_non_static_init(cat);

    Cat_instance_init_string(cat, "black");

    printf("Cat Ctor\n");

    cat->cat_num_masters = 2;
}

void Cat_instance_init_string(Cat_t *cat, char *colors)
{
    Cat_non_static_init(cat);

    cat->colors = colors;

    printf("Cat Ctor with color: %s\n", cat->colors);
}

Cat_t *Cat_new_void(void)
{
    /* ensure static initialization is done */
    Cat_static_init();

    Cat_t *cat = (Cat_t *)malloc(sizeof(Cat_t));
    if (!cat)
    {
        return NULL;
    }

    cat->animal.obj.metadata = &Cat_metadata;

    Cat_instance_init_void(cat);

    return cat;
}

Cat_t *Cat_new_string(char *colors)
{
    Cat_static_init();

    Cat_t *cat = (Cat_t *)malloc(sizeof(Cat_t));
    if (!cat)
    {
        return NULL;
    }

    cat->animal.obj.metadata = &Cat_metadata;

    Animal_instance_init_void(&(cat->animal));

    Cat_instance_init_string(cat, colors);

    return cat;
}

/* Cat function implementations */
char *Cat_toString(Object_t *self, char *buffer)
{
    Cat_t *cat = (Cat_t *)self;
    sprintf(buffer, "Cat with ID: %d\n", cat->animal.ID);

    return buffer;
}

void Cat_finalize(Object_t *self)
{
    Cat_t *cat = (Cat_t *)self;
    printf("finalize Cat with ID: %d\n", cat->animal.ID);

    ((finalize_func_t)(cat->animal.obj.metadata->super_class->vtable[FINALIZE_FUNC]))(self);
}

/*********** end of class Cat ***********/

/*********** class LegendaryAnimal ***********/

typedef struct LegendaryAnimal_t
{
    Cat_t cat;
} LegendaryAnimal_t;

static class_metadata LegendaryAnimal_metadata;
static int is_legendaryanimal_init = NOT_INIT;

/* Cat function prototypes */
LegendaryAnimal_t *LegendaryAnimal_new_void(void);
char *LegendaryAnimal_toString(Object_t *self, char *buffer);
void LegendaryAnimal_static_init(void);
void LegendaryAnimal_finalize(Object_t *self);
void LegendaryAnimal_non_static_init(LegendaryAnimal_t *la);
void LegendaryAnimal_instance_init_void(LegendaryAnimal_t *la);
void LegendaryAnimal_sayHello(Animal_t *animal);

void LegendaryAnimal_static_init(void)
{
    Cat_static_init();

    if (is_legendaryanimal_init == NOT_INIT)
    {
        /* class initialization */
        static vfunc_t legendaryanimal_vtable[] = {
            (void (*)(void))LegendaryAnimal_toString,
            (void (*)(void))LegendaryAnimal_finalize,
            (void (*)(void))LegendaryAnimal_sayHello,
            (void (*)(void))Animal_getNumMasters};

        LegendaryAnimal_metadata.class_name = "LegendaryAnimal_t";
        LegendaryAnimal_metadata.object_size = sizeof(LegendaryAnimal_t);
        LegendaryAnimal_metadata.super_class = &Cat_metadata;
        LegendaryAnimal_metadata.vtable = (void **)&legendaryanimal_vtable;

        /* static init blocks */
        printf("Static block Legendary Animal\n");

        is_legendaryanimal_init = INIT;
    }
}

void LegendaryAnimal_non_static_init(LegendaryAnimal_t *la)
{
    return;
}

void LegendaryAnimal_instance_init_void(LegendaryAnimal_t *la)
{
    LegendaryAnimal_non_static_init(la);

    printf("Legendary Ctor\n");
}

LegendaryAnimal_t *LegendaryAnimal_new_void(void)
{
    /* ensure static initialization is done */
    LegendaryAnimal_static_init();

    LegendaryAnimal_t *la = (LegendaryAnimal_t *)malloc(sizeof(LegendaryAnimal_t));
    if (!la)
    {
        return NULL;
    }

    la->cat.animal.obj.metadata = &LegendaryAnimal_metadata;

    Cat_instance_init_void(&(la->cat));

    LegendaryAnimal_instance_init_void(la);

    return la;
}

/* LegendaryAnimal function implementations */
char *LegendaryAnimal_toString(Object_t *self, char *buffer)
{
    LegendaryAnimal_t *la = (LegendaryAnimal_t *)self;
    sprintf(buffer, "LegendaryAnimal with ID: %d\n", la->cat.animal.ID);

    return buffer;
}

void LegendaryAnimal_finalize(Object_t *self)
{
    LegendaryAnimal_t *la = (LegendaryAnimal_t *)self;
    printf("finalize LegendaryAnimal with ID: %d\n", la->cat.animal.ID);

    ((finalize_func_t)(la->cat.animal.obj.metadata->super_class->vtable[FINALIZE_FUNC]))(self);
}

void LegendaryAnimal_sayHello(Animal_t *self)
{
    printf("Legendary Hello!\n");
}

/*********** end of class LegendaryAnimal ***********/

/*********** main ***********/

void foo(Animal_t *animal);

int main()
{
    int i = 0;
    int num_masters = 0;

    Object_t obj;
    Animal_t *animal = Animal_new_void();
    Dog_t *dog = Dog_new_void();
    Cat_t *cat = Cat_new_void();
    LegendaryAnimal_t *la = LegendaryAnimal_new_void();

    Animal_t *dog_cast_to_animal = (Animal_t *)dog;
    Animal_t *cat_cast_to_animal = (Animal_t *)cat;
    Animal_t *la_cast_to_animal = (Animal_t *)la;

    Animal_showCounter();
    printf("%d\n", animal->ID);
    printf("%d\n", ((dog_cast_to_animal)->ID));
    printf("%d\n", ((cat_cast_to_animal)->ID));
    printf("%d\n", ((la_cast_to_animal)->ID));

    Animal_t *array[] = {
        (Animal_t *)Dog_new_void(),
        (Animal_t *)Cat_new_void(),
        (Animal_t *)Cat_new_string("white"),
        (Animal_t *)LegendaryAnimal_new_void(),
        (Animal_t *)Animal_new_void()};

    for (i = 0; i < 5; ++i)
    {
        ((sayhello_func_t)(array[i]->obj.metadata->vtable[SAY_HELLO_FUNC]))((Animal_t *)array[i]);

        num_masters = ((get_num_masters_func_t)(array[i]->obj.metadata->vtable[GET_NUM_MASTERS_FUNC]))((Animal_t *)array[i]);
        printf("%d\n", num_masters);
    }

    for (i = 0; i < 5; ++i)
    {
        foo((Animal_t *)array[i]);
    }

    return 0;
}

void foo(Animal_t *animal)
{
    char tmp_to_string[64] = {0};
    ((tostring_func_t)(animal->obj.metadata->vtable[TO_STRING_FUNC]))((Object_t *)animal, tmp_to_string);
    printf("%s\n", tmp_to_string);
}