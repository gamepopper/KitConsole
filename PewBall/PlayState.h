#pragma once
#include "state.h"
#include <game.h>
#include "gameobject.h"
#include "Tilemap.h"
#include "Enemies.h"
#include "GUI.h"

struct PlayState : public StateObject
{
	static const int NUM_BULLETS = 10;
	static const int NUM_ENEMIES = 64;
	float FIRE_RATE = 0.2f;

	Sprite* ball = NULL;
	Sprite* bullet[NUM_BULLETS];
	bool bulletActive[NUM_BULLETS];
	float bulletTimer = 0.5f;

	bool playerDead = false;
	float deadTimer = 0.0f;

	bool upShoot = true;
	float upShootTime = 0;

	Tilemap* tilemap = NULL;
	Paddle* paddleLeft = NULL;
	Paddle* paddleRight = NULL;

	Enemy* enemy[NUM_ENEMIES];
	float enemySpawnTime = 5.0f;
	float enemyTimer = 0.0f;
	short enemyCount = 0;

	GUI* gui = NULL;

	float t = 0.0f;
	int seed = 0;

	void KillPlayer();

	void SpawnEnemy();
	void DisposeEnemy(int index);

	float a(float d, float e);

	virtual void Initialise();
	virtual void Update(float dt);
	virtual void Render();
	virtual void Cleanup();
};


void PlayState::Initialise()
{
	ball = new Sprite(2, 2);
	ball->String[0] = '/';
	ball->String[1] = '\\';
	ball->String[2] = '\\';
	ball->String[3] = '/';
	ball->Colours[0] = COLOUR::YELLOW;
	ball->Colours[1] = COLOUR::YELLOW;
	ball->Colours[2] = COLOUR::YELLOW;
	ball->Colours[3] = COLOUR::YELLOW;

	srand((unsigned int)time(NULL));
	seed = rand() % 255;

	float height = SCREEN_HEIGHT - 4.0f;
	float distance = SCREEN_WIDTH - 2.0f + (rand() % 3) - 1.0f;
	float t = 3.0f;
	float moveSpeed = distance / t;

	ball->PosX = SCREEN_WIDTH - 4 - 3;
	ball->PosY = SCREEN_HEIGHT - 4 - 2;
	ball->AccY = (2 * height) / (t * t);
	ball->MaxX = moveSpeed;
	ball->MaxY = 2 * ((2 * height) / t);
	ball->VelY = -ball->MaxY * (10 - (rand() % 5)) * 0.1f;
	ball->Bounce = 0.75f;

	for (int i = 0; i < NUM_BULLETS; i++)
	{
		bullet[i] = new Sprite(1, 1);
		bullet[i]->String[0] = 233;
		bullet[i]->Colours[0] = COLOUR::WHITE;
	}

	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		enemy[i] = NULL;
	}

	SpawnEnemy();

	srand((unsigned int)time(NULL));

	tilemap = new Tilemap();
	paddleLeft = new Paddle();
	paddleLeft->PosX = 43;
	paddleLeft->PosY = SCREEN_HEIGHT - 7;
	paddleLeft->key = 0x4F;
	paddleRight = new Paddle();
	paddleRight->PosX = 56;
	paddleRight->PosY = SCREEN_HEIGHT - 7;
	paddleRight->flip = true;
	paddleRight->key = 0x50;

	gui = new GUI();
	gui->PosX = SCREEN_WIDTH / 2;
	gui->PosY = 10;

	playAudio(3);
}

void PlayState::Update(float dt)
{
	t += dt;

	gui->Update(dt);

	if (playerDead)
	{
		deadTimer += dt;

		if (keyState[0x59])
		{
			nextState = GS_GOPLAY;
		}

		if (keyState[0x4E])
		{
			nextState = GS_GOTITLE;
		}
	}
	else
	{
		ball->Update(dt);
		tilemap->Update(dt);
		paddleLeft->Update(dt);
		paddleRight->Update(dt);

		if (keyState[VK_SPACE])
		{
			bulletTimer += dt;

			if (bulletTimer > FIRE_RATE)
			{
				for (int i = 0; i < NUM_BULLETS; i++)
				{
					if (!bulletActive[i])
					{
						bullet[i]->VelX = 0;
						bullet[i]->VelY = 0;

						if (keyState[0x41]) //A Key
							bullet[i]->VelX -= 32.0f;
						if (keyState[0x44]) //D Key
							bullet[i]->VelX += 32.0f;
						if (keyState[0x57]) //W Key
							bullet[i]->VelY -= 32.0f;
						if (keyState[0x53]) //S Key
							bullet[i]->VelY += 32.0f;

						if (bullet[i]->VelX != 0 && bullet[i]->VelY != 0)
						{
							bullet[i]->VelX *= 0.7f;
							bullet[i]->VelY *= 0.7f;
						}

						if (bullet[i]->VelX != 0 || bullet[i]->VelY != 0)
						{
							ball->VelX -= bullet[i]->VelX / 16.0f;
							ball->VelY -= bullet[i]->VelY / 16.0f;

							bullet[i]->PosX = ball->PosX + (ball->Width / 2);
							bullet[i]->PosY = ball->PosY + (ball->Height / 2);

							bulletActive[i] = true;
							playAudio(5);
						}

						break;
					}
				}

				bulletTimer = 0.0f;
			}
		}
		else
		{
			bulletTimer = FIRE_RATE;
		}

		if (upShoot && ball->PosY < 16)
		{
			upShoot = false;
		}
		else if (!upShoot && upShootTime < 10.0f)
		{
			upShootTime += 10.0f * dt;

			for (int i = 0; i < (int)upShootTime; i++)
			{
				if (i < 2)
				{
					tilemap->map[((i + 6) * tilemap->mapWidth) + 25] = 10;
					tilemap->map[((i + 6) * tilemap->mapWidth) + 26] = -1;
				}
				else if (i == 2)
				{
					tilemap->map[((i + 6) * tilemap->mapWidth) + 25] = -1;
					tilemap->map[((i + 6) * tilemap->mapWidth) + 26] = -1;
				}
				else
				{
					tilemap->map[((i + 6) * tilemap->mapWidth) + 24] = -1;
					tilemap->map[((i + 6) * tilemap->mapWidth) + 25] = -1;
					tilemap->map[((i + 6) * tilemap->mapWidth) + 26] = -1;
				}
			}
		}

		if (ball->PosY > SCREEN_HEIGHT)
		{
			KillPlayer();
			ball->PosY = SCREEN_HEIGHT - 3;
		}

		tilemap->CollisionTest(ball);
		paddleLeft->CollisionTest(ball, true);
		paddleRight->CollisionTest(ball, true);

		for (unsigned int i = 0; i < NUM_BULLETS; i++)
		{
			bullet[i]->Update(dt);
			if (bulletActive[i])
			{
				if (bullet[i]->PosX < 0 || bullet[i]->PosX >= SCREEN_WIDTH ||
					bullet[i]->PosY < 0 || bullet[i]->PosY >= SCREEN_HEIGHT)
				{
					bulletActive[i] = false;
				}

				if (tilemap->CollisionTest(bullet[i]))
					bulletActive[i] = false;
			}
			else
			{
				bullet[i]->PosX = -1;
				bullet[i]->PosY = -1;
				bullet[i]->VelX = 0;
				bullet[i]->VelY = 0;
			}
		}

		enemyTimer += dt;
		if ((enemyCount == 0 && enemyTimer > 2.0f) ||
			(enemyTimer > enemySpawnTime))
		{
			SpawnEnemy();
			playAudio(3, false, 1.0f, 3.0f);
			enemySpawnTime -= 0.1f;
			enemyTimer = 0.0f;
		}

		for (unsigned int i = 0; i < NUM_ENEMIES; i++)
		{
			if (enemy[i])
			{
				enemy[i]->Update(dt);
				enemy[i]->Process(dt, tilemap, paddleLeft, paddleRight, bullet, bulletActive, NUM_BULLETS);
				
				if (enemy[i]->PlayerCollision(dt, ball))
				{
					KillPlayer();
				}

				if (enemy[i]->PosY > SCREEN_HEIGHT)
				{
					enemy[i]->PosY = SCREEN_HEIGHT - 3;
					enemy[i]->alive = false;
				}

				if (!enemy[i]->alive && enemy[i]->deadTimer > 3.0f)
					DisposeEnemy(i);
			}
		}
	}
}

void PlayState::KillPlayer()
{
	playerDead = true;
	deadTimer = 0.0f;

	gui->dead = true;
	playAudio(4);
}

void PlayState::SpawnEnemy()
{
	int index = -1;
	for (int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemy[i] == NULL)
		{
			index = i;
			break;
		}
	}

	if (index < 0)
		return;

	bool overlap = false;

	srand((unsigned int)time(NULL));

	switch (rand() % 4)
	{
	case 0:
		enemy[index] = new FollowEnemy();
		break;
	case 1:
		enemy[index] = new BounceEnemy();
		break;
	case 2:
		enemy[index] = new ChargeEnemy();
		break;
	default:
		enemy[index] = new ShootEnemy();
		break;
	}

	do
	{
		overlap = false;
		enemy[index]->PosX = 12.0f + (rand() % 84);
		enemy[index]->PosY = 8.0f + (rand() % 28);

		for (unsigned int y = 0; y < enemy[index]->Height; y++)
		{
			for (unsigned int x = 0; x < enemy[index]->Width; x++)
			{
				short prevB = getPrevBColour((int)(enemy[index]->PosX + x), (int)(enemy[index]->PosY + y));
				short prevF = getPrevFColour((int)(enemy[index]->PosX + x), (int)(enemy[index]->PosY + y));
				short prevC = getPrevChar((int)(enemy[index]->PosX + x), (int)(enemy[index]->PosY + y));
				if (prevC != 0 && prevC != 219 && prevB != 0)
				{
					overlap = true;
				}
			}
		}
	} while (overlap);

	enemyCount++;
}

void PlayState::DisposeEnemy(int index)
{
	if (enemy[index] == NULL)
		return;

	enemy[index]->Destroy();
	delete enemy[index];
	enemy[index] = NULL;

	enemyCount--;
}

float PlayState::a(float d, float e)
{
	return sqrtf(powf(d - 64, 2.0f) + powf(e - 64, 2.0f)) / 20.0f;
}

void PlayState::Render()
{
	float c = 0, m = 127, z = 8.0f;

	for (int i = 0; i < m; i += 3)
	{
		for (int j = 0; j < m; j += 3)
		{
			float b = a((float)i, (float)j);
			float v = cosf(i / z) * cosf(j / z) * b + sinf(t) * b;
			v += z;

			switch ((int)v % 3)
			{
			case 0:
				fill(i, j, i + 2, j + 2, 176, COLOUR::WHITE);
				break;
			case 1:
				fill(i, j, i + 2, j + 2, 176, COLOUR::GREY);
				break;
			case 2:
				fill(i, j, i + 2, j + 2, 176, COLOUR::DARK_GREY);
				break;
			}
		}
	}

	if (playerDead)
	{
		srand((unsigned int)ball);

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
			
			drawChar((int)(x + ball->PosX), (int)(y + ball->PosY), c, COLOUR::RED);
		}
	}
	else
	{
		ball->Draw();
	}

	for (unsigned int i = 0; i < NUM_BULLETS; i++)
	{
		if (bulletActive[i])
			bullet[i]->Draw();
	}

	for (unsigned int i = 0; i < NUM_ENEMIES; i++)
	{
		if (enemy[i])
			enemy[i]->Draw();
	}

	tilemap->Draw();
	paddleLeft->Draw();
	paddleRight->Draw();

	gui->Draw();
}

void PlayState::Cleanup()
{
	ball->Destroy();
	delete ball;

	tilemap->Destroy();
	delete tilemap;

	for (unsigned int i = 0; i < NUM_BULLETS; i++)
	{
		bullet[i]->Destroy();
		delete bullet[i];
	}

	gui->Destroy();
	delete gui;
}