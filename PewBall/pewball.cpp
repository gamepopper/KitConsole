#define SCREEN_WIDTH 108
#define SCREEN_HEIGHT 60
#define FONTWIDTH 8
#define FONTHEIGHT 8
#define FPS 60
#define GAME_NAME "PEWBALL"
#define ENABLE_SOUND

#include <game.h>
#include "PlayState.h"
#include "TitleState.h"

StateObject* state = NULL;

void gameloop(float dt)
{
	if (currentState == GS_INIT)
	{
		currentState = GS_GOTITLE;

		loadAudio("Audio/bounce.ogg", 0);
		loadAudio("Audio/enemyDeath.ogg", 1);
		loadAudio("Audio/enemyShoot.ogg", 2);
		loadAudio("Audio/launch.ogg", 3);
		loadAudio("Audio/playerDeath.ogg", 4);
		loadAudio("Audio/playerShoot.ogg", 5);
		loadAudio("Audio/select.ogg", 6);
		loadAudio("Audio/ambient.ogg", 7);
		playAudio(7, true, 0.8f);
	}

	if (currentState == GS_GOPLAY)
	{
		nextState = GS_PLAY;
	}
	else if (currentState == GS_GOTITLE)
	{
		nextState = GS_TITLE;
	}

	clearScreen();
	if (state != NULL)
	{
		state->Update(dt);
		state->Render();
	}

	if (nextState != currentState)
	{
		currentState = nextState;

		if (state != NULL)
		{
			state->Cleanup();
			delete state;
			state = NULL;
		}

		switch (currentState)
		{
		case GS_TITLE:
			state = new TitleState();
			break;
		case GS_PLAY:
			state = new PlayState();
			break;
		default:
			break;
		}

		if (state != NULL)
		{
			state->Initialise();
		}
	}

	if ((keyState[VK_ESCAPE] && !prevKeyState[VK_ESCAPE]) || currentState == GS_SHUTDOWN)
		windowActive = false;
}

int main()
{
	printf("This game was made for FloppyJam.");
	Sleep(2000);
	printf("\nThe challenge is to make a game that was small enough to fit on a floppy disk.");
	Sleep(2000);
	printf("\nAs such, I've made a game built into this command prompt. Enjoy!");
	Sleep(2000);
	printf("\n-- Gamepopper");
	Sleep(5000);

	return run(gameloop);
}