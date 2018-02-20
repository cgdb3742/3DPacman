#pragma once
#include "GameCharacter.h"
class GameCharacterPacman :
	public GameCharacter
{
public:
	GameCharacterPacman();
	~GameCharacterPacman();

	virtual void Update(float dt, GameMap & map);
	virtual void ProcessInput(MY_INPUT * inp);
	//virtual void Draw(GraphicData& graphicData);

	virtual void DrawCharacter(GraphicData& graphicData, float xpos, float ypos, float zpos);
	virtual void DrawCharacter(GraphicData& graphicData, float xoffset, float yoffset);
	virtual void DrawPacmanModel(GraphicData& graphicData, float mouthPosAngle, float mouthOpenAngle);

	virtual void SetDead(bool value, GameMap& map);

protected:
	float mouthAnimationTimer = 0.0f;
	float mouthAnimationCycleDuration = 0.5f;

	float deathAnimationTimer = 0.0f;
	float deathAnimationDuration = 2.0f;
};

