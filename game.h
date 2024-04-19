#pragma once

#ifdef WIN32
#define KITCONSOLE_WINDOWS
#endif

#ifdef KITCONSOLE_WINDOWS
#include <Windows.h>
#else
typedef struct _CHAR_INFO {
	union {
		wchar_t UnicodeChar;
		char   AsciiChar;
	} Char;
	unsigned short Attributes;
} CHAR_INFO, *PCHAR_INFO;

typedef struct _COORD
{
	short X;
	short Y;
} COORD;

typedef struct _SMALL_RECT {
	short Left;
	short Top;
	short Right;
	short Bottom;
} SMALL_RECT, *PSMALL_RECT;
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "math.h"

#ifdef ENABLE_SOUND
#include "sound.h"
#endif

#ifndef min
#define min(a,b) (((a)<(b))?(a):(b))
#endif // min

#define UNICODE
#define PI 3.14159265358979323846f

float gameTime = 0;
float updateTime = 0;

enum COLOUR
{
	BLACK = 0x0000,
	DARK_BLUE = 0x0001,
	DARK_GREEN = 0x0002,
	DARK_CYAN = 0x0003,
	DARK_RED = 0x0004,
	DARK_MAGENTA = 0x0005,
	DARK_YELLOW = 0x0006,
	GREY = 0x0007, // Thanks MS :-/
	DARK_GREY = 0x0008,
	BLUE = 0x0009,
	GREEN = 0x000A,
	CYAN = 0x000B,
	RED = 0x000C,
	MAGENTA = 0x000D,
	YELLOW = 0x000E,
	WHITE = 0x000F,
};

bool windowActive = true;
bool windowFocus = true;
CHAR_INFO *bufScreen;
CHAR_INFO *prevBufScreen;
bool keyState[256];
bool prevKeyState[256];
COORD mousePos;

double PCFreq = 0.0;
__int64 CounterStart = 0;

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
		button(-1),
		text(0),
		shift(false),
		ctrl(false),
		alt(false),
		doublePress(false),
		vel()
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

bool StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return false;

	PCFreq = double(li.QuadPart);

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	return true;
}

float GetCounterNanoseconds()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return (float)((li.QuadPart - CounterStart) / PCFreq);
}

float GetCounterMilliseconds()
{
	return GetCounterNanoseconds() * 1000.f;
}

SYSTEMTIME stime;
LARGE_INTEGER sys_freq;

bool ctrl_handler(unsigned long event)
{
	if (event == CTRL_CLOSE_EVENT) {
		windowActive = false;
		return TRUE;
	}
	return FALSE;
}

short getPrevChar(int x, int y)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return 0;

	return prevBufScreen[y * SCREEN_WIDTH + x].Char.UnicodeChar;
}

short getCurrentChar(int x, int y)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return 0;

	return bufScreen[y * SCREEN_WIDTH + x].Char.UnicodeChar;
}

short getPrevFColour(int x, int y)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return 0;

	return prevBufScreen[y * SCREEN_WIDTH + x].Attributes % 16;
}

short getCurrentFColour(int x, int y)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return 0;

	return bufScreen[y * SCREEN_WIDTH + x].Attributes % 16;
}

short getPrevBColour(int x, int y)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return 0;

	return prevBufScreen[y * SCREEN_WIDTH + x].Attributes / 16;
}

short getCurrentBColour(int x, int y)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return 0;

	return bufScreen[y * SCREEN_WIDTH + x].Attributes / 16;
}

int initWindow()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return -1;

	SMALL_RECT rectWindow = { 0, 0, 79, 59 };
	SetConsoleWindowInfo(hConsole, TRUE, &rectWindow);

	COORD coord = { (short)SCREEN_WIDTH, (short)SCREEN_HEIGHT };
	if (!SetConsoleScreenBufferSize(hConsole, coord))
		return -2;

	if (!SetConsoleActiveScreenBuffer(hConsole))
		return -3;

	if (SCREEN_HEIGHT - 1 > csbi.dwMaximumWindowSize.Y)
		return -4;
	if (SCREEN_WIDTH - 1 > csbi.dwMaximumWindowSize.X)
		return -5;

	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		rectWindow = { 0, 0, (short)SCREEN_WIDTH - 1, (short)SCREEN_HEIGHT - 1 };
		if (!SetConsoleWindowInfo(hConsole, TRUE, &rectWindow))
			return -6;

		if (!SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT))
			return -7;

		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = FONTWIDTH;
		cfi.dwFontSize.Y = FONTHEIGHT;
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_BOLD;

		wcscpy_s(cfi.FaceName, L"Consolas");
		if (!SetCurrentConsoleFontEx(hConsole, FALSE, &cfi))
			return -8;

		CONSOLE_CURSOR_INFO structCursorInfo;
		GetConsoleCursorInfo(hConsole, &structCursorInfo); // Get current cursor size
		structCursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(hConsole, &structCursorInfo);

		SetConsoleCtrlHandler((PHANDLER_ROUTINE)(ctrl_handler), TRUE);

		QueryPerformanceFrequency(&sys_freq);
		StartCounter();
	}

	printf("\nKitConsole Window Successful");
	return 0;
}

void handleInput(void(*handleEvent)(Event &e))
{
	for (unsigned int i = 0; i < 256; i++)
	{
		keyState[i] = GetAsyncKeyState(i) & 0x8000;
	}

	HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	INPUT_RECORD inBuf[32];
	DWORD events = 0;
	GetNumberOfConsoleInputEvents(hConsoleIn, &events);
	if (events > 0)
		ReadConsoleInput(hConsoleIn, inBuf, events, &events);

	for (DWORD i = 0; i < events; i++)
	{
		switch (inBuf[i].EventType)
		{
		case FOCUS_EVENT:
		{
			windowFocus = inBuf[i].Event.FocusEvent.bSetFocus;
		}
		break;

		case MOUSE_EVENT:
		{
			Event e;

			switch (inBuf[i].Event.MouseEvent.dwEventFlags)
			{
			case MOUSE_MOVED:
			{
				if (mousePos.X != inBuf[i].Event.MouseEvent.dwMousePosition.X ||
					mousePos.Y != inBuf[i].Event.MouseEvent.dwMousePosition.Y)
				{
					e.type = EventType::MouseMoved;
					e.vel.X = inBuf[i].Event.MouseEvent.dwMousePosition.X - mousePos.X;
					e.vel.Y = inBuf[i].Event.MouseEvent.dwMousePosition.Y - mousePos.Y;
				}

				mousePos.X = inBuf[i].Event.MouseEvent.dwMousePosition.X;
				mousePos.Y = inBuf[i].Event.MouseEvent.dwMousePosition.Y;
			}
			break;

			case DOUBLE_CLICK:
			{
				e.doublePress = true;
				e.type = EventType::MousePressed;
			}
			break;

			case MOUSE_WHEELED:
			{
				e.type = EventType::MouseWheel;
			}
			break;

			default:
				break;
			}

			if (handleEvent && e.type != EventType::Null)
				handleEvent(e);
		}
		break;

		case KEY_EVENT:
		{
			Event e;
			if (inBuf[i].Event.KeyEvent.bKeyDown && !prevKeyState[inBuf[i].Event.KeyEvent.wVirtualKeyCode])
			{
				e.type = EventType::TextEntered;
				e.text = inBuf[i].Event.KeyEvent.uChar.AsciiChar;

				if (handleEvent && e.text)
					handleEvent(e);

				e.type = EventType::KeyPressed;
				e.text = 0;
			}
			else if (!keyState[inBuf[i].Event.KeyEvent.wVirtualKeyCode])
				e.type = EventType::KeyReleased;

			e.key = inBuf[i].Event.KeyEvent.wVirtualKeyCode;
			switch (e.key)
			{
			case VK_SHIFT:
			case VK_CONTROL:
			case VK_MENU:
				break;
			default:
				e.alt = inBuf[i].Event.KeyEvent.dwControlKeyState & (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED);
				e.ctrl = inBuf[i].Event.KeyEvent.dwControlKeyState & (RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED);
				e.shift = inBuf[i].Event.KeyEvent.dwControlKeyState & (SHIFT_PRESSED);
			
				if (handleEvent && e.type != EventType::Null)
					handleEvent(e);
				break;
			}
		}
		break;
		default:
			break;
			// We don't care just at the moment
		}
	}

	for (int i = 1; i < 7; i++)
	{
		if (keyState[i] && !prevKeyState[i])
		{
			Event e;
			e.type = EventType::MousePressed;
			e.button = i - 1;

				if (handleEvent)
					handleEvent(e);
		}
		else if (!keyState[i] && prevKeyState[i])
		{
			Event e;
			e.type = EventType::MouseReleased;
			e.button = i - 1;

			if (handleEvent)
				handleEvent(e);
		}
	}
}

int run(void(*gameloop)(float dt) = 0, void(*handleEvent)(Event &e) = 0)
{
	int result = 0;
	if ((result = initWindow()) != 0)
	{
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		FILE* f;
		if (fopen_s(&f, "error.txt", "w") != 0)
		{
			printf("Error opening file!\n");
			exit(1);
		}

		const char* point[] = {
			"",
			"GetConsoleScreenBufferInfo(hConsole, &csbi",
			"SetConsoleScreenBufferSize(hConsole, coord)",
			"SetConsoleActiveScreenBuffer(hConsole)",
			"if (SCREEN_HEIGHT > csbi.dwMaximumWindowSize.Y)",
			"if (SCREEN_WIDTH > csbi.dwMaximumWindowSize.X)",
			"SetConsoleWindowInfo(hConsole, TRUE, &rectWindow)",
			"SetConsoleMode(hConsoleIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)",
			"SetCurrentConsoleFontEx(hConsole, false, &cfi)",
		};

		fprintf(f, "Game %s crashed at \"%s\" with the system error (%d): %s", GAME_NAME, point[-result], dw, (LPTSTR)lpMsgBuf);

		fclose(f);
		return result;
	}

#ifdef ENABLE_SOUND
	if ((result = initAudio()) != 0)
	{
		FILE* f;
		if (fopen_s(&f, "error.txt", "w") != 0)
		{
			printf("Error opening file!\n");
			exit(1);
		}

		fprintf(f, "Game %s crashed while initialising the audio. Context could not be found.", GAME_NAME);

		fclose(f);

		return result;
	}

	printf("\nKitConsole Audio Successful");
#endif

	SMALL_RECT rectWindow = { 0, 0, (short)SCREEN_WIDTH - 1, (short)SCREEN_HEIGHT - 1 };

	bufScreen = new CHAR_INFO[SCREEN_WIDTH*SCREEN_HEIGHT];
	prevBufScreen = new CHAR_INFO[SCREEN_WIDTH*SCREEN_HEIGHT];
	memset(bufScreen, 0, sizeof(CHAR_INFO) * SCREEN_WIDTH * SCREEN_HEIGHT);

	float tp1 = GetCounterMilliseconds() / 1000.0f, tp2 = tp1;

#ifdef KITCONSOLE_WINDOWS
	SetConsoleTitle(GAME_NAME);
#endif

	while (windowActive)
	{
		float dt = 1.0f / FPS;

		tp2 = GetCounterMilliseconds() / 1000.0f;
		float fElapsedTime = tp2 - tp1;
		tp1 = tp2;
		gameTime += fElapsedTime;
		updateTime += fElapsedTime;

		if (updateTime > 1.0f / FPS)
		{
			handleInput(handleEvent);
			
			if (windowFocus)
			{
				if (gameloop)
					gameloop(1.0f / FPS);
			}

#ifdef ENABLE_SOUND
			updateAudio();
#endif
			updateTime -= 1.0f / FPS;

			for (unsigned int i = 0; i < 256; i++)
			{
				prevKeyState[i] = keyState[i];
			}

			if (windowFocus)
			{
			#ifdef KITCONSOLE_WINDOWS
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				WriteConsoleOutput(hConsole, bufScreen, { (short)SCREEN_WIDTH, (short)SCREEN_HEIGHT }, { 0,0 }, &rectWindow);
			#else
				printf("%s", "\033[2J\033[3J");
				printf("%s", "\033]0;");

				const char* foreColours[] =
				{
					"\033[22;30m",
					"\033[01;34m",
					"\033[01;32m",
					"\033[01;36m",
					"\033[01;31m",
					"\033[01;35m",
					"\033[01;33m",
					"\033[22;37m",
					"\033[01;30m",
					"\033[22;34m",
					"\033[22;32m",
					"\033[22;36m",
					"\033[22;31m",
					"\033[22;35m",
					"\033[22;33m",
					"\033[01;37m",
				};

				const char* backColours[] = 
				{
					"\033[40m",
					"\033[44m",
					"\033[42m",
					"\033[46m",
					"\033[41m",
					"\033[45m",
					"\033[43m",
					"\033[47m",
					"\033[40m",
					"\033[44m",
					"\033[42m",
					"\033[46m",
					"\033[41m",
					"\033[45m",
					"\033[43m",
					"\033[47m",
				};

				for (int y = 0; y < SCREEN_HEIGHT; y++)
				{
					for (int x = 0; x < SCREEN_WIDTH; x++)
					{
						printf("%c", (char)getCurrentChar(x, y));
						/*printf("%s", foreColours[getCurrentFColour(x, y)]);
						printf("%s", backColours[getCurrentBColour(x, y)]);*/
					}

					printf("\n");
				}
			#endif
			}
		}

		memcpy(prevBufScreen, bufScreen, sizeof(CHAR_INFO) * SCREEN_WIDTH * SCREEN_HEIGHT);
	}

	delete[] bufScreen;
	delete[] prevBufScreen;

#ifdef ENABLE_SOUND
	shutdownAudio();
#endif

	return result;
}

void drawChar(int x, int y, short c, int color, int bgColor = 0)
{
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return;

	bufScreen[y * SCREEN_WIDTH + x].Char.UnicodeChar = (short)c;
	bufScreen[y * SCREEN_WIDTH + x].Attributes = color | (short)(bgColor * 16);
}

void clip(int &x, int &y)
{
	if (x < 0) x = 0;
	if (x >= SCREEN_WIDTH) x = SCREEN_WIDTH;
	if (y < 0) y = 0;
	if (y >= SCREEN_HEIGHT) y = SCREEN_HEIGHT;
}

void fill(int x1, int y1, int x2, int y2, short c, int col, int bgColor = 0)
{
	clip(x1, y1);
	clip(x2, y2);
	for (int x = x1; x < x2; x++)
		for (int y = y1; y < y2; y++)
			drawChar(x, y, c, col, bgColor);
}

void drawLine(int x1, int y1, int x2, int y2, short c, int col, int bgColor = 0)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1; dy = y2 - y1;
	dx1 = abs(dx); dy1 = abs(dy);
	px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1; y = y1; xe = x2;
		}
		else
		{
			x = x2; y = y2; xe = x1;
		}

		drawChar(x, y, c, col, bgColor);

		for (i = 0; x<xe; i++)
		{
			x = x + 1;
			if (px<0)
				px = px + 2 * dy1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) y = y + 1; else y = y - 1;
				px = px + 2 * (dy1 - dx1);
			}
			drawChar(x, y, c, col, bgColor);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1; y = y1; ye = y2;
		}
		else
		{
			x = x2; y = y2; ye = y1;
		}

		drawChar(x, y, c, col, bgColor);

		for (i = 0; y<ye; i++)
		{
			y = y + 1;
			if (py <= 0)
				py = py + 2 * dx1;
			else
			{
				if ((dx<0 && dy<0) || (dx>0 && dy>0)) x = x + 1; else x = x - 1;
				py = py + 2 * (dx1 - dy1);
			}
			drawChar(x, y, c, col, bgColor);
		}
	}
}


void drawCircle(int xc, int yc, int r, short c, int col, int bgColor = 0)
{
	int x = 0;
	int y = r;
	int p = 3 - 2 * r;
	if (!r) return;

	while (y >= x) // only formulate 1/8 of circle
	{
		drawChar(xc - x, yc - y, c, col);//upper left left
		drawChar(xc - y, yc - x, c, col);//upper upper left
		drawChar(xc + y, yc - x, c, col);//upper upper right
		drawChar(xc + x, yc - y, c, col);//upper right right
		drawChar(xc - x, yc + y, c, col);//lower left left
		drawChar(xc - y, yc + x, c, col);//lower lower left
		drawChar(xc + y, yc + x, c, col);//lower lower right
		drawChar(xc + x, yc + y, c, col);//lower right right
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void drawFillCircle(int xc, int yc, int r, short c, int col, int bgColor = 0)
{
	// Taken from wikipedia
	int x = 0;
	int y = r;
	int p = 3 - 2 * r;
	if (!r) return;

	auto drawline = [&](int sx, int ex, int ny)
	{
		for (int i = sx; i <= ex; i++)
			drawChar(i, ny, c, col, bgColor);
	};

	while (y >= x)
	{
		// Modified to draw scan-lines instead of edges
		drawline(xc - x, xc + x, yc - y);
		drawline(xc - y, xc + y, yc - x);
		drawline(xc - x, xc + x, yc + y);
		drawline(xc - y, xc + y, yc + x);
		if (p < 0) p += 4 * x++ + 6;
		else p += 4 * (x++ - y--) + 10;
	}
}

void clearScreen()
{
	memset(bufScreen, 0, sizeof(CHAR_INFO) * SCREEN_WIDTH * SCREEN_HEIGHT);
}