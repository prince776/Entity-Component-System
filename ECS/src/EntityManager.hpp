#pragma once

#include <queue>
#include <array>

#include "Base.hpp"

class EntityManager
{
public:
	EntityManager()
	{
		// Initialize the queue with all possible entities.
		for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
		{
			m_AvailableEntities.push(entity);
		}
	}

	Entity CreateEntity()
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Can't create more entities, cap reached!");

		// Create Entity.
		Entity entity = m_AvailableEntities.front();
		m_AvailableEntities.pop();

		m_LivingEntityCount++;

		return entity;
	}

	void DestroyEntity(Entity entity)
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Entity out of range");
		assert(m_LivingEntityCount >= 0 && "Invalid entity");

		// Invalidate the destroyed entity's signature
		m_Signatures[entity].reset();

		// Put the destroyed entity back in the available queue
		m_AvailableEntities.push(entity);

		m_LivingEntityCount--;
	}

	void SetSignature(Entity entity, Signature signature)
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Entity out of range");
		assert(m_LivingEntityCount >= 0 && "Invalid entity");

		m_Signatures[entity] = signature;
	}

	Signature GetSignature(Entity entity) const
	{
		assert(m_LivingEntityCount < MAX_ENTITIES && "Entity out of range");
		assert(m_LivingEntityCount >= 0 && "Invalid entity");

		return m_Signatures[entity];
	}

private:
	// Queue of unused entity IDs.
	std::queue<Entity> m_AvailableEntities{};

	// Array of signatures where the index corresponds to the entity ID
	std::array<Signature, MAX_ENTITIES> m_Signatures{};

	// Total living entities.
	uint32_t m_LivingEntityCount{ 0 };
};
