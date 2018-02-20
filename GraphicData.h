#pragma once

#include "load.h"

#include "Enums.h"

class SpritePosition {
	public:
		SpritePosition() {

		}
		~SpritePosition() {
			
		}

		//corner = 0 : i1
		//corner = 1 : i2
		//corner = 2 : j1
		//corner = 3 : j2
		int GetCorner(int corner, Direction dir, bool alternate) {
			if (dir == undefined || corner < 0 || corner >= 4) return 0;

			int id = ((int)dir - 1) * 8 + corner * 2 + (alternate ? 1 : 0);

			return posData[id];
		}

		bool SetCorner(int corner, Direction dir, bool alternate, int value) {
			if (dir == undefined || corner < 0 || corner >= 4) return false;

			int id = ((int)dir - 1) * 8 + corner * 2 + (alternate ? 1 : 0);

			posData[id] = value;

			return true;
		}

	protected:
		int posData[32];
};

class GraphicData
{
public:
	GraphicData();
	~GraphicData();

	void LoadPacmanModel(char* model_name);
	void LoadGhostModel(char* model_name);
	void LoadDotModel(char* model_name);
	void LoadWallModel(char* model_name);
	void LoadGateModel(char* model_name);
	void LoadArrowModel(char* model_name);

	void LoadSpriteSheet(char* sheet_name, char* alpha_name);

	Model3D* GetPacmanModel();
	Model3D* GetGhostModel();
	Model3D* GetDotModel();
	Model3D* GetWallModel();
	Model3D* GetGateModel();
	Model3D* GetArrowModel();

	SpritePosition& GetPacmanPosition();
	SpritePosition& GetGhostPosition(GhostType type);

	void DrawImage(Texture* tex, int i1, int i2, int j1, int j2, float x, float y, float w, float h, float angle);
	void DrawImage(Texture* tex, float x, float y, float w, float h, float angle);
	void DrawImage(SpritePosition& pos, Direction dir, bool alternate, float x, float y, float w, float h, float angle);
	void DrawImage(SpritePosition& pos, float sprangle, bool alternate, float x, float y, float w, float h, float angle);

protected:
	Model3D* pacmanModel;
	Model3D* ghostModel;
	Model3D* dotModel;
	Model3D* wallModel;
	Model3D* gateModel;
	Model3D* arrowModel;

	Texture* spriteSheet;

	SpritePosition pacmanPosition;
	SpritePosition blinkyPosition;
	SpritePosition pinkyPosition;
	SpritePosition inkyPosition;
	SpritePosition clydePosition;
};

