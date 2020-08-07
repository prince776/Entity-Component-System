#pragma once

#include <array>
#include <unordered_map>

#include "Base.hpp"

// Base Class.
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity entity, T component)
	{
		assert(m_EntityToIndexMap.find(entity) == m_EntityToIndexMap.end() && "Component added to same entity more than once.");

		// Put the new entry at the end and update the maps.
		size_t newIndex = m_Size;
		m_ComponentArray[newIndex] = component;

		m_EntityToIndexMap[entity] = newIndex;
		m_IndexToEntityMap[newIndex] = entity;

		m_Size++;
	}

	void RemoveData(Entity entity)
	{
		assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Removing non-existent component.");

		// Copy element at end into deleted element's place to maintain density.
		size_t indexOfRemovedEntity = m_EntityToIndexMap[entity];
		size_t indexOfLastElement = m_Size - 1;

		m_ComponentArray[indexOfRemovedEntity] = m_ComponentArray[indexOfLastElement]; // TODO: Move maybe.

		// Update map to point to moved spot.
		Entity entityOfLastElement = m_IndexToEntityMap[indexOfLastElement];
		m_EntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		m_IndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		// Remove the entity from map whose component has been removed.
		m_EntityToIndexMap.erase(entity);
		m_IndexToEntityMap.erase(indexOfLastElement);

		m_Size--;
	}

	T& GetData(Entity entity)
	{
		assert(m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end() && "Retrieving non-existent component.");

		return m_ComponentArray[m_EntityToIndexMap[entity]];
	}

	void EntityDestroyed(Entity entity) override
	{
		// Remove the entity's component if it existed.
		if (m_EntityToIndexMap.find(entity) != m_EntityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}

private:
	// Contiguous packed(packed in the sense that all the alive components will be together) array of components of Type T.
	std::array<T, MAX_ENTITIES> m_ComponentArray;

	// Map from entity IDs to array indices.
	std::unordered_map<Entity, size_t> m_EntityToIndexMap;

	// Map from the array indices to entity IDs.
	std::unordered_map<size_t, Entity> m_IndexToEntityMap;

	// Size of valid entries in the array.
	size_t m_Size;
};
