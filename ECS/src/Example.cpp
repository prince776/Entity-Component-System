#include <iostream>
#include <vector>
#include <type_traits>

#include "ECS.hpp"

using namespace std;

#include <chrono>

///////////////////////////////////////////////
// Profiling //////////////////////////////////
///////////////////////////////////////////////

class Timer
{
public:
	Timer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = false;

		float duration = (end - start) * 0.001f;

		cout << "-------------------------------------------------------------------------\n";
		cout << "Timer {" << m_Name << "}: " << duration << "ms" << "\n";
		cout << "-------------------------------------------------------------------------\n";

	}

private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name)

///////////////////////////////////////////////
// Profiling End //////////////////////////////
///////////////////////////////////////////////

// Helper struct
struct Vec2
{
	float x, y;
	Vec2()
		: x(0.0f), y(0.0f)
	{
	}

	Vec2(float x, float y)
		: x(x), y(y)
	{
	}

};

ostream& operator<<(ostream& stream, const Vec2& other)
{
	stream << other.x << ", " << other.y;
	return stream;
}

// Components
struct RigidBodyComponent
{
	Vec2 position;

	RigidBodyComponent() = default;

	RigidBodyComponent(Vec2&& pos)
		: position(move(pos))
	{
	}

	RigidBodyComponent(const Vec2& pos)
		: position(pos)
	{
	}

};

struct GravityComponent
{
	Vec2 force;

	GravityComponent() = default;

	GravityComponent(Vec2&& force)
		: force(move(force))
	{
	}

	GravityComponent(const Vec2& force)
		: force(force)
	{
	}
};

struct WeightComponent
{
	int weight;

	WeightComponent() = default;

	WeightComponent(int weight)
		: weight(weight)
	{
	}
};

// Global for now
ECS ecs;

// Systems

/**
 * Gravity System
 * Signature = RigidBody | Gravity
 */
class GravitySystem : public System
{
public:
	GravitySystem() = default;

	void Update()
	{
		for (const auto& entity : m_Entities)
		{
			auto& rigidBody = ecs.GetComponent<RigidBodyComponent>(entity);
			auto& gravity = ecs.GetComponent<GravityComponent>(entity);

			// Stupid and wrong physics
			rigidBody.position.x -= gravity.force.x;
			rigidBody.position.y -= gravity.force.y;

			cout << "New position for entity after gravity: " << entity << " is: " << rigidBody.position << "\n";
		}
	}
};

/**
 * RigidBody System
 * Signature = RigidBody
 */
class RigidBodySystem : public System
{
public:
	//RigidBodySystem() = default;

	RigidBodySystem(const string& msg)
	{
		cout << "RigidBodySystem constructed with msg: " << msg << "\n";
	}

	void Update()
	{
		for (const auto& entity : m_Entities)
		{
			auto& rigidBody = ecs.GetComponent<RigidBodyComponent>(entity);

			cout << "Position of entity: " << entity << " is: " << rigidBody.position << "\n";
		}
	}
};

int main()
{
	cout << "ECS Test:\n";
	cout << "-------------------------------------------------------------------------\n";

	// Init the ECS.
	ecs.Init();

	// Register all components.
	ecs.RegisterComponent<RigidBodyComponent>();
	ecs.RegisterComponent<GravityComponent>();
	ecs.RegisterComponent<WeightComponent>();
	
	// Create system and set its signature.
	auto gravitySystem = ecs.RegisterSystem<GravitySystem>();
	auto rigidBodySystem = ecs.RegisterSystem<RigidBodySystem>("Message");

	Signature gravitySystemSignature;
	gravitySystemSignature.set(ecs.GetComponentType<RigidBodyComponent>(), true);
	gravitySystemSignature.set(ecs.GetComponentType<GravityComponent>(), true);
	ecs.SetSystemSignature<GravitySystem>(gravitySystemSignature);

	Signature rigidBodySystemSignature;
	rigidBodySystemSignature.set(ecs.GetComponentType<RigidBodyComponent>(), true);
	ecs.SetSystemSignature<RigidBodySystem>(rigidBodySystemSignature);

	// Create entities for testing.
	int numEntities = 30;
	vector<Entity> entities(numEntities);

	// First 10 entities are gonna have both RigidBody and Gravity components.

	{
		PROFILE_SCOPE("Entity Creation");

		for (int i = 0; i < 10; i++)
		{
			entities[i] = ecs.CreateEntity();
			ecs.AddComponent(entities[i], RigidBodyComponent(Vec2(100, 100)));
			ecs.AddComponent(entities[i], GravityComponent(Vec2(0, 1)));
		}

		// Next 10 will only have RigidBody component.
		for (int i = 10; i < 20; i++)
		{
			entities[i] = ecs.CreateEntity();
			ecs.AddComponent(entities[i], RigidBodyComponent(Vec2(100, 100)));
		}

		// Next 10 will only have both RigidBody and Gravity components as well as WeightComponent.
		for (int i = 20; i < numEntities; i++)
		{
			entities[i] = ecs.CreateEntity();
			ecs.AddComponent(entities[i], RigidBodyComponent(Vec2(100, 100)));
			ecs.AddComponent(entities[i], GravityComponent(Vec2(0, 1)));
			ecs.AddComponent(entities[i], WeightComponent(10));
		}

	}

	{
		PROFILE_SCOPE("Update");

		gravitySystem->Update();
		cout << "-------------------------------------------------------------------------\n";
		rigidBodySystem->Update();
	}
}
