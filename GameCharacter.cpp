#include "GameCharacter.h"

#include <cmath>
//#include <math.h>

#include "GameMap.h"
#include "GraphicData.h"


GameCharacter::GameCharacter()
{
	primaryTargetKey = KEY_CODE_SPACE;
	secondaryTargetKey = KEY_CODE_SPACE;
	leftKey = KEY_CODE_LEFT;
	rightKey = KEY_CODE_RIGHT;
	backKey = KEY_CODE_DOWN;
	forwardKey = KEY_CODE_UP;

	speedMult = 1.0f;
	targetI = 14;
	targetJ = 18;
	isGhost = false;
	rotateSpeed = 360.0f;
	moveSpeed = 3.0f;

	playerControl = false;
	directControl = false;
}


GameCharacter::~GameCharacter()
{
}

//TODO Can move trough walls after a back order and moving right ?
void GameCharacter::Update(float dt, GameMap & map)
{
	while (dt > 0.0f) {
		if (dir == undefined) {
			dir = Enums::AngleToDir(angle);
			FindNewDirection(map);

			if (dir == undefined) {
				dt = 0.0f;
			}
		//} else if (angle == Enums::DirToAngle(dir)) {
		} else if (Enums::AreClose(angle, dir)) {
			dt = Move(dt, map);

			//if (dt > 0.0f && Enums::AreClose(angle, dir)) {
			if (dt > 0.0f || (x == map.GetCenterX(x) && y == map.GetCenterY(y))) {
				FindNewDirection(map);
			}
		} else {
			dt = Rotate(dt, map);
		}
	}
}

void GameCharacter::ProcessInput(MY_INPUT * inp)
{
	if (!playerControl) {
		return;
	}

	if (directControl) {
		if (inp->keys[backKey] && !isGhost) {
			backRequested = dir == undefined || Enums::AreClose(angle, dir);
			leftRequested = false;
			rightRequested = false;
		}
		if (inp->keys[leftKey]) {
			leftRequested = dir == undefined || Enums::AreClose(angle, dir);
			rightRequested = false;
			backRequested = false;
		}
		if (inp->keys[rightKey]) {
			rightRequested = dir == undefined || Enums::AreClose(angle, dir);
			leftRequested = false;
			backRequested = false;
		}
		if (inp->keys[forwardKey]) {
			leftRequested = false;
			rightRequested = false;
			backRequested = false;
		}
	}
	else {
		//if (inp->keys[primaryTargetKey] || inp->keys[secondaryTargetKey] || inp->keys[toupper(primaryTargetKey)] || inp->keys[toupper(secondaryTargetKey)]) {
		if (inp->keys[primaryTargetKey] || inp->keys[secondaryTargetKey]) {
			//TODO set new target if requirements met
		}
	}
}

void GameCharacter::Draw(GraphicData& graphicData)
{
	DrawCharacter(graphicData, x, y, 0.0f);

	GLenum lightID = GetLightID();

	GLfloat light_position[] = { x, y, -0.4f, 1.0f };
	GLfloat light_color[] = { red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f };
	glLightfv(lightID, GL_POSITION, light_position);
	glLightfv(lightID, GL_DIFFUSE, light_color);
	//glLightfv(ghostLight, GL_SPECULAR, light_color);
}

void GameCharacter::DrawCharacter(GraphicData & graphicData, float xpos, float ypos, float zpos)
{
	glPushMatrix();

	glScalef(1.0f, 1.0f, -1.0f);
	glTranslatef(xpos, ypos, zpos);
	glRotatef(-angle, 0.0f, 0.0f, 1.0f);

	GLfloat mat_specular[] = { red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f };
	GLfloat mat_shininess[] = { 50.0 };

	if (isGhost) {
		mat_specular[3] = 0.5f;
		glEnable(GL_CULL_FACE);
	}

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	Model3D* model;

	if (isGhost) {
		model = graphicData.GetGhostModel();
	} else {
		model = graphicData.GetPacmanModel();
	}

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < model->nb_triangles; i++) {
		//glColor3f(red / 255.0f, green / 255.0f, blue / 255.0f);
		glVertex3f(model->points[model->faces[i].a].x, model->points[model->faces[i].a].y, model->points[model->faces[i].a].z);

		//glColor3f(red / 255.0f, green / 255.0f, blue / 255.0f);
		glVertex3f(model->points[model->faces[i].b].x, model->points[model->faces[i].b].y, model->points[model->faces[i].b].z);

		//glColor3f(red / 255.0f, green / 255.0f, blue / 255.0f);
		glVertex3f(model->points[model->faces[i].c].x, model->points[model->faces[i].c].y, model->points[model->faces[i].c].z);
	}

	glEnd();

	if (isGhost) {
		glDisable(GL_CULL_FACE);
	}

	glBegin(GL_TRIANGLES);

	DrawEyes(graphicData);

	glEnd();

	glPopMatrix();
}

void GameCharacter::DrawCharacter(GraphicData & graphicData, float xoffset, float yoffset)
{
	DrawCharacter(graphicData, x + xoffset, y + yoffset, 0.0f);
}

void GameCharacter::DrawMap(GraphicData & graphicData, GameMap & map, float leftCorner, float upCorner, float sizex, float sizey)
{
	float posx = x;
	float posy = y;
	float mapw = map.GetXSize();
	float maph = map.GetYSize();
	float resx = sizex / mapw * map.GetRes();
	float resy = sizey / maph * map.GetRes();

	while (posx < 0) {
		posx += mapw;
	}
	while (posx >= mapw) {
		posx -= mapw;
	}
	while (posy < 0) {
		posy += maph;
	}
	while (posy >= maph) {
		posy -= maph;
	}

	SpritePosition& spritePos = GetSpritePosition(graphicData);
	
	graphicData.DrawImage(spritePos, angle, map.GetAlternate(), leftCorner + (posx - 0.5f) * resx, upCorner + (posy - 0.5f) * resy, resx, resy, 0.0f);
}

SpritePosition & GameCharacter::GetSpritePosition(GraphicData& graphicData)
{
	return graphicData.GetPacmanPosition();
}

void GameCharacter::DrawEyes(GraphicData & graphicData)
{
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, 0.1f, 0.2f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, 0.2f, 0.4f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, 0.1f, 0.4f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, 0.1f, 0.2f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, 0.2f, 0.4f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, 0.2f, 0.2f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, -0.1f, 0.2f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, -0.2f, 0.4f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, -0.1f, 0.4f);

	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, -0.1f, 0.2f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, -0.2f, 0.4f);
	//glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.45f, -0.2f, 0.2f);
}

void GameCharacter::SetCamera(bool firstPerson)
{
	float pi = 3.14159265358979f;

	float cosAngle = cosf(angle * pi / 180.0f);
	float sinAngle = sinf(angle * pi / 180.0f);

	if (firstPerson) {
		gluLookAt(x - cosAngle * 0.4f, y + sinAngle * 0.4f, -0.3f,		// position
			x + cosAngle * 2.0f, y - sinAngle * 2.0f, -0.3f,		// point cible
			0, 0, -1.0f);		// vecteur up
	} else {
		gluLookAt(x - cosAngle * 2.0f, y + sinAngle * 2.0f, -2.0f,		// position
			x + cosAngle * 0.5f, y - sinAngle * 0.5f, 0.0f,		// point cible
			0, 0, -1.0f);		// vecteur up
	}
}

GLenum GameCharacter::GetLightID()
{
	return GL_LIGHT1;
}

float GameCharacter::Move(float dt, GameMap & map)
{
	if (dir == undefined) {
		return 0.0f;
	}

	if (directControl && !isGhost && backRequested) {
		dir = Enums::ComposeDirections(dir, left);
		backRequested = false;
		return dt;
	}

	float maxDist = 0.0f;
	float target = 0.0f;

	switch (dir) {
	case up:
		target = map.GetCenterY(map.GetJ(y));
		if (target >= y) {
			target = map.GetCenterY(map.GetJ(y) - 1);
		}
		maxDist = y - target;
		break;
	case left:
		target = map.GetCenterX(map.GetI(x));
		if (target >= x) {
			target = map.GetCenterX(map.GetI(x) - 1);
		}
		maxDist = x - target;
		break;
	case down:
		target = map.GetCenterY(map.GetJ(y));
		if (target <= y) {
			target = map.GetCenterY(map.GetJ(y) + 1);
		}
		maxDist = target - y;
		break;
	case right:
		target = map.GetCenterX(map.GetI(x));
		if (target <= x) {
			target = map.GetCenterX(map.GetI(x) + 1);
		}
		maxDist = target - x;
		break;
	}

	float realSpeed = moveSpeed * speedMult;

	if (isGhost  &&  map.GetDataType(map.GetI(x), map.GetJ(y)) == tunnel) {
		realSpeed *= tunnelSpeedMult;
	}

	float maxTime = maxDist / realSpeed;
	float remainingTime = 0.0f;

	if (maxTime <= dt) {
		remainingTime = dt - maxTime;

		dt = maxTime;
	}

	switch (dir) {
	case up:
		y -= dt * realSpeed;
		break;
	case left:
		x -= dt * realSpeed;
		break;
	case down:
		y += dt * realSpeed;
		break;
	case right:
		x += dt * realSpeed;
		break;
	}

	x = map.WrapX(x, dir);
	y = map.WrapY(y, dir);

	return remainingTime;
}

float GameCharacter::Rotate(float dt, GameMap & map)
{
	float targetAngle = Enums::DirToAngle(dir);
	float angleDiff = targetAngle - angle;

	while (angleDiff <= -180.0f) {
		angleDiff += 360.0f;
	}
	while (angleDiff > 180.0f) {
		angleDiff -= 360.0f;
	}

	int rotateDir = 1;
	if (angleDiff < 0.0f) rotateDir = -1;

	float rotateAmount = abs(angleDiff);

	float realSpeed = rotateSpeed * speedMult;

	float maxTime = rotateAmount / realSpeed;
	float remainingTime = 0.0f;

	if (maxTime <= dt) {
		remainingTime = dt - maxTime;

		dt = maxTime;
	}

	angle += dt * realSpeed * rotateDir;

	while (angle <= -180.0f) {
		angle += 360.0f;
	}
	while (angle > 180.0f) {
		angle -= 360.0f;
	}

	return remainingTime;
}

void GameCharacter::FindNewDirection(GameMap & map)
{
	int i = map.GetI(x);
	int j = map.GetJ(y);

	bool upAvailable = map.CanMoveUp(i, j, isGhost);
	bool leftAvailable = map.CanMoveLeft(i, j, isGhost);
	bool downAvailable = map.CanMoveDown(i, j, isGhost);
	bool rightAvailable = map.CanMoveRight(i, j, isGhost);

	if (isGhost) { //A ghost can't move backwards
		switch (dir) {
		case up: downAvailable = false; break;
		case left: rightAvailable = false; break;
		case down: upAvailable = false; break;
		case right: leftAvailable = false; break;
		}
	}

	if (directControl) {
		bool canProgress = false;

		switch (dir) {
		case up: canProgress = upAvailable; break;
		case left: canProgress = leftAvailable; break;
		case down: canProgress = downAvailable; break;
		case right: canProgress = rightAvailable; break;
		}

		//TODO turning
		if (leftRequested || rightRequested || backRequested) {
			Direction wantedDir = dir;

			if (leftRequested) {
				wantedDir = Enums::ComposeDirections(dir, up);
			} else {
				wantedDir = Enums::ComposeDirections(dir, down);
			}

			if (backRequested) {
				wantedDir = Enums::ComposeDirections(dir, left);
				backRequested = false;
			}

			leftRequested = false;
			rightRequested = false;

			if (upAvailable && wantedDir == up) {
				dir = up;
				return;
			}
			if (leftAvailable && wantedDir == left) {
				dir = left;
				return;
			}
			if (downAvailable && wantedDir == down) {
				dir = down;
				return;
			}
			if (rightAvailable && wantedDir == right) {
				dir = right;
				return;
			}
		}

		if (!canProgress) {
			//dir = undefined;
			dir = undefined;

			if (isGhost) {
				if (upAvailable) {
					dir = up;
				} else if (leftAvailable) {
					dir = left;
				} else if (downAvailable) {
					dir = down;
				} else if (rightAvailable) {
					dir = right;
				} else {
					dir = Enums::ComposeDirections(dir, left);
				}
			}
		}
	} else {
		Direction bestDir = GetBestDirection(map, upAvailable, leftAvailable, downAvailable, rightAvailable);

		if (bestDir != undefined) {
			dir = bestDir;
		} else {
			dir = Enums::ComposeDirections(dir, left);
		}
	}
}

void GameCharacter::SetPosition(int i, int j, GameMap & map)
{
	x = map.GetCenterX(i);
	y = map.GetCenterY(j);
}

void GameCharacter::SetPosition(int i1, int i2, int j1, int j2, GameMap & map)
{
	x = map.GetCenterX(i1, i2);
	y = map.GetCenterY(j1, j2);
}

void GameCharacter::SetColor(unsigned int redValue, unsigned int greenValue, unsigned int blueValue)
{
	if (redValue >= 256) {
		red = 255;
	}
	else {
		red = redValue;
	}
	if (greenValue >= 256) {
		green = 255;
	}
	else {
		green = greenValue;
	}
	if (blueValue >= 256) {
		blue = 255;
	}
	else {
		blue = blueValue;
	}
}

bool GameCharacter::GetDead()
{
	return isDead;
}

void GameCharacter::SetDead(bool value, GameMap & map)
{
	isDead = value;
}

int GameCharacter::GetI(GameMap & map)
{
	return map.GetI(x);
}

int GameCharacter::GetJ(GameMap & map)
{
	return map.GetJ(y);
}

float GameCharacter::GetX()
{
	return x;
}

float GameCharacter::GetY()
{
	return y;
}

Direction GameCharacter::GetDirection()
{
	return dir;
}

void GameCharacter::SetDirection(Direction newDir)
{
	dir = newDir;
	angle = Enums::DirToAngle(dir);
}

void GameCharacter::SetTarget(int i, int j)
{
	targetI = i;
	targetJ = j;
}

//TODO AStar
Direction GameCharacter::GetBestDirection(GameMap & map, bool upAvailable, bool leftAvailable, bool downAvailable, bool rightAvailable)
{
	int bestDist = -1;
	Direction bestDir = undefined;

	int newDist = -1;

	if (upAvailable) {
		if (map.UseAStarPathing()) {
			//TODO
		}
		else {
			newDist = map.GetDistance2(GetI(map), GetJ(map) - 1, targetI, targetJ);
		}

		if (newDist >= 0 && (bestDist < 0 || newDist < bestDist)) {
			bestDist = newDist;
			bestDir = up;
		}

		newDist = -1;
	}

	if (leftAvailable) {
		if (map.UseAStarPathing()) {
			//TODO
		}
		else {
			newDist = map.GetDistance2(GetI(map) - 1, GetJ(map), targetI, targetJ);
		}

		if (newDist >= 0 && (bestDist < 0 || newDist < bestDist)) {
			bestDist = newDist;
			bestDir = left;
		}

		newDist = -1;
	}

	if (downAvailable) {
		if (map.UseAStarPathing()) {
			//TODO
		}
		else {
			newDist = map.GetDistance2(GetI(map), GetJ(map) + 1, targetI, targetJ);
		}

		if (newDist >= 0 && (bestDist < 0 || newDist < bestDist)) {
			bestDist = newDist;
			bestDir = down;
		}

		newDist = -1;
	}

	if (rightAvailable) {
		if (map.UseAStarPathing()) {
			//TODO
		}
		else {
			newDist = map.GetDistance2(GetI(map) + 1, GetJ(map), targetI, targetJ);
		}

		if (newDist >= 0 && (bestDist < 0 || newDist < bestDist)) {
			bestDist = newDist;
			bestDir = right;
		}

		newDist = -1;
	}

	return bestDir;
}