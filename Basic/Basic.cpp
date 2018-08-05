#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60
#define FONTWIDTH 8
#define FONTHEIGHT 8
#define FPS 60
#define GAME_NAME "Example Game"
//#define ENABLE_SOUND ///Optional define if a you want sound.
#include <game.h>

void gameloop(float dt) ///Game Loops are done in here. dt is the delta time between frames (1 / FPS).
{
	clearScreen();

	for (int i = 0; i < SCREEN_WIDTH / 2; i++)
	{
		float s = ((float)3.0f * i / SCREEN_WIDTH) * sinf(gameTime + (10.0f * i / SCREEN_WIDTH));

		int segmentHeight = 10;
		int mainChar = 178;

		if (i == 0)
		{
			segmentHeight = 6;
			mainChar = 177;
		}
		if (i == 1)
		{
			segmentHeight = 9;
		}

		if (i >(SCREEN_WIDTH / 2) - 5)
			segmentHeight = 9;
		if (i > (SCREEN_WIDTH / 2) - 4)
			segmentHeight = 8;
		if (i > (SCREEN_WIDTH / 2) - 3)
			segmentHeight = 6;
		if (i > (SCREEN_WIDTH / 2) - 2)
		{
			segmentHeight = 4;
			mainChar = 177;
		}
		if (i > (SCREEN_WIDTH / 2) - 1)
		{
			segmentHeight = 1;
			mainChar = 176;
		}

		drawLine(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 + i,
			SCREEN_HEIGHT / 2 - segmentHeight + (s * 2),
			SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 + i,
			SCREEN_HEIGHT / 2 + segmentHeight + (s * 2),
			mainChar, (i / 4) % 2 ? COLOUR::GREY : COLOUR::DARK_GREY);

		drawChar(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 + i,
			SCREEN_HEIGHT / 2 - segmentHeight + (s * 2) + 1,
			177, (i / 4) % 2 ? COLOUR::GREY : COLOUR::DARK_GREY);
		drawChar(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 + i,
			SCREEN_HEIGHT / 2 + segmentHeight + (s * 2) - 1,
			177, (i / 4) % 2 ? COLOUR::GREY : COLOUR::DARK_GREY);

		drawChar(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 + i,
			SCREEN_HEIGHT / 2 - segmentHeight + (s * 2),
			176, (i / 4) % 2 ? COLOUR::GREY : COLOUR::DARK_GREY);
		drawChar(SCREEN_WIDTH / 2 - SCREEN_WIDTH / 4 + i,
			SCREEN_HEIGHT / 2 + segmentHeight + (s * 2),
			176, (i / 4) % 2 ? COLOUR::GREY : COLOUR::DARK_GREY);
	}

	const char* name =
		" _  ___ _    ____                      _      " \
		"| |/ (_) |_ / ___|___  _ __  ___  ___ | | ___ " \
		"| ' /| | __| |   / _ \\| '_ \\/ __|/ _ \\| |/ _ \\" \
		"| . \\| | |_| |__| (_) | | | \\__ \\ (_) | |  __/" \
		"|_|\\_\\_|\\__|\\____\\___/|_| |_|___/\\___/|_|\\___|" \
		"                                              ";

	int height = 6;
	int width = strlen(name) / height;

	for (int i = 0; i < width * height; i++)
	{
		short colour = ((int)(i + gameTime * 10.0f) % 5) + 9;
		drawChar(SCREEN_WIDTH / 2 - width / 2 + (i % width), SCREEN_HEIGHT / 2 - height / 2 + (i / width), name[i], colour);
	}
}

int main()
{
	return run(gameloop); ///Initialises the console window for game use as well as run the game loop.
}