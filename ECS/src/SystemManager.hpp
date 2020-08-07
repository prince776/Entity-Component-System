#pragma once

#include "Base.hpp"
#include "System.hpp"

#include <unordered_map>
#include <memory>


class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem() // TODO: take constructor arguments
	{
		const char* typeName = typeid(T).name();

		assert(m_Systems.find(typeName) == m_Systems.end() && "Registering system more than once.");

		// Create a pointer to the system and return it so it can be used externally.
		auto system = std::make_shared<T>();
		m_Systems.insert({ typeName, system });

		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(m_Systems.find(typeName) != m_Systems.end() && "System used before registered.");

		// Set the signature of this system.
		m_Signatures.insert({ typeName, signature });
	}

	void EntityDestroyed(Entity entity)
	{
		// Erase a destroyed entity from all systems lists.
		// since m_Entities is a set, no checks are needed.
		for (const auto& pair : m_Systems)
		{
			const auto& system = pair.second;

			system->m_Entities.erase(entity);
		}
	}

	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		// Notify each system that an entity's signature changed.
		for (const auto& pair : m_Systems)
		{
			const auto& type = pair.first;
			const auto& system = pair.second;
			const auto& systemSignature = m_Signatures[type];

			// Entity signature matches system signature - insert into set
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->m_Entities.insert(entity);
			}
			// Entity signature does not match system signature - erase from set
			else
			{
				system->m_Entities.erase(entity);
			}
		}
	}
private:
	// Map from system type string pointer to a signature.
	std::unordered_map<const char*, Signature> m_Signatures{};

	// Map from system type string pointer to a system pointer.
	std::unordered_map<const char*, std::shared_ptr<System>> m_Systems{};
};
