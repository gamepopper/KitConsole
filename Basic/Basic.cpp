#define SCREEN_WIDTH 120
#define SCREEN_HEIGHT 64
#define FONTWIDTH 8
#define FONTHEIGHT 8
#define FPS 60
#define GAME_NAME "Example Game"
//#define ENABLE_SOUND ///Optional define if a you want sound.
#include <game.h>

int f(int x, int y, float t)
{
	return static_cast<int>(sin(cos((x + 64) / t / 12)) + cos(sin((y + 64) / (t - 1) / 20)) + t) % 7;
}

void gameloop(float dt) ///Game Loops are done in here. dt is the delta time between frames (1 / FPS).
{
	clearScreen();	

	//Spiral Demo by Jose Guerra: https://twitter.com/guerragames/status/1015430558699216896
	float z = gameTime / 4.0f;
	for (float a = 1.f; a > 0.05f; a -= 0.005f)
	{
		for (int i = 1; i < 14; ++i)
		{
			float r = a * 60 + max(0, i * 3 * sinf((a + z) * (2 * PI)));
			int x = static_cast<int>(r * cosf((a * 4 - z) * (2 * PI)));
			int y = static_cast<int>(r * sinf((a * 4 - z) * (2 * PI)));
			drawFillCircle((SCREEN_WIDTH / 2) + x, (SCREEN_HEIGHT / 2) + y, 3, 219, i);
		}
	}

	//Console name in rainbow colours
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