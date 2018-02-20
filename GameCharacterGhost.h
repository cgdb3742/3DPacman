#pragma once
#include "GameCharacter.h"

class GameCharacterGhost :
	public GameCharacter
{
public:
	GameCharacterGhost();
	~GameCharacterGhost();

	virtual void Update(float dt, GameMap & map);
	virtual void ProcessInput(MY_INPUT * inp);
	virtual void DrawCharacter(GraphicData& graphicData, float xpos, float ypos, float zpos);
	virtual void DrawCharacter(GraphicData& graphicData, float xoffset, float yoffset);
	//virtual void Draw(GraphicData& graphicData);
	virtual SpritePosition& GetSpritePosition(GraphicData& graphicData);
	virtual GLenum GetLightID();

	void SetGhostType(GhostType newType, GameMap & map);
	void SetTarget(GameMap & map, bool isChaseMode);

	void SetVulnerable(bool vulnerable);
	bool GetVulnerable();

	void SetStartDelay(float value);
	void SetSpawnDone(bool value);

	virtual void SetDead(bool value, GameMap& map);

protected:
	float bodyAnimationTimer = 0.0f;
	float bodyAnimationCycleDuration = 4.0f;

	float vulnerableAnimationTimer = 0.0f;
	float vulnerableAnimationCycleDuration = 1.0f;

	float vulnerableTimer = 0.0f;
	float vulnerableTimerMax = 15.0f;

	unsigned int redDefault;
	unsigned int greenDefault;
	unsigned int blueDefault;
	unsigned int redVulnerable;
	unsigned int greenVulnerable;
	unsigned int blueVulnerable;
	unsigned int redVulnerableAlternate;
	unsigned int greenVulnerableAlternate;
	unsigned int blueVulnerableAlternate;

	bool chaseMode;
	bool spawnDone;

	GhostType type;

	float startDelay;
};

