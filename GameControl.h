#pragma once

#include "input.h"

#include "GraphicData.h"
#include "GameMap.h"

class GameControl
{
public:
	GameControl();
	~GameControl();

	virtual void Update(float dt);
	virtual void ProcessInput(MY_INPUT * inp);
	virtual void Draw();
	virtual void DrawMap(WINDOW* win, float leftCorner, float upCorner, float sizex, float sizey);
	virtual void SetCamera(bool firstPerson);

	GraphicData& GetGraphicData();

protected:
	GameMap gameMap;

	GraphicData graphicData;
};

