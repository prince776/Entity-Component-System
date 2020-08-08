#pragma once

// From ECS
#include "ECS.hpp"

// From this app
#include "Components.h"

extern int width;
extern int height;
extern void SetColor(float, float, float);
extern void FillQuad(float, float, float, float);

class RigidBodySystem : public System
{
public:
	RigidBodySystem() = default;

	void Update(ECS& ecs)
	{
		for (const auto& entity : m_Entities)
		{
			auto& rigidBody = ecs.GetComponent<RigidBody>(entity);
			auto& size = ecs.GetComponent<Size>(entity);

			rigidBody.vx += rigidBody.ax;
			rigidBody.vy += rigidBody.ay;

			rigidBody.x += rigidBody.vx;
			rigidBody.y += rigidBody.vy;

			rigidBody.ax = 0;
			rigidBody.ay = 0;

			// Simple bounds check which shouldn't be here
			if (rigidBody.x < 0)
				rigidBody.x = 0;
			if (rigidBody.y < 0)
				rigidBody.y = 0;

			if (rigidBody.x + size.width > width)
				rigidBody.x = width - size.width;
			if (rigidBody.y + size.height > height)
				rigidBody.y = height - size.height;
		}
	}
};

class RenderSystem : public System
{
public:
	void Update(ECS& ecs)
	{
		for (const auto& entity : m_Entities)
		{
			auto& rigidBody = ecs.GetComponent<RigidBody>(entity);
			auto& size = ecs.GetComponent<Size>(entity);
			
			SetColor(55.0f / 255, 222.0f / 255, 61.0f / 255);
			FillQuad(rigidBody.x, rigidBody.y, size.width, size.height);
		}
	}
};

class GravitySystem : public System
{
public:
	GravitySystem() = default;

	void Update(ECS &ecs)
	{
		for (const auto& entity : m_Entities)
		{
			auto& rigidBody = ecs.GetComponent<RigidBody>(entity);
			auto& gravity = ecs.GetComponent<Gravity>(entity);

			rigidBody.ay += gravity.magnitude;
		}
	}
};
