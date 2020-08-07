#pragma once

#include <unordered_map>
#include <memory>

#include "Base.hpp"
#include "ComponentArray.hpp"

class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();
		assert(m_ComponentTypes.find(typeName) == m_ComponentTypes.end() && "Cannot register a type more than once.");

		// Add this component type to the component type map.
		m_ComponentTypes.insert({ typeName, m_NextComponentType });

		// Create a ComponentArray pointer and add it to the map
		m_ComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

		m_NextComponentType++;
	}

	// Get the component type after registering, so that signature can be created.
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();
		assert(m_ComponentTypes.find(typeName) != m_ComponentTypes.end() && "Component not registered before use.");

		return m_ComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(Entity entity, T component) // TODO: Take component by ref.
	{
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		// Notify each component array that an entity has been destroyed
		// If it has a component for that entity, it will remove it.
		for (const auto& pair : m_ComponentArrays)
		{
			const auto& component = pair.second;
			component->EntityDestroyed(entity);
		}
	}

private:
	// Map from type string pointer to a component type.
	std::unordered_map<const char*, ComponentType> m_ComponentTypes{};

	// Map from type string pointer to a component array pointer.
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_ComponentArrays;

	// The component type to be assigned to the next component.
	ComponentType m_NextComponentType{ 0 };

	// Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(m_ComponentArrays.find(typeName) != m_ComponentArrays.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(m_ComponentArrays[typeName]);
	}
};
