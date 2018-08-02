#pragma once
#include <game.h>
#include "Tilemap.h"

//Enemies must check for collisions against the player, tilemap and paddles.

struct Enemy : public Sprite
{
	bool alive = true;
	float deadTimer = 0.0f;

	Enemy(unsigned int width = 1, unsigned int height = 1) : Sprite(width, height) {}
	virtual void Process(float dt, Tilemap* map, Paddle* left, Paddle* right, Sprite** bullets, bool* activeBullets, int bulletCount);
	virtual bool PlayerCollision(float dt, Sprite* Player);
	virtual void Draw();
};

void Enemy::Process(float dt, Tilemap* map, Paddle* left, Paddle* right, Sprite** bullets, bool* activeBullets, int bulletCount)
{
	if (!alive)
	{
		deadTimer += dt;
		return;
	}

	if (map)
		map->CollisionTest(this);

	if (left)
		left->CollisionTest(this);

	if (right)
		right->CollisionTest(this);

	if (bullets && activeBullets)
	{
		for (int i = 0; i < bulletCount; i++)
		{
			if (activeBullets[i])
			{
				if (bullets[i]->PosX >= PosX && bullets[i]->PosX < PosX + Width &&
					bullets[i]->PosY >= PosY && bullets[i]->PosY < PosY + Height)
				{
					alive = false;
					playAudio(1);
				}
			}
		}
	}
}

bool Enemy::PlayerCollision(float dt, Sprite* Player)
{
	if (Player == NULL)
		return false;

	if (!alive)
		return false;

	return PosX < Player->PosX + Player->Width &&
		PosX + Width > Player->PosX &&
		PosY < Player->PosY + Player->Height &&
		PosY + Height > Player->PosY;
}

void Enemy::Draw()
{
	if (alive)
		Sprite::Draw();
	else
	{
		srand((unsigned int)(PosX * PosY));

		for (int i = 0; i < 24; i++)
		{
			float x = (rand() % 4) - 2.0f;
			float y = (rand() % 4) - 3.0f;
			float length = (x * x) + (y * y);
			x /= length;
			y /= length;

			x *= 10 * deadTimer;
			y *= 15 * deadTimer;

			y += 5 * (deadTimer * deadTimer);

			char c = '*';

			if (deadTimer > 2.0f)
				c = (char)178;
			else if (deadTimer > 1.0f)
				c = '#';
			else if (deadTimer > 0.5f)
				c = '+';

			drawChar((int)(x + PosX), (int)(y + PosY), c, COLOUR::YELLOW);
		}
	}
}

//Types:
//Follow Enemy
//Charge Enemy
//Shoot Enemy
//Bounce Enemy

struct FollowEnemy : public Enemy
{
	FollowEnemy() : Enemy(5, 3)
	{
		String[1] = String[13] = '/';
		String[3] = String[11] = '\\';
		String[5] = '<';
		String[7] = 'O';
		String[9] = '>';
		String[0] = String[2] = String[4] = String[6] = String[8] = String[10] = String[12] = String[14] = ' ';

		Colours[1] = Colours[13] = COLOUR::RED;
		Colours[3] = Colours[11] = COLOUR::RED;
		Colours[5] = COLOUR::RED;
		Colours[7] = COLOUR::GREY;
		Colours[9] = COLOUR::RED;
		Colours[0] = Colours[2] = Colours[4] = Colours[6] = Colours[8] = Colours[10] = Colours[12] = Colours[14] = 0;

		MaxX = 5.0f;
		MaxY = 5.0f;
	}

	virtual bool PlayerCollision(float dt, Sprite* player)
	{
		if (!alive)
			return false;

		AccX = player->PosX - PosX;
		AccY = player->PosY - PosY;
		float length = sqrtf((AccX * AccX) + (AccY * AccY));

		AccX *= 30.0f / length;
		AccY *= 30.0f / length;

		return Enemy::PlayerCollision(dt, player);
	}
};

struct ChargeEnemy : public Enemy
{
	bool charge = false;
	float timer = 0.0f;

	ChargeEnemy() : Enemy(3, 3)
	{
		String[1] = 'A';
		String[3] = '<';
		String[4] = 'H';
		String[5] = '>';
		String[7] = 'V';
		String[0] = String[2] = String[6] = String[8] = ' ';

		Colours[1] = COLOUR::GREY;
		Colours[3] = COLOUR::GREY;
		Colours[4] = COLOUR::GREY;
		Colours[5] = COLOUR::GREY;
		Colours[7] = COLOUR::GREY;
		Colours[0] = Colours[2] = Colours[6] = Colours[8] = 0;
	}

	virtual bool PlayerCollision(float dt, Sprite* player)
	{
		if (!alive)
			return false;

		timer += dt;

		if (!charge)
		{
			if (timer > 2.0f)
			{
				Colours[1] = COLOUR::RED;
			}
			else if (timer > 1.0f)
			{
				Colours[3] = COLOUR::RED;
				Colours[4] = COLOUR::RED;
				Colours[5] = COLOUR::RED;
			}
			else
			{
				Colours[7] = COLOUR::RED;
			}

			Colours[0] = Colours[2] = Colours[6] = Colours[8] = 0;
		}
		else
		{
			Colours[1] = COLOUR::GREY;
			Colours[3] = COLOUR::GREY;
			Colours[4] = COLOUR::GREY;
			Colours[5] = COLOUR::GREY;
			Colours[7] = COLOUR::GREY;
			Colours[0] = Colours[2] = Colours[6] = Colours[8] = 0;
		}

		if (timer > 3.0f)
		{
			if (charge)
			{
				VelX = VelY = 0.0f;
			}
			else
			{
				VelX = player->PosX - PosX;
				VelY = player->PosY - PosY;
				float length = sqrtf((VelX * VelX) + (VelY * VelY));

				VelX *= 50.0f / length;
				VelY *= 50.0f / length;
			}

			charge = !charge;
			timer = 0.0f;
		}

		return Enemy::PlayerCollision(dt, player);
	}
};

struct ShootEnemy : public Enemy
{
	Sprite* bullet[5];
	bool bulletActive[5];
	float bulletTimer = 1.0f;

	ShootEnemy() : Enemy(3, 1)
	{
		String[0] = '(';
		String[1] = '0';
		String[2] = ')';

		Colours[0] = COLOUR::RED;
		Colours[1] = COLOUR::WHITE;
		Colours[2] = COLOUR::RED;

		for (int i = 0; i < 5; i++)
		{
			bullet[i] = new Sprite(1, 1);
			bullet[i]->String[0] = 'X';
			bullet[i]->Colours[0] = COLOUR::RED;
			bulletActive[i] = false;
		}
	}

	virtual void Process(float dt, Tilemap* map, Paddle* left, Paddle* right, Sprite** bullets, bool* activeBullets, int bulletCount)
	{
		Enemy::Process(dt, map, left, right, bullets, activeBullets, bulletCount);

		for (int i = 0; i < 5; i++)
		{
			bullet[i]->Update(dt);
			if (bulletActive[i] && map->CollisionTest(bullet[i]))
			{
				bulletActive[i] = false;
			}
		}
	}

	virtual bool PlayerCollision(float dt, Sprite* player)
	{
		if (!alive)
			return false;

		float DiffX = player->PosX - PosX;
		float DiffY = player->PosY - PosY;
		float length = sqrtf((DiffX * DiffX) + (DiffY * DiffY));

		if (length < 50.0f)
		{
			bulletTimer += dt;

			if (bulletTimer > 1.0f)
			{
				for (int i = 0; i < 5; i++)
				{
					if (!bulletActive[i])
					{
						bullet[i]->VelX = DiffX * 16.0f / length;
						bullet[i]->VelY = DiffY * 16.0f / length;

						if (bullet[i]->VelX != 0 || bullet[i]->VelY != 0)
						{
							bullet[i]->PosX = PosX + (Width / 2);
							bullet[i]->PosY = PosY + (Height / 2);

							bulletActive[i] = true;
							playAudio(2);
						}

						break;
					}
				}

				bulletTimer = 0.0f;
			}
		}
		else
		{
			bulletTimer = 1.0f;
		}

		for (int i = 0; i < 5; i++)
		{
			if (bulletActive[i])
			{
				if (bullet[i]->PosX < player->PosX + player->Width &&
					bullet[i]->PosX + bullet[i]->Width > player->PosX &&
					bullet[i]->PosY < player->PosY + player->Height &&
					bullet[i]->PosY + bullet[i]->Height > player->PosY)
				{
					return true;
				}
			}

			if (bullet[i]->PosX > 108 || bullet[i]->PosX + bullet[i]->Width < 0 ||
				bullet[i]->PosY > 60 || bullet[i]->PosY + bullet[i]->Height < 0)
			{
				bulletActive[i] = false;
			}
		}

		return Enemy::PlayerCollision(dt, player);
	}

	virtual void Draw()
	{
		Enemy::Draw();

		if (alive)
		{
			for (int i = 0; i < 5; i++)
			{
				if (bulletActive[i])
				{
					bullet[i]->Draw();
				}
			}
		}
	}
};

struct BounceEnemy : public Enemy
{
	BounceEnemy() : Enemy(3, 3)
	{
		String[1] = String[7] = '=';
		String[3] = String[5] = '|';
		String[4] = 'o';
		String[0] = String[2] = String[6] = String[8] = ' ';

		Colours[1] = Colours[7] = COLOUR::RED;
		Colours[3] = Colours[5] = COLOUR::RED;
		Colours[4] = COLOUR::DARK_CYAN;
		Colours[0] = Colours[2] = Colours[6] = Colours[8] = 0;

		float height = 60.0f; 
		float time = 2.0f;
		AccY = (2 * height) / (time * time);
		MaxX = height / time;
		MaxY = 2 * ((2 * height) / time);
	}
};