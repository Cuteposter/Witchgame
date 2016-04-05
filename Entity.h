#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>

#include "Component.h"
class Entity
{
public:
	Entity();
	Entity(int eid);
	~Entity();

	std::vector<Component*> components;

	void add(Component* c);
	int getID();

	//Returns returns the index of the first component of type T
	//Otherwise returns -1
	template <typename T>
	int getComponentIndex()
	{
		int i = 0;
		for (Component* c : components)
		{
			if (dynamic_cast<T*>(c))
			{
				return i;
			}
			i++;
		}

		return -1;
	}

	//Returns a component pointer of the specified type T
	template <typename T>
	T* getComponent()
	{
		for (Component* c : components)
		{
			if (dynamic_cast<T*>(c))
			{
				//std::cout << typeid(static_cast<T*>(c)).name() << '\n';
				return static_cast<T*>(c);
			}
		}

		return NULL;
	}


	//Returns a vector of component pointers of the specified type T
	template <typename T>
	std::vector<T*> get()
	{
		std::vector<T*> temp;
		for (Component* c : components)
		{
			if (dynamic_cast<T*>(c))
			{
				//std::cout << typeid(static_cast<T*>(c)).name() << '\n';
				temp.push_back(static_cast<T*>(c));
			}
		}

		return temp;
	}

protected:
	int id;
};

