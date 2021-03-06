#pragma once

#include "Base.hpp"
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

#include <memory>

class ECS
{
public:
	void Init() // Maybe: change to constructor.
	{
		m_EntityManager = std::make_unique<EntityManager>();
		m_ComponentManager = std::make_unique<ComponentManager>();
		m_SystemManager = std::make_unique<SystemManager>();
	}

	// Entity methods.
	Entity CreateEntity()
	{
		return m_EntityManager->CreateEntity();
	}
	
	void DestroyEntity(Entity entity)
	{
		m_EntityManager->DestroyEntity(entity);
		m_ComponentManager->EntityDestroyed(entity);
		m_SystemManager->EntityDestroyed(entity);
	}

	// Component methods.
	template<typename T>
	void RegisterComponent()
	{
		m_ComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T&& component)
	{
		m_ComponentManager->AddComponent<T>(entity, std::forward<T>(component));

		auto signature = m_EntityManager->GetSignature(entity);
		signature.set(m_ComponentManager->GetComponentType<T>(), true);
		m_EntityManager->SetSignature(entity, signature);

		m_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		m_ComponentManager->RemoveComponent<T>(entity);

		auto signature = m_EntityManager->GetSignature(entity);
		signature.set(m_ComponentManager->GetComponentType<T>(), false);
		m_EntityManager->SetSignature(entity, signature);

		m_SystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return m_ComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return m_ComponentManager->GetComponentType<T>();
	}

	// System methods.
	template<typename T, typename... Args>
	std::shared_ptr<T> RegisterSystem(Args&&... params)
	{
		return m_SystemManager->RegisterSystem<T>(std::forward<Args>(params)...);
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		m_SystemManager->SetSignature<T>(signature);
	}

	const std::unique_ptr<EntityManager>& GetEntityManager() const { return m_EntityManager; }
	const std::unique_ptr<ComponentManager>& GetComponentManager() const { return m_ComponentManager; }
	const std::unique_ptr<SystemManager>& GetSystemManager() const { return m_SystemManager; }

private:
	std::unique_ptr<EntityManager> m_EntityManager;
	std::unique_ptr<ComponentManager> m_ComponentManager;
	std::unique_ptr<SystemManager> m_SystemManager;
};
