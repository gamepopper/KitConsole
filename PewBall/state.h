#pragma once

enum GAMESTATE
{
	GS_INIT,
	GS_GOTITLE,
	GS_TITLE,
	GS_GOPLAY,
	GS_PLAY,
	GS_SHUTDOWN
};

static GAMESTATE currentState;
static GAMESTATE nextState;

struct StateObject
{
	virtual void Initialise() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render() = 0;
	virtual void Cleanup() = 0;
};