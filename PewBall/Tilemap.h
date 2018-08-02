#pragma once
#include "gameobject.h"

struct Paddle : public GameObject
{
	bool flip = false;
	float state = 0.0f;
	short key = 0;
	const short size = 8;
	char* subMap;

	Paddle() : GameObject()
	{

	}

	virtual void Update(float dt)
	{
		GameObject::Update(dt);

		if (key > 0 && keyState[key])
		{
			if (state < 4)
				state += dt * 40;
			else
				state = 4;
		}
		else
		{
			if (state > 0)
				state -= dt * 30;
			else
				state = 0;
		}


		switch ((short)state)
		{
		case 0:
			subMap =
				"00000000" \
				"00000000" \
				"00000000" \
				"01100000" \
				"11111000" \
				"11111110" \
				"11111111" \
				"01111110";
			break;
		case 1:
			subMap =
				"00000000" \
				"00000000" \
				"00000000" \
				"01111110" \
				"11111111" \
				"11111110" \
				"11111000" \
				"01110000";
			break;
		case 2:
			subMap =
				"00000000" \
				"00000000" \
				"00000011" \
				"01111111" \
				"11111110" \
				"11111100" \
				"11110000" \
				"01100000";
			break;
		case 3:
			subMap =
				"00000000" \
				"00000011" \
				"00011111" \
				"01111110" \
				"11111100" \
				"11110000" \
				"11110000" \
				"01100000";
			break;
		default:
			subMap =
				"00000001" \
				"00000111" \
				"00011110" \
				"01111110" \
				"11111100" \
				"11111000" \
				"11110000";
			break;
		}
	}

	bool CollisionTest(Sprite* spr, bool inputHandling = false)
	{
		if (spr->PosX + spr->Width >= PosX &&
			spr->PosX < PosX + 8 &&
			spr->PosY + spr->Height >= PosY &&
			spr->PosY < PosY + 8)
		{
			short overlapX = 0;
			short overlapY = 0;
			short diffX = (short)spr->LastX - (short)spr->PosX;
			short diffY = (short)spr->LastY - (short)spr->PosY;

			for (short y = 0; y < (short)spr->Height; y++)
			{
				for (short x = 0; x < (short)spr->Width; x++)
				{
					if (spr->PosX + x - PosX < 0 || spr->PosX + x - PosX >= 8 ||
						spr->PosY + y - PosY < 0 || spr->PosY + y - PosY >= 16)
						continue;

					if (subMap[((int)(spr->PosY + y - PosY) * 8) + (int)(spr->PosX + (flip ? 7 - x : x) - PosX)] == '1')
					{
						if (diffX > 0)
							overlapX = -1;
						else if (diffX < 0)
							overlapX = 1;

						if (diffY > 0)
							overlapY = -1;
						else if (diffY < 0)
							overlapY = 1;

						short response = (short)state;
						if (response >= 0)
						{
							response = response < 5 ? response : 4;
							float normal[][2] = {
								{ 0.38f, -0.92f }, //14
								{ 0.00f, -1.00f }, //13
								{ -0.38f, -0.92f }, //12
								{ -0.70f, -0.70f }, //11
								{ -0.92f, -0.38f }, //10
							};

							if (flip)
							{
								normal[response][0] *= -1;
							}

							float VelX = spr->VelX - 2 * Dot(spr->VelX, spr->VelY, normal[response][0], normal[response][1]) * normal[response][0] * spr->Bounce;
							float VelY = normal[response][1] * spr->Bounce;

							if (inputHandling == true && keyState[key] && state < 4)
							{
								VelY *= spr->MaxY * 0.8f;
							}
							else
							{
								VelY = spr->VelY - 2 * Dot(spr->VelX, spr->VelY, normal[response][0], normal[response][1]) * normal[response][1] * spr->Bounce;
								overlapY = 2;
							}

							playAudio(0);

							spr->VelX = VelX;
							spr->VelY = VelY;
						}

						spr->PosX -= overlapX;
						spr->PosY -= overlapY;

						return true;
					}
				}
			}
		}

		return false;
	}

	float Dot(float x1, float y1, float x2, float y2)
	{
		return x1 * x2 + y1 * y2;
	}

	virtual void Draw()
	{
		for (short i = 0; i < 64; i++)
		{
			if (subMap[i] != '0')
			{
				if (flip)
					drawChar((int)PosX + 8 - (i % 8), (int)PosY + (i / 8), 219, 15);
				else
					drawChar((int)PosX + (i % 8), (int)PosY + (i / 8), 219, 15);
			}
		}
	}

	virtual void Destroy() {}
};

struct Tilemap : public GameObject
{
	short* map;
	static const short mapWidth = 27;
	static const short mapHeight = 15;
	short colour = 0;

	Tilemap() : GameObject()
	{
		map = new short[mapWidth * mapHeight]{
			-1, -1, -1, -1, -1,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5, -1, -1, -1, -1, -1,
			-1, -1, -1,  4,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6,  6, -1, -1, -1,
			-1, -1,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  7, -1, -1,
			-1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, -1,
			-1,  2,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  8, -1,
			 1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,
			-1, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,
			-1, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  9,
			-1, -1, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  0,  9,
			-1, -1, -1, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11,  1,  0,  9,
			-1, -1, -1, -1, 15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 11, -1,  1,  0,  9,
			-1, -1, -1, -1, -1, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, -1, -1,  1,  0,  9,
			-1, -1, -1, -1, -1, -1, -1, 14, 14,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 12, -1, -1, -1, -1,  1,  0,  9,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, 14, 14,  0,  0,  0,  0,  0, 12, 12, -1, -1, -1, -1, -1, -1,  1,  0,  9,
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  1,  0,  0,  0,  0,  0,  9, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1,
		};

		colour = 1 + (rand() % 6);
	}

	bool CollisionTest(Sprite* spr)
	{
		short overlapX = 0;
		short overlapY = 0;
		short diffX = (short)spr->LastX - (short)spr->PosX;
		short diffY = (short)spr->LastY - (short)spr->PosY;

		for (short y = 0; y < (short)spr->Height; y++)
		{
			for (short x = 0; x < (short)spr->Width; x++)
			{
				if (getPrevBColour((int)spr->PosX + x, (int)spr->PosY + y) != 0)
				{
					if (diffX > 0)
						overlapX = -1;
					else if (diffX < 0)
						overlapX = 1;

					if (diffY > 0)
						overlapY = -1;
					else if (diffY < 0)
						overlapY = 1;

					short response = map[((short)(spr->PosY + y) / 4) * mapWidth + ((short)(spr->PosX + x) / 4)] - 1;
					if (response >= 0)
					{
						float normal[][2] = {
							{  1.00f,  0.00f }, //01
							{  0.92f,  0.38f }, //02
							{  0.70f,  0.70f }, //03
							{  0.38f,  0.92f }, //04
							{  0.00f,  1.00f }, //05
							{ -0.38f,  0.92f }, //06
							{ -0.70f,  0.70f }, //07
							{ -0.92f,  0.38f }, //08
							{ -1.00f,  0.00f }, //09
							{ -0.92f, -0.38f }, //10
							{ -0.70f, -0.70f }, //11
							{ -0.38f, -0.92f }, //12
							{  0.00f, -1.00f }, //13
							{  0.38f, -0.92f }, //14
							{  0.70f, -0.70f }, //15
							{  0.92f, -0.38f }, //16
						};

						float vector[2];
						vector[0] = spr->VelX - 2 * Dot(spr->VelX, spr->VelY, normal[response][0], normal[response][1]) * normal[response][0];
						vector[1] = spr->VelY - 2 * Dot(spr->VelX, spr->VelY, normal[response][0], normal[response][1]) * normal[response][1];

						spr->VelX = vector[0] * spr->Bounce;
						spr->VelY = vector[1] * spr->Bounce;
					}

					spr->PosX -= overlapX;
					spr->PosY -= overlapY;
					playAudio(0);

					return true;
				}
			}
		}

		return false;
	}

	float Dot(float x1, float y1, float x2, float y2)
	{
		return x1 * x2 + y1 * y2;
	}

	virtual void Draw()
	{
		char* subMap;
		for (unsigned short y = 0; y < mapHeight; y++)
		{
			for (unsigned short x = 0; x < mapWidth; x++)
			{

				switch (map[(y * mapWidth) + x])
				{
				case 0:
					subMap =
						"0000" \
						"0000" \
						"0000" \
						"0000";
					break;
				case 1:
					subMap =
						"1112" \
						"1112" \
						"1112" \
						"1112";
					break;
				case 2:
					if (map[((y + 1) * mapWidth) + x] == 2)
					{
						subMap =
							"1112" \
							"1112" \
							"1120" \
							"1120";
					}
					else
					{
						subMap =
							"1200" \
							"1200" \
							"2000" \
							"2000";
					}
					break;
				case 3:
					subMap =
						"1112" \
						"1120" \
						"1200" \
						"2000";
					break;
				case 4:
					if (map[(y * mapWidth) + x + 1] == 4)
					{
						subMap =
							"1111" \
							"1111" \
							"1122" \
							"2200";
					}
					else
					{
						subMap =
							"1122" \
							"2200" \
							"0000" \
							"0000";
					}
					break;
				case 5:
					subMap =
						"1111" \
						"1111" \
						"1111" \
						"2222";
					break;
				case 6:
					if (map[(y * mapWidth) + x - 1] == 6)
					{
						subMap =
							"1111" \
							"1111" \
							"2211" \
							"0022";
					}
					else
					{
						subMap =
							"2211" \
							"0022" \
							"0000" \
							"0000";
					}
					break;
				case 7:
					subMap =
						"2111" \
						"0211" \
						"0021" \
						"0002";
					break;
				case 8:
					if (map[((y + 1) * mapWidth) + x] == 8)
					{
						subMap =
							"2111" \
							"2111" \
							"0211" \
							"0211";
					}
					else
					{
						subMap =
							"0021" \
							"0021" \
							"0002" \
							"0002";
					}
					break;
				case 9:
					subMap =
						"2111" \
						"2111" \
						"2111" \
						"2111";
					break;
				case 10:
					if (map[((y - 1) * mapWidth) + x] == 10)
					{
						subMap =
							"0211" \
							"0211" \
							"2111" \
							"2111";
					}
					else
					{
						subMap =
							"0002" \
							"0002" \
							"0021" \
							"0021";
					}
					break;
				case 11:
					subMap =
						"0002" \
						"0021" \
						"0211" \
						"2111";
					break;
				case 12:
					if (map[(y * mapWidth) + x - 1] == 12)
					{
						subMap =
							"0022" \
							"2211" \
							"1111" \
							"1111";
					}
					else
					{
						subMap =
							"0000" \
							"0000" \
							"0022" \
							"2211";
					}
					break;
				case 13:
					subMap =
						"2222" \
						"1111" \
						"1111" \
						"1111";
					break;
				case 14:
					if (map[(y * mapWidth) + x + 1] == 14)
					{
						subMap =
							"2200" \
							"1122" \
							"1111" \
							"1111";
					}
					else
					{
						subMap =
							"0000" \
							"0000" \
							"2200" \
							"1122";
					}
					break;
				case 15:
					subMap =
						"2000" \
						"1200" \
						"1120" \
						"1112";
					break;
				case 16:
					if (map[((y - 1) * mapWidth) + x] == 16)
					{
						subMap =
							"1120" \
							"1120" \
							"1112" \
							"1112";
					}
					else
					{
						subMap =
							"2000" \
							"2000" \
							"1200" \
							"1200";
					}
					break;
				default:
					subMap =
						"1111" \
						"1111" \
						"1111" \
						"1111";
				}

				for (short i = 0; i < 16; i++)
				{
					if (subMap[i] != '0')
					{
						char c = 0;

						if (getPrevBColour((x * 4) + (i % 4) - 1, (y * 4) + (i / 4)) == colour ||
							getPrevBColour((x * 4) + (i % 4) + 1, (y * 4) + (i / 4)) == colour ||
							getPrevBColour((x * 4) + (i % 4), (y * 4) + (i / 4) - 1) == colour ||
							getPrevBColour((x * 4) + (i % 4), (y * 4) + (i / 4) + 1) == colour)
						{
							c = (char)178;
						}

						if (getPrevFColour((x * 4) + (i % 4) - 1, (y * 4) + (i / 4)) == colour + 8 ||
							getPrevFColour((x * 4) + (i % 4) + 1, (y * 4) + (i / 4)) == colour + 8 ||
							getPrevFColour((x * 4) + (i % 4), (y * 4) + (i / 4) - 1) == colour + 8 ||
							getPrevFColour((x * 4) + (i % 4), (y * 4) + (i / 4) + 1) == colour + 8)
						{
							c = (char)176;
						}

						drawChar((x * 4) + (i % 4), (y * 4) + (i / 4),  c,  c != 0 ? colour + 8 : COLOUR::BLACK, subMap[i] == '2' ? colour : COLOUR::BLACK);
					}
				}
			}
		}
	}

	virtual void Destroy()
	{
		delete[] map;
	}
};