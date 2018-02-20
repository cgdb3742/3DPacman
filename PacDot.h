#pragma once

class GameMap;
class GraphicData;

class PacDot
{
public:
	PacDot();
	PacDot(int posi, int posj, bool superGum);
	~PacDot();

	void SetPosition(int ivalue, int jvalue);
	int GetI();
	int GetJ();
	float GetX(GameMap& map);
	float GetY(GameMap& map);

	void SetActive(bool newValue);
	bool IsEaten();
	bool CanBeEaten();
	void EatDot();

	virtual void Update(float dt, GameMap & map);

	virtual void DrawDot(GraphicData& graphicData, GameMap& map);
	virtual void DrawDot(GraphicData& graphicData, GameMap& map, float xoffset, float yoffset);
	virtual void DrawDot(GraphicData& graphicData, float xpos, float ypos, float zpos);

protected:
	bool active = false;
	float activationTimer = 0.0f;
	float maxActivationTimer = 1.0f;
	bool eaten = false;
	int i = 0;
	int j = 0;
	bool isSuperGum = false;
};

