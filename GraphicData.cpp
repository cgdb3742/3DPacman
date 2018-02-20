#include "GraphicData.h"



GraphicData::GraphicData()
{
	pacmanModel = new Model3D();
	ghostModel = new Model3D();
	dotModel = new Model3D();
	wallModel = new Model3D();
	gateModel = new Model3D();
	arrowModel = new Model3D();

	spriteSheet = new Texture();

	LoadPacmanModel("pacman_tri.off");
	LoadGhostModel("ghost_tri.off");
	LoadDotModel("sphere_tri.off");
	LoadWallModel("wall_tri.off");
	LoadGateModel("gate_tri.off");
	LoadArrowModel("arrow_tri.off");

	LoadSpriteSheet("sprite_sheet.tga", "sprite_sheetm.tga");
}


GraphicData::~GraphicData()
{
	if (pacmanModel != nullptr) {
		delete pacmanModel;
	}
	if (ghostModel != nullptr) {
		delete ghostModel;
	}
	if (dotModel != nullptr) {
		delete dotModel;
	}
	if (wallModel != nullptr) {
		delete wallModel;
	}
	if (gateModel != nullptr) {
		delete gateModel;
	}
	if (arrowModel != nullptr) {
		delete arrowModel;
	}

	if (spriteSheet != nullptr) {
		delete spriteSheet;
	}
}

void GraphicData::LoadPacmanModel(char * model_name)
{
	if (pacmanModel != nullptr) {
		delete pacmanModel;
	}

	pacmanModel = new Model3D();
	pacmanModel->load_3d_model(model_name);
}

void GraphicData::LoadGhostModel(char * model_name)
{
	if (ghostModel != nullptr) {
		delete ghostModel;
	}

	ghostModel = new Model3D();
	ghostModel->load_3d_model(model_name);
}

void GraphicData::LoadDotModel(char * model_name)
{
	if (dotModel != nullptr) {
		delete dotModel;
	}

	dotModel = new Model3D();
	dotModel->load_3d_model(model_name);
}

void GraphicData::LoadWallModel(char * model_name)
{
	if (wallModel != nullptr) {
		delete wallModel;
	}

	wallModel = new Model3D();
	wallModel->load_3d_model(model_name);
}

void GraphicData::LoadGateModel(char * model_name)
{
	if (gateModel != nullptr) {
		delete gateModel;
	}

	gateModel = new Model3D();
	gateModel->load_3d_model(model_name);
}

void GraphicData::LoadArrowModel(char * model_name)
{
	if (arrowModel != nullptr) {
		delete arrowModel;
	}

	arrowModel = new Model3D();
	arrowModel->load_3d_model(model_name);
}

void GraphicData::LoadSpriteSheet(char * sheet_name, char * alpha_name)
{
	if (spriteSheet != nullptr) {
		delete spriteSheet;
	}

	spriteSheet = new Texture();

	spriteSheet->load_texture(sheet_name, alpha_name);

	glGenTextures(1, spriteSheet->OpenGL_ID);

	pacmanPosition = SpritePosition();
	blinkyPosition = SpritePosition();
	pinkyPosition = SpritePosition();
	inkyPosition = SpritePosition();
	clydePosition = SpritePosition();

	
	for (int i = 0; i < 4; i++) {
		Direction toUse = undefined;

		switch (i) {
		case 0: toUse = left; break;
		case 1: toUse = right; break;
		case 2: toUse = up; break;
		case 3: toUse = down; break;
		}

		pacmanPosition.SetCorner(0, toUse, false, 5);
		pacmanPosition.SetCorner(1, toUse, false, 17);
		pacmanPosition.SetCorner(2, toUse, false, 1 + i * 16);
		pacmanPosition.SetCorner(3, toUse, false, 13 + i * 16);

		pacmanPosition.SetCorner(0, toUse, true, 21);
		pacmanPosition.SetCorner(1, toUse, true, 33);
		pacmanPosition.SetCorner(2, toUse, true, 1 + i * 16);
		pacmanPosition.SetCorner(3, toUse, true, 13 + i * 16);
	}
	for (int i = 0; i < 4; i++) {
		Direction toUse = undefined;

		switch (i) {
		case 0: toUse = left; break;
		case 1: toUse = right; break;
		case 2: toUse = up; break;
		case 3: toUse = down; break;
		}

		blinkyPosition.SetCorner(0, toUse, false, 5 + i * 32);
		blinkyPosition.SetCorner(1, toUse, false, 18 + i * 32);
		blinkyPosition.SetCorner(2, toUse, false, 65);
		blinkyPosition.SetCorner(3, toUse, false, 78);

		blinkyPosition.SetCorner(0, toUse, true, 21 + i * 32);
		blinkyPosition.SetCorner(1, toUse, true, 34 + i * 32);
		blinkyPosition.SetCorner(2, toUse, true, 65);
		blinkyPosition.SetCorner(3, toUse, true, 78);
	}
	for (int i = 0; i < 4; i++) {
		Direction toUse = undefined;

		switch (i) {
		case 0: toUse = left; break;
		case 1: toUse = right; break;
		case 2: toUse = up; break;
		case 3: toUse = down; break;
		}

		pinkyPosition.SetCorner(0, toUse, false, 5 + i * 32);
		pinkyPosition.SetCorner(1, toUse, false, 18 + i * 32);
		pinkyPosition.SetCorner(2, toUse, false, 81);
		pinkyPosition.SetCorner(3, toUse, false, 94);

		pinkyPosition.SetCorner(0, toUse, true, 21 + i* 32);
		pinkyPosition.SetCorner(1, toUse, true, 34 + i* 32);
		pinkyPosition.SetCorner(2, toUse, true, 81);
		pinkyPosition.SetCorner(3, toUse, true, 94);
	}
	for (int i = 0; i < 4; i++) {
		Direction toUse = undefined;

		switch (i) {
		case 0: toUse = left; break;
		case 1: toUse = right; break;
		case 2: toUse = up; break;
		case 3: toUse = down; break;
		}

		inkyPosition.SetCorner(0, toUse, false, 5 + i * 32);
		inkyPosition.SetCorner(1, toUse, false, 18 + i * 32);
		inkyPosition.SetCorner(2, toUse, false, 97);
		inkyPosition.SetCorner(3, toUse, false, 110);

		inkyPosition.SetCorner(0, toUse, true, 21 + i* 32);
		inkyPosition.SetCorner(1, toUse, true, 34 + i * 32);
		inkyPosition.SetCorner(2, toUse, true, 97);
		inkyPosition.SetCorner(3, toUse, true, 110);
	}
	for (int i = 0; i < 4; i++) {
		Direction toUse = undefined;

		switch (i) {
		case 0: toUse = left; break;
		case 1: toUse = right; break;
		case 2: toUse = up; break;
		case 3: toUse = down; break;
		}

		clydePosition.SetCorner(0, toUse, false, 5 + i * 32);
		clydePosition.SetCorner(1, toUse, false, 18 + i * 32);
		clydePosition.SetCorner(2, toUse, false, 113);
		clydePosition.SetCorner(3, toUse, false, 126);

		clydePosition.SetCorner(0, toUse, true, 21 + i * 32);
		clydePosition.SetCorner(1, toUse, true, 34 + i * 32);
		clydePosition.SetCorner(2, toUse, true, 113);
		clydePosition.SetCorner(3, toUse, true, 126);
	}
}

Model3D * GraphicData::GetPacmanModel()
{
	return pacmanModel;
}

Model3D * GraphicData::GetGhostModel()
{
	return ghostModel;
}

Model3D * GraphicData::GetDotModel()
{
	return dotModel;
}

Model3D * GraphicData::GetWallModel()
{
	return wallModel;
}

Model3D * GraphicData::GetGateModel()
{
	return gateModel;
}

Model3D * GraphicData::GetArrowModel()
{
	return arrowModel;
}

SpritePosition & GraphicData::GetPacmanPosition()
{
	return pacmanPosition;
}

SpritePosition & GraphicData::GetGhostPosition(GhostType type)
{
	switch (type) {
	case blinky: return blinkyPosition;
	case pinky: return pinkyPosition;
	case inky: return inkyPosition;
	case clyde: default: return clydePosition;
	}
}

void GraphicData::DrawImage(Texture * tex, int i1, int i2, int j1, int j2, float x, float y, float w, float h, float angle)
{
	int sizex = tex->img_color->lenx;
	int sizey = tex->img_color->leny;

	float x1 = (float)i1 / (float)sizex;
	float x2 = (float)(i2 + 1) / (float)sizex;
	float y1 = (float)j1 / (float)sizey;
	float y2 = (float)(j2 + 1) / (float)sizey;

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glPushMatrix();
	//glTranslatef(x, y, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);    // on répète la texture en cas de U,V > 1.0
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);    // ou < 0.0


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); // indique qu'il faut mélanger la texture avec la couleur courante

	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex->OpenGL_ID[0]);

	if (tex->isRGBA)
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, tex->img_color->lenx, tex->img_color->leny, GL_RGBA, GL_UNSIGNED_BYTE, tex->img_all);
	else
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, tex->img_color->lenx, tex->img_color->leny, GL_RGB, GL_UNSIGNED_BYTE, tex->img_color->data);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

	glBegin(GL_QUADS);
	//glTexCoord2f(x1, y2); glVertex3f(0.0f, 0.0f, 0.0f);
	//glTexCoord2f(x1, y1); glVertex3f(0.0f, h, 0.0f);
	//glTexCoord2f(x2, y1); glVertex3f(w, h, 0.0f);
	//glTexCoord2f(x2, y2); glVertex3f(w, 0.0f, 0.0f);
	glTexCoord2f(x2, y1); glVertex3f(-x, -y, 0.0f);
	glTexCoord2f(x1, y1); glVertex3f(-x - w, -y, 0.0f);
	glTexCoord2f(x1, y2); glVertex3f(-x - w, -y - h, 0.0f);
	glTexCoord2f(x2, y2); glVertex3f(-x, -y - h, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glPopMatrix();
}

void GraphicData::DrawImage(Texture* tex,
	float x,
	float y,
	float w,
	float h,
	float angle)
{
	DrawImage(tex, 0, tex->img_color->lenx - 1, 0, tex->img_color->leny - 1, x, y, w, h, angle);
}

void GraphicData::DrawImage(SpritePosition & pos, Direction dir, bool alternate, float x, float y, float w, float h, float angle)
{
	DrawImage(spriteSheet, pos.GetCorner(0, dir, alternate), pos.GetCorner(1, dir, alternate), pos.GetCorner(2, dir, alternate), pos.GetCorner(3, dir, alternate), x, y, w, h, angle);
}

void GraphicData::DrawImage(SpritePosition & pos, float sprangle, bool alternate, float x, float y, float w, float h, float angle)
{
	DrawImage(pos, Enums::AngleToDir(sprangle), alternate, x, y, w, h, angle);
}
