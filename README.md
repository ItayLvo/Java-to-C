# Java to C Object-Oriented Simulation Project

## Overview and purpose

**This project translates object-oriented programming concepts from Java into C by implementing inheritance, polymorphism, and class structures.**

Through manual memory management and virtual function tables, **the code emulates a simplified version of Java's runtime type identification and method dispatch mechanisms**, allowing C to operate in an object-oriented fashion.

The project focused on precise memory representation of object-oriented structures, including polymorphic and compind objects, to simulate Java-like behavior within C's memory model.


## Project Structure
The Java file (java2c.java) showcases a typical object-oriented program that uses inheritance and polymorphism, and the corresponding C implementation (test/java2c.c) replicates that behavior.

The project implements a series of classes (Object, Animal, Dog, Cat, and LegendaryAnimal) with relationships and behaviors similar to their Java counterparts.

### Classes/Types in both the Java and C implementations:
1. **Animal**: The base class for all animals, contains fields, instance methods and static methods.

2. **Dog (extends Animal)**:
    - Inherits from `Animal` and overrides methods (like sayHello()).
    - Has its own instance variables.

3. **Cat (extends Animal)**:
    - Another class that inherits from Animal.

4. **LegendaryAnimal (extends Cat)**:
    - Further specialization of Cat.
    - Overrides certain methods and adds specific functionality.
    
5. **metadata_class**:
	- Used in the C implementation to serve as the metadata for each "class".
	- Each class's metadata holds information about its name, size in memory, virtual table that holds its virtual functions (overriden or not), and information about its super class.
