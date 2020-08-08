# Entity-Component-System (ECS)

## What is an ECS?
Entity Component System(ECS) is an architectural pattern that is most used in game development to tackle problems caused from inheritance approach.

## Why use an ECS?
With Inheritance approach in a game, complex inheritence trees can be formed and it become less intuitive to choose a class to inherit from as many components are shared along different
paths of the inheritance tree. 
This approach is also cache unfriendly, since the container of Entities usually contains the pointer to the memeory allocated on heap for each object which
are distributed at different places, hence a lot of cache misses happens.
The need to iterate to entities that can't be processed in a loop also occurs.

For example: 

Once we iterate through all entities and apply physcics to only the eligible ones.

Then we iterate through all entities and render only the renderable ones.

As we can see, we are looping through unneccessary objects.


## What ECS does?
ECS follows the approach of composition over inheritance, which means entities are composed of different components. Hence we can have an entity with really varied set of components

which would otherwise be placed at a very confusing place in inheritance approach.
It also follows Data Oriented Design(DOD) and stores all the components in contiguous memory.

## Overview of my implementation of ECS
There are three parts to my implementation:

EntityManager: It manages handing out the id of entities it creates to client, in fact entities are just 32 bit unsigned ints represeting the id.

ComponentManager: It adds componenets of same type in contiguous memory, while also mapping which component is associated with which entity.

SystemManager: It contains a set of entities which are supposed to be processed by the system, which is decided by which components an entity is associated with.


## How to use:
It's a header only library, so no linking is required. To use it we just need to include "ECS.hpp".

Example.cpp explains how to use the ECS.

I've also added UnitTests if that's better to understand the usage.
