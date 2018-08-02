#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <game.h>

static float computeVelocity(float v, float a, float d, float max, float dt)
{
	if (a != 0)
	{
		v += a * dt;
	}
	else if (d != 0)
	{
		float drag = d * dt;

		if (v - drag > 0)
		{
			v -= drag;
		}
		else if (v + drag < 0)
		{
			v += drag;
		}
		else
		{
			v = 0;
		}
	}

	if (v != 0 && fabs(max) > 0)
	{
		if (v > max)
		{
			v = max;
		}
		else if (v < -max)
		{
			v = -max;
		}
	}

	return v;
}

struct GameObject
{
	float LastX, LastY;
	float PosX, PosY;
	float VelX, VelY;
	float DrgX, DrgY;
	float AccX, AccY;
	float MaxX, MaxY;
	float Bounce;

	GameObject()
	{
		LastX = LastY = PosX = PosY = VelX = VelY = DrgX = DrgY = AccX = AccY = MaxX = MaxY = 0;
		Bounce = 1.0f;
	}

	virtual void Update(float dt)
	{
		float deltaV = 0;
		float delta = 0;

		LastX = PosX;
		LastY = PosY;

		deltaV = 0.5f * (computeVelocity(VelX, AccX, DrgX, MaxX, dt) - VelX);
		VelX += deltaV;
		delta = VelX * dt;
		VelX += deltaV;
		PosX += delta;

		deltaV = 0.5f * (computeVelocity(VelY, AccY, DrgY, MaxY, dt) - VelY);
		VelY += deltaV;
		delta = VelY * dt;
		VelY += deltaV;
		PosY += delta;
	}

	virtual void Draw() = 0;
	virtual void Destroy() = 0;
};

struct Sprite : public GameObject
{
	unsigned int Width, Height;
	short* String;
	short* Colours;

	Sprite(unsigned int width = 1, unsigned int height = 1)
	{
		Width = width;
		Height = height;
		String = new short[Width * Height];
		Colours = new short[Width * Height];
	}

	virtual void Draw()
	{
		for (unsigned int y = 0; y < Height; y++)
		{
			for (unsigned int x = 0; x < Width; x++)
			{
				short bg = Colours[y * Width + x] / 16;

				if (bg == 0)
				{
					if (getCurrentChar(x + (int)PosX, y + (int)PosY) == 219)
					{
						bg = getCurrentFColour(x + (int)PosX, y + (int)PosY);
					}
				}

				drawChar(x + (int)PosX, y + (int)PosY, String[y * Width + x], Colours[y * Width + x] % 16, bg);
			}
		}
	}

	virtual void Destroy()
	{
		delete[] String;
		delete[] Colours;
	}
};