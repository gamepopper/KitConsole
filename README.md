# KitConsole
KitConsole is a tiny engine made for FloppyJam to create a game that could render to the Windows Command Prompt, as well as load and play audio files (.wav and .ogg).

It's written for use in C/C++ to be mostly header only.

## Usage

Here is the boilerplate/minimum code needed to run an application using KitConsole.

```
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60
#define FONTWIDTH 8
#define FONTHEIGHT 8
#define FPS 60
#define GAME_NAME "Example Game"
#define ENABLE_SOUND ///Optional define if a you want sound.
#include <game.h>

void gameloop(float dt) ///Game Loops are done in here, useful for real-time input and rendering behaviour. float dt is the delta time between frames (1 / FPS).
{
	
}

void handleEvent(Event &e) ///Actions are performed here if an event is to be processed, useful for event-based behaviour. Event &e is the current input to be processed. This function can be called more than once per frame.
{

}

int main()
{
	return run(gameloop, handleEvent); ///Initialises the console window for game use as well as run the game loop and event handling.
}
```

You need to pass in either a gameloop or handleEvent function in order to do any rendering or audio processing. You can pass in 0 to one of the two parameters if you do not want to use one.

As this is a header, you can access a lot of the games inner working, but here are the main properties and functions worth using in game.h:

```
bool windowActive = true; ///Will always remain true unless you want to exit the game safely.
bool windowFocus = true; ///Is true if the window is currently in focus. Game will not render new frames or play audio when out of focus.
CHAR_INFO *bufScreen; ///All the screen data for the scene about to be rendered can be found here.
CHAR_INFO *prevBufScreen; ///Screen data for the previously rendered frame.
bool keyState[256]; ///The current state for all keys (including special keys like directional keys)
bool prevKeyState[256]; ///The previous state of all keys.
COORD mousePos; ///The current mouse position.

void clearScreen(); ///Sets the entire screen to a plain black background.
void drawChar(int x, int y, short c, int color, int bgColor = 0); ///Draws a single character symbol to a certain position, as well as set it's colour and the background colour.
void fill(int x1, int y1, int x2, int y2, short c, int col, int bgColor = 0); ///Fills an entire area with a character of a specified colour and background colour.
void drawLine(int x1, int y1, int x2, int y2, short c, int col, int bgColor = 0); ///Draws a line of a character of a specified colour and background colour.
void drawCircle(int xc, int yc, int r, short c, int col, int bgColor = 0); ///Draws a lined circle of a character of a specified colour and background colour.
void drawFillCircle(int xc, int yc, int r, short c, int col, int bgColor = 0); ///Draws a filled circle of a character of a specified colour and background colour.

short getPrevChar(int x, int y); Gets the rendered character symbol of the last frame at a specified coordinate.
short getCurrentChar(int x, int y); Gets the rendered character symbol of the current frame at a specified coordinate.
short getPrevFColour(int x, int y); Gets the character symbol's colour of the last frame at a specified coordinate.
short getCurrentFColour(int x, int y); Gets the character symbol's colour of the current frame at a specified coordinate.
short getPrevBColour(int x, int y); Gets the background colour of the last frame at a specified coordinate.
short getCurrentBColour(int x, int y); Gets the background colour of the current frame at a specified coordinate.
```

If you want audio, be sure to include a preprocessor definition called ENABLE_SOUND.

```
void loadAudio(const char* name, unsigned int id); ///Loads a WAV or OGG file to a specific ID.
void playAudio(int id, bool loop = false, float volume = 1.0f, float pitch = 1.0f, float pan = 0.5f, float delay = 0.0f); ///Plays audio file at specified ID, setting to loop, volume, pitch, pan and with delay.
void stopAudio(int id); ///Stops an audio file at a specified ID.
void freeAudio(int id); //Frees an audio file at a specified ID, making the ID free to use a different audio file.
```

If you want to use Event-based Input, here is the important stuff.

```
enum EventType
{
	Null,
	KeyPressed,
	KeyReleased,
	TextEntered,
	MousePressed,
	MouseReleased,
	MouseMoved,
	MouseWheel,
	NumEvents
};

struct Event
{
	Event() :
		type(EventType::Null),
		key(-1),
		button(255),
		text(0),
		shift(false),
		ctrl(false),
		alt(false),
		doublePress(false)
	{

	}

	short type;
	//Key Events
	short key;
	char text;
	bool shift;
	bool ctrl;
	bool alt;

	//Mouse Events
	char button;
	COORD vel;
	bool doublePress;
};
```

## Future Features
* Linux Support
* Gamepad Support
