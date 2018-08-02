#pragma once
#include <game.h>
#include "gameobject.h"

struct GUI : public GameObject
{
	float timer = 0.0f;
	bool dead = false;
	float deadTimer = 0.0f;

	GUI() : GameObject()
	{
		
	}

	virtual void Update(float dt) override;
	void DrawBiggerNumber(int x, int y, int number, float t = 1.0f);
	virtual void Draw() override;
	virtual void Destroy() {}
};

void GUI::Update(float dt)
{
	GameObject::Update(dt);

	if (dead)
	{
		deadTimer += dt;
	}
	else
	{
		timer += dt;
	}
}

void GUI::DrawBiggerNumber(int x, int y, int number, float t)
{
	char* subMap;
	switch (number)
	{
	case 0:
		subMap =
			"000" \
			"0 0" \
			"0 0" \
			"0 0" \
			"000";
		break;
	case 1:
		subMap =
			" 1 " \
			" 1 " \
			" 1 " \
			" 1 " \
			" 1 ";
		break;
	case 2:
		subMap =
			"222" \
			"  2" \
			"222" \
			"2  " \
			"222";
		break;
	case 3:
		subMap =
			"333" \
			"  3" \
			"333" \
			"  3" \
			"333";
		break;
	case 4:
		subMap =
			"4 4" \
			"4 4" \
			"444" \
			"  4" \
			"  4";
		break;
	case 5:
		subMap =
			"555" \
			"5  " \
			"555" \
			"  5" \
			"555";
		break;
	case 6:
		subMap =
			"666" \
			"6  " \
			"666" \
			"6 6" \
			"666";
		break;
	case 7:
		subMap =
			"777" \
			"  7" \
			"  7" \
			"  7" \
			"  7";
		break;
	case 8:
		subMap =
			"888" \
			"8 8" \
			"888" \
			"8 8" \
			"888";
		break;
	case 9:
		subMap =
			"999" \
			"9 9" \
			"999" \
			"  9" \
			"999";
		break;
	case 10:
		subMap =
			"   " \
			" # " \
			"   " \
			" # " \
			"   ";
		break;
	case 11:
		subMap =
			"   " \
			"   " \
			"   " \
			" # " \
			"   ";
		break;
	default:
		subMap =
			"   " \
			"   " \
			"   " \
			"   " \
			"   ";
		break;
	}

	for (int i = 0; i < 15; i++)
	{
		if (t * 3 > i % 3)
			drawChar(x + (i % 3), y + (i / 3), subMap[i], COLOUR::WHITE);
	}
}

void GUI::Draw()
{
	int minutes = (int)timer / 60;
	int seconds = (int)timer % 60;
	int milliseconds = (int)((timer - seconds) * 1000) % 1000;
	int width = 0;

	if (dead)
	{
		width = (3 + 1 + 2 + 1 + 2) * 3;
		fill((int)PosX - (width / 2), (int)PosY, (int)PosX + (width / 2), (int)PosY + 5, 0, 0);
		DrawBiggerNumber((int)PosX - (width / 2) + 0, (int)PosY, (minutes / 10), deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 4, (int)PosY, (minutes % 10), deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 7, (int)PosY, 10, deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 10, (int)PosY, (seconds / 10), deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 14, (int)PosY, (seconds % 10), deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 17, (int)PosY, 11, deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 20, (int)PosY, (milliseconds / 100), deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 24, (int)PosY, ((milliseconds / 10) % 10), deadTimer);
		DrawBiggerNumber((int)PosX - (width / 2) + 28, (int)PosY, (milliseconds % 10), deadTimer);

		const char* message = "Retry? Press Y or N.";

		for (int i = 0; i < (int)strlen(message); i++)
		{
			drawChar((int)PosX - (strlen(message) / 2) + i, (int)PosY + 30, message[i], COLOUR::WHITE);
		}
	}
	else
	{
		width = 3 + 1 + 2 + 1 + 2;
		drawChar((int)PosX - (width / 2) + 0, (int)PosY, 48 + (minutes / 10), COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 1, (int)PosY, 48 + (minutes % 10), COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 2, (int)PosY, 58, COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 3, (int)PosY, 48 + (seconds / 10), COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 4, (int)PosY, 48 + (seconds % 10), COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 5, (int)PosY, 46, COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 6, (int)PosY, 48 + (milliseconds / 100), COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 7, (int)PosY, 48 + ((milliseconds / 10) % 10), COLOUR::WHITE);
		drawChar((int)PosX - (width / 2) + 8, (int)PosY, 48 + (milliseconds % 10), COLOUR::WHITE);
	}
}