#pragma once
#include "state.h"
#include <game.h>

float easeOut(float t, float b, float c, float d) {
	return c * ((t = t / d - 1)*t*t + 1) + b;
}

struct TitleState : public StateObject
{
	float t = 0.0f;
	virtual void Initialise() 
	{
		playAudio(3, false, 0.5f, 0.2f, 0.5f);
	}
	virtual void Update(float dt)
	{
		t += dt;

		if (t > 5.0f)
		{
			if (keyState[0x59])
			{
				nextState = GS_GOPLAY;
				playAudio(6);
			}

			if (keyState[0x4E])
			{
				nextState = GS_SHUTDOWN;
			}
		}
	}
	virtual void Render();
	virtual void Cleanup()
	{

	}

	void DrawMessage(const char* message, int x, int y, short colour, float t);
};

void TitleState::Render()
{
	srand((unsigned int)this);

	for (int i = 0; i < 256; i++)
	{
		float x = (rand() % 16) - 8.0f;
		float y = (rand() % 16) - 8.0f;
		float offset = (rand() % 32) - 16.0f;
		float length = sqrtf((x*x) + (y*y));

		x /= length;
		y /= length;

		float distance = 16.f + fmodf(powf((t * 20.0f) + offset, 1.1f), SCREEN_WIDTH / 2.0f);

		x *= distance;
		y *= distance;

		short c = '*';
		if (distance > SCREEN_WIDTH * 0.3f)
			c = 219;
		else if (distance > SCREEN_WIDTH * 0.2f)
			c = '#';
		else if (distance > SCREEN_WIDTH * 0.1f)
			c = '+';

		drawChar((int)x + (SCREEN_WIDTH / 2), (int)y + (SCREEN_HEIGHT / 2), c, COLOUR::WHITE);
	}

	if (t < 3.0f)
	{
		const char* logo = 
			"       &&                                             &     "\
			"       & &                                           & &    "\
			"       &  &                                        &&  &    "\
			"      &    &&                                     &     &   "\
			"     &      %&                                 &&&      &   "\
			"      &       &&                             %&         &   "\
			"      &         &&                          &          %&   "\
			"      &           &            &          &&            &&  "\
			"     &             &&       &&& &&&     &&            ## && "\
			"    &&#  &#          &&& &&&      &&&& &          &%#####&& "\
			"    & # #% &            &             &          & %##  ##& "\
			"   %&#  #%  &#                                 %&  %#     & "\
			"   &%  %%%   &%                               &&   %#%    & "\
			"   &#  %  #   &    %%%%%%                    &&   % %%   %& "\
			"   &#   %    &    %%  %%%%        &            &   %%%%  && "\
			"  &&&  %   %&    #                 %&&&         &   %#   &&&"\
			"  & &&  %  &&&&&&&&&&&&                &&#&&&&&&&&&%%   && &"\
			"  #&     &%            &&&%&       &&&&             &%%%   &"\
			"   &&  %#&%                 &&&&&&&               %&% %   & "\
			"   &&& %% &&      &&&&&&&                        %&& %  &&& "\
			"    &%  % &&     &&&&&&&&&          &&&&&&        &&%%    & "\
			"   && ##%& &     &&&&&&&&&&        &&&&&&&&&     %&&&   &&  "\
			"     &&&&   &#&   &&&&&&&                      &##  &#  %&  "\
			"     &&&%##   %#&          &&&&&&&&         &&#&     &# %&  "\
			"      &%##%%     &&&&&&   &        &  %&&&&&         #&%    "\
			"     &%##                  &      &                 ###&&   "\
			"    &%##       &            &&&&&&           &        ##&&  "\
			"  %&&%#        #&%        &&      &&        &&         #%&&&"\
			"    &&&&##      &&&       &&&    &&&      &&&        #&&&#  "\
			"  &&&%###        &&&%&%%%&&&&&&&&&&%%&&&&&&&        ###%&&& "\
			"     eee&&&#       &%eeeeeeeeeeeeeeeeee##&      ##&&eeee    "\
			"     ee  &&&&%#      &&&pp&        &pp&&       ###%&  eee   "\
			"     ee     &&&&          &#      &#          #&&&    eee   "\
			"      ee    %%%&&&&&       &&% %&&%        #&&&      eee    "\
			" eeeeeeeeeeeeeeeeeee&&&                &&&eeeeeeeeeeeeeeeee "\
			"ee    eee ee       e   &&&&        &&&&  e     e     e    ee"\
			"e  eeeee   ee     ee  ee e &&&&&&&& e  e  e ee ee ee e ee ee"\
			"e ee  ee e ee e e ee  ee e ee e  e  e  e  e ee ee  eee    ee"\
			"e  ee ee e ee e e ee   eee    e  e  e    ee    ee ee e eee e"\
			"ee    e    ee e e ee  eeee eeee  e  e  eeee  eeee ee e eee e"\
			"eeeeeee ee  e e e ee  ee e eeee  e  e  eeee  eee    eeeeeeee"\
			"     eeeeeeeeeeee  e     e eeeee   ee  eeee  eeeeeeeeee     "\
			"          eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee          ";

		for (int i = (t - 2.0f >= 0 ? t - 2.0f : 0.0f) * 60 * 43; i < 60 * 43; i++)
		{
			if (logo[i] != ' ')
				drawChar((SCREEN_WIDTH / 2) - 30 + (i % 60), 8 + (int)(i / 60), 219, COLOUR::WHITE);
		}
	}

	int yPos = (int)easeOut(t > 2.0f ? t - 2.0f : 0.0f, -47, 30 + 20, 1.0f);
	if (t > 3)
	{
		yPos = 4 + (int)(sinf(t) * 3.0f);
	}

	drawFillCircle((SCREEN_WIDTH / 2), (SCREEN_HEIGHT / 2) + yPos, 16, 176, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) - 1, (SCREEN_HEIGHT / 2) + yPos - 1, 15, 177, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) + 0, (SCREEN_HEIGHT / 2) + yPos - 1, 15, 177, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) + 0, (SCREEN_HEIGHT / 2) + yPos + 0, 15, 177, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) - 2, (SCREEN_HEIGHT / 2) + yPos - 2, 14, 178, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) - 1, (SCREEN_HEIGHT / 2) + yPos - 2, 14, 178, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) - 1, (SCREEN_HEIGHT / 2) + yPos - 1, 14, 178, COLOUR::DARK_YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) - 4, (SCREEN_HEIGHT / 2) + yPos - 5, 9, 178, COLOUR::YELLOW);
	drawFillCircle((SCREEN_WIDTH / 2) - 5, (SCREEN_HEIGHT / 2) + yPos - 6, 6, 219, COLOUR::YELLOW);

	const char* title = 
	" ____  _______        ______    _    _     _     " \
	"|  _ \\| ____\\ \\      / / __ )  / \\  | |   | |    " \
	"| |_) |  _|  \\ \\ /\\ / /|  _ \\ / _ \\ | |   | |    " \
	"|  __/| |___  \\ V  V / | |_) / ___ \\| |___| |___ " \
	"|_|   |_____|  \\_/\\_/  |____/_/   \\_\\_____|_____|";

	float zoom = t - 3.0f;
	if (zoom < 0)
		zoom = 0.0f;
	else if (zoom > 1)
		zoom = 1.0f;

	if (zoom > 0)
	{
		int offset = (int)(1 / zoom);
		for (int i = 0; i < 49 * 5; i++)
		{
			if (i % offset == 0)
				drawChar((SCREEN_WIDTH / 2) - (int)(24 / zoom) + (i % 49), 8 + (int)(i / (49 * zoom)), title[i], COLOUR::GREEN);
		}
	}

	int offset = -SCREEN_WIDTH + (int)(t * SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		drawLine(offset, SCREEN_HEIGHT + i, SCREEN_WIDTH, i, 219, 0);
	}

	DrawMessage("Developed by Gamepopper - 2018 - FloppyJam", (SCREEN_WIDTH / 2) - (strlen("Developed by Gamepopper - 2018 - FloppyJam") / 2), 57, COLOUR::YELLOW, t - 4.0f);
	DrawMessage("W/A/S/D = AIM", 3, (SCREEN_HEIGHT / 2) - 2 + 8, COLOUR::YELLOW, t - 5.0f);
	DrawMessage("SPACE = FIRE", 3, (SCREEN_HEIGHT / 2) + 8, COLOUR::YELLOW, t - 5.0f);
	DrawMessage("O/P = LEFT/RRIGHT FLIPPERS", 3, (SCREEN_HEIGHT / 2) + 2 + 8, COLOUR::YELLOW, t - 5.0f);

	DrawMessage("PLAY (Y) or QUIT (N)", SCREEN_WIDTH - 3 - strlen("PLAY (Y) or QUIT (N)"), (SCREEN_HEIGHT / 2) + 8, COLOUR::YELLOW, t - 5.0f);
}

void TitleState::DrawMessage(const char* message, int x, int y, short colour, float t)
{
	if (t < 0)
		return;

	if (t > 1)
		t = 1;

	int length = (int)(strlen(message) * t);

	for (int i = 0; i < length; i++)
	{
		drawChar(x + i, y, message[i], colour);
	}
}