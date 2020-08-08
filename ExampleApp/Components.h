#pragma once

struct RigidBody
{
	int x, y;
	int vx, vy;
	int ax, ay;

	RigidBody() = default;
	RigidBody(int x, int y, int vx, int vy, int ax = 0, int ay = 0)
		: x(x), y(y), vx(vx), vy(vy), ax(ax), ay(ay)
	{
	}
};

struct Size
{
	int width, height;

	Size() = default;
	
	Size(int width, int height)
		: width(width), height(height)
	{
	}
};


struct Gravity
{
	int magnitude;

	Gravity() = default;

	Gravity(int magnitude)
		: magnitude(magnitude)
	{
	}
};
