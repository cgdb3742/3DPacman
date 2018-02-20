#include "GameControl.h"



GameControl::GameControl()
{
}


GameControl::~GameControl()
{
}

void GameControl::Update(float dt)
{
	gameMap.Update(dt);
}

void GameControl::ProcessInput(MY_INPUT * inp)
{
	gameMap.ProcessInput(inp);
}

void GameControl::Draw()
{
	gameMap.Draw(GetGraphicData());
}

void GameControl::DrawMap(WINDOW* win, float leftCorner, float upCorner, float sizex, float sizey)
{
	glOrtho(0.0, (double)win->Xres, 0.0, (double)win->Yres, 10, 30000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();	// vecteur up
	gluLookAt(0, 0, -100,		// position
		0, 0, 1,		// point cible
		0, 1, 0);		// vecteur up

	//glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);

	//glAlphaFunc(GL_EQUAL, 0.75f);


	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_LEQUAL, 0.2f);

	gameMap.DrawMap(GetGraphicData(), leftCorner, upCorner, sizex, sizey, false);

	//glDisable(GL_ALPHA_TEST);

	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
}

void GameControl::SetCamera(bool firstPerson)
{
	gameMap.SetCamera(firstPerson);
}

GraphicData & GameControl::GetGraphicData()
{
	return graphicData;
}
