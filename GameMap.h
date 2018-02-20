#pragma once

#include <list>

#include "load.h"
#include "input.h"

#include "Enums.h"

#include "GameCharacterPacman.h"
#include "GameCharacterGhost.h"
#include "PacDot.h"

class GameControl;
class GraphicData;

class GameMap
{
public:
	GameMap();
	~GameMap();

	virtual void Update(float dt);
	virtual void ProcessInput(MY_INPUT * inp);
	virtual void Draw(GraphicData& graphicData);
	virtual void DrawWall(GraphicData& graphicData, float xpos, float ypos, float zpos);
	virtual void DrawGate(GraphicData& graphicData, float xpos, float ypos, float zpos, Direction dir);
	virtual void DrawFloor(GraphicData& graphicData, float xpos, float ypos, float zpos);
	virtual void DrawArrow(GraphicData& graphicData, float xpos, float ypos, float zpos, float xtarget, float ytarget);
	virtual void DrawMap(GraphicData& graphicData, float leftCorner, float upCorner, float sizex, float sizey, bool stretch);
	virtual void SetCamera(bool firstPerson);

	virtual void ComputeContacts();

	virtual void ActivatePacDots(bool activateSuperGums);
	virtual void DeactivatePacDots(bool deactivateSuperGums);
	virtual void ActivateRandomPacDot(bool activateSuperGum);

	void SetGhostsVulnerable(bool vulnerable);

	bool LoadMaps(char * newData, char * newDot, char * newColor);

	unsigned int GetDataRed(int i, int j);
	unsigned int GetDataGreen(int i, int j);
	unsigned int GetDataBlue(int i, int j);
	unsigned int GetColorRed(int i, int j);
	unsigned int GetColorGreen(int i, int j);
	unsigned int GetColorBlue(int i, int j);
	
	unsigned int GetDataTypeRed(DataType dataType);
	unsigned int GetDataTypeGreen(DataType dataType);
	unsigned int GetDataTypeBlue(DataType dataType);
	DataType GetDataType(unsigned int redValue, unsigned int greenValue, unsigned int blueValue);
	DataType GetDataType(int i, int j);

	void ClearPacDots();
	void ClearSuperGums();

	bool HasPacDot(int i, int j);
	bool HasSuperGum(int i, int j);

	int GetI(int i);
	int GetJ(int j);
	int GetI(float x);
	int GetJ(float y);

	float GetCenterX(int i);
	float GetCenterY(int j);
	float GetCenterX(float x);
	float GetCenterY(float y);

	float GetCenterX(int i1, int i2);
	float GetCenterY(int j1, int j2);

	float GetPacmanStartX();
	float GetPacmanStartY();
	float GetGhostSpawnX();
	float GetGhostSpawnY();
	float GetGhostExitX();
	float GetGhostExitY();

	float WrapX(float x, Direction moveDir);
	float WrapY(float y, Direction moveDir);

	float GetXSize();
	float GetYSize();
	float GetRes();
	bool GetAlternate();

	bool IsWall(int i, int j);

	bool CanMoveTo(int i, int j);
	bool CanMoveUp(int i, int j, bool isGhost);
	bool CanMoveLeft(int i, int j, bool isGhost);
	bool CanMoveDown(int i, int j, bool isGhost);
	bool CanMoveRight(int i, int j, bool isGhost);

	GameCharacterPacman* GetPacman();
	GameCharacterGhost* GetGhost(GhostType type);

	void ResetAll();
	void SpawnPacman();
	void SpawnGhost(GhostType type, bool firstSpawn);

	int GetDistance0(int i1, int j1, int i2, int j2);
	int GetDistance1(int i1, int j1, int i2, int j2);
	int GetDistance2(int i1, int j1, int i2, int j2);
	int GetDistance0(GameCharacter* char1, GameCharacter* char2);
	int GetDistance1(GameCharacter* char1, GameCharacter* char2);
	int GetDistance2(GameCharacter* char1, GameCharacter* char2);

	bool Contact(GameCharacter* char1, GameCharacter* char2);
	bool Contact(GameCharacter* char1, PacDot* dot);

	bool UseAStarPathing();

	bool IsCurrentGamePhaseDone();
	void StartNextGamePhase();
	void ResetGamePhase();

protected:
	float alternateTimer;
	float maxAlternateTimer;
	bool alternate;
	unsigned int width;
	unsigned int height;
	float res;

	bool repeatHorizontal = true;
	bool repeatVertical = false;
	int repeatDist = 5;

	bool useNoUpCrosses = false;
	bool useAStarPathing = false; //Use original algo otherwise
	bool useDistanceForCollision = true;

	Image *dataMap;
	Image *colorMap;
	Image *dotMap;

	Texture *colorTexture;

	int pacmanStartI;
	int pacmanStartJ;
	Direction pacmanStartOffset;

	int ghostSpawnI;
	int ghostSpawnJ;
	Direction ghostSpawnOffset;
	Direction ghostSpawnDir;

	int ghostExitI;
	int ghostExitJ;
	Direction ghostExitOffset;

	GameCharacterPacman* pacman;
	GameCharacterGhost* ghostBlinky;
	GameCharacterGhost* ghostPinky;
	GameCharacterGhost* ghostInky;
	GameCharacterGhost* ghostClyde;

	std::list<PacDot*> pacDots;
	std::list<PacDot*> superGums;
	PacDot* currentSuperGum = nullptr;

	int eatenDots = 0;
	int targetEatentDots = 25;

	float freeRoamTimer = 0.0f;
	float freeRoamDuration = 15.0f;

	GamePhase currentPhase;
};

