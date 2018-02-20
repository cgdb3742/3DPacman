#pragma once

#include "ogl.h"

#include "load.h"
#include "input.h"

#include "Enums.h"

class GameMap;
class GraphicData;
class SpritePosition;

class GameCharacter
{
public:
	GameCharacter();
	~GameCharacter();

	virtual void Update(float dt, GameMap & map);
	virtual void ProcessInput(MY_INPUT * inp);
	virtual void Draw(GraphicData& graphicData);
	virtual void DrawCharacter(GraphicData& graphicData, float xpos, float ypos, float zpos);
	virtual void DrawCharacter(GraphicData& graphicData, float xoffset, float yoffset);
	virtual void DrawMap(GraphicData& graphicData, GameMap& map, float leftCorner, float upCorner, float sizex, float sizey);
	virtual SpritePosition& GetSpritePosition(GraphicData& graphicData);
	virtual void DrawEyes(GraphicData& graphicData);
	virtual void SetCamera(bool firstPerson);
	virtual GLenum GetLightID();

	virtual float Move(float dt, GameMap & map);
	virtual float Rotate(float dt, GameMap & map);
	virtual void FindNewDirection(GameMap & map);

	void SetPosition(int i, int j, GameMap & map);
	void SetPosition(int i1, int i2, int j1, int j2, GameMap & map);
	void SetColor(unsigned int redValue, unsigned int greenValue, unsigned int blueValue);

	virtual bool GetDead();
	virtual void SetDead(bool value, GameMap& map);

	int GetI(GameMap & map);
	int GetJ(GameMap & map);
	float GetX();
	float GetY();
	Direction GetDirection();

	void SetDirection(Direction newDir);

	void SetTarget(int i, int j);
	Direction GetBestDirection(GameMap & map, bool upAvailable, bool leftAvailable, bool downAvailable, bool rightAvailable);

protected:
	float x = 0.0f;
	float y = 0.0f;
	float angle = 0.0f;

	float moveSpeed = 1.0f;
	float rotateSpeed = 90.0f; //deg
	float speedMult = 1.0f;
	float tunnelSpeedMult = 0.75f;

	bool isGhost = false;

	bool isDead = false;

	bool playerControl;
	bool directControl; //false : use targets instead

	int targetI = 0;
	int targetJ = 0;

	int primaryTargetKey;
	int secondaryTargetKey;

	bool leftRequested = false;
	bool rightRequested = false;
	bool backRequested = false;

	int leftKey;
	int rightKey;
	int backKey;
	int forwardKey;

	float red = 0;
	float green = 0;
	float blue = 0;

	Direction dir = undefined;
};

