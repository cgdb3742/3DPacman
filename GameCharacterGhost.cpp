#include "GameCharacterGhost.h"

#include <cctype>

#include "GraphicData.h"
#include "ogl.h"
#include "GameMap.h"


GameCharacterGhost::GameCharacterGhost():
	GameCharacter()
{
	//dir = left;
	isGhost = true;

	speedMult = 0.8f;
	
	chaseMode = true;
	spawnDone = false;

	startDelay = 0.0f;

	vulnerableTimerMax = 15.0f;

	red = 0;
	green = 0;
	blue = 0;
	redDefault = 0;
	greenDefault = 0;
	blueDefault = 0;
	redVulnerable = 255;
	greenVulnerable = 255;
	blueVulnerable = 255;
	redVulnerableAlternate = 0;
	greenVulnerableAlternate = 0;
	blueVulnerableAlternate = 255;
}


GameCharacterGhost::~GameCharacterGhost()
{
}

void GameCharacterGhost::Update(float dt, GameMap & map)
{
	bodyAnimationTimer += dt;

	while (bodyAnimationTimer >= bodyAnimationCycleDuration) {
		bodyAnimationTimer -= bodyAnimationCycleDuration;
	}

	if (vulnerableTimer > 0.0f) {
		vulnerableTimer -= dt;

		if (vulnerableTimer < 0.0f) {
			vulnerableTimer = 0.0f;
		}

		vulnerableAnimationTimer += dt;

		while (vulnerableAnimationTimer >= vulnerableAnimationCycleDuration) {
			vulnerableAnimationTimer -= vulnerableAnimationCycleDuration;
		}
	}

	red = redDefault;
	green = greenDefault;
	blue = blueDefault;

	if (isGhost && vulnerableTimer > 0.0f) {
		float ratio = 0.75f;

		red = red * (1 - ratio) + (vulnerableAnimationTimer < vulnerableAnimationCycleDuration / 2.0f ? redVulnerable : redVulnerableAlternate) * ratio;
		green = green * (1 - ratio) + (vulnerableAnimationTimer < vulnerableAnimationCycleDuration / 2.0f ? greenVulnerable : greenVulnerableAlternate) * ratio;
		blue = blue * (1 - ratio) + (vulnerableAnimationTimer < vulnerableAnimationCycleDuration / 2.0f ? blueVulnerable : blueVulnerableAlternate) * ratio;
	}

	SetTarget(map, chaseMode);

	if (startDelay > 0.0f) {
		startDelay -= dt;

		if (startDelay < 0.0f) {
			dt = -startDelay;
		} else {
			dt = 0.0f;
		}
	}

	while (dt > 0.0f) {
		if (spawnDone) {
			GameCharacter::Update(dt, map);
			dt = 0.0f;
		}
		else {
			float ghostSpawnX = map.GetGhostSpawnX();
			float ghostSpawnY = map.GetGhostSpawnY();
			float ghostExitX = map.GetGhostExitX();
			float ghostExitY = map.GetGhostExitY();
			float realSpeed = moveSpeed * speedMult;

			bool moveX = true;
			float movePos = true;
			float maxDist = -1.0f;

			if (ghostSpawnX == ghostExitX) {
				if (x == ghostSpawnX) {
					moveX = false;
					movePos = (y < ghostExitY);
					maxDist = y - ghostExitY;
				} else {
					moveX = true;
					movePos = (x < ghostSpawnX);
					maxDist = x - ghostSpawnX;
				}
			} else {
				if (y == ghostSpawnY) {
					moveX = true;
					movePos = (x < ghostExitX);
					maxDist = x - ghostExitX;
				}
				else {
					moveX = false;
					movePos = (y < ghostSpawnY);
					maxDist = y - ghostSpawnY;
				}
			}

			if (movePos) {
				maxDist *= -1.0f;
			}

			float maxTime = maxDist / realSpeed;
			float remainingTime = 0.0f;

			if (maxTime <= dt) {
				remainingTime = dt - maxTime;

				dt = maxTime;
			}

			if (moveX) {
				x += dt * realSpeed * (movePos ? 1.0f : -1.0f);
			} else {
				y += dt * realSpeed * (movePos ? 1.0f : -1.0f);
			}

			if (x == ghostExitX && y == ghostExitY) {
				spawnDone = true;

				//FindNewDirection(map);
			}

			dt = remainingTime;
		}
	}
}

void GameCharacterGhost::ProcessInput(MY_INPUT * inp)
{
	GameCharacter::ProcessInput(inp);
}

void GameCharacterGhost::DrawCharacter(GraphicData & graphicData, float xpos, float ypos, float zpos)
{
	float bodyAngle = 360.0f * bodyAnimationTimer / bodyAnimationCycleDuration;

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
	}
	else {
		model = graphicData.GetPacmanModel();
	}

	glRotatef(bodyAngle, 0.0f, 0.0f, 1.0f);

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

	glRotatef(-bodyAngle, 0.0f, 0.0f, 1.0f);

	if (isGhost) {
		glDisable(GL_CULL_FACE);
	}

	glBegin(GL_TRIANGLES);

	DrawEyes(graphicData);

	glEnd();

	glPopMatrix();
}

void GameCharacterGhost::DrawCharacter(GraphicData & graphicData, float xoffset, float yoffset)
{
	GameCharacter::DrawCharacter(graphicData, xoffset, yoffset);
}

SpritePosition & GameCharacterGhost::GetSpritePosition(GraphicData & graphicData)
{
	return graphicData.GetGhostPosition(type);
}

GLenum GameCharacterGhost::GetLightID()
{
	switch (type) {
	case blinky: return GL_LIGHT2;
	case pinky: return GL_LIGHT3;
	case inky: return GL_LIGHT4;
	case clyde: return GL_LIGHT5;
	}

	return GL_LIGHT1;
}

void GameCharacterGhost::SetGhostType(GhostType newType, GameMap & map)
{
	type = newType;

	switch (type) {
	case blinky:
		redDefault = 255;
		greenDefault = 0;
		blueDefault = 0;
		targetI = map.GetI(-1);
		targetJ = map.GetJ(0);
		//primaryTargetKey = 'a';
		//secondaryTargetKey = 'q';
		primaryTargetKey = KEY_CODE_A;
		secondaryTargetKey = KEY_CODE_Q;
		break;
	case pinky:
		redDefault = 255;
		greenDefault = 128;
		blueDefault = 128;
		targetI = map.GetI(0);
		targetJ = map.GetJ(0);
		//primaryTargetKey = 'z';
		//secondaryTargetKey = 'w';
		primaryTargetKey = KEY_CODE_Z;
		secondaryTargetKey = KEY_CODE_W;
		break;
	case inky:
		redDefault = 0;
		greenDefault = 255;
		blueDefault = 255;
		targetI = map.GetI(-1);
		targetJ = map.GetJ(-1);
		//primaryTargetKey = 'e';
		//secondaryTargetKey = 'e';
		primaryTargetKey = KEY_CODE_E;
		secondaryTargetKey = KEY_CODE_E;
		break;
	case clyde:
		redDefault = 255;
		greenDefault = 128;
		blueDefault = 0;
		targetI = map.GetI(0);
		targetJ = map.GetJ(-1);
		//primaryTargetKey = 'r';
		//secondaryTargetKey = 'r';
		primaryTargetKey = KEY_CODE_R;
		secondaryTargetKey = KEY_CODE_R;
		break;
	}
}

void GameCharacterGhost::SetTarget(GameMap & map, bool isChaseMode)
{
	if (isChaseMode) {
		switch (type) {
		case blinky:
			targetI = map.GetPacman()->GetI(map);
			targetJ = map.GetPacman()->GetJ(map);
			break;
		case pinky:
			targetI = map.GetPacman()->GetI(map);
			targetJ = map.GetPacman()->GetJ(map);

			switch (map.GetPacman()->GetDirection()) {
			case up: targetJ -= 4; break;
			case left: targetI -= 4; break;
			case down: targetJ += 4; break;
			case right: targetI += 4; break;
			}

			break;
		case inky:
			//int di = map.GetPacman()->GetI(map) - map.GetGhost(blinky)->GetI(map);
			//int dj = map.GetPacman()->GetJ(map) - map.GetGhost(blinky)->GetJ(map);

			//switch (map.GetPacman()->GetDirection()) {
			//case up: dj -= 2; break;
			//case left: di -= 2; break;
			//case down: dj += 2; break;
			//case right: di += 2; break;
			//}

			//targetI = map.GetGhost(blinky)->GetI(map) + 2 * di;
			//targetJ = map.GetGhost(blinky)->GetJ(map) + 2 * dj;

			targetI = map.GetGhost(blinky)->GetI(map) + 2 * (map.GetPacman()->GetI(map) - map.GetGhost(blinky)->GetI(map));
			targetJ = map.GetGhost(blinky)->GetJ(map) + 2 * (map.GetPacman()->GetJ(map) - map.GetGhost(blinky)->GetJ(map));

			switch (map.GetPacman()->GetDirection()) {
			case up: targetJ -= 4; break;
			case left: targetI -= 4; break;
			case down: targetJ += 4; break;
			case right: targetI += 4; break;
			}

			break;
		case clyde:
			if (map.GetDistance0(this, map.GetPacman()) > 8) {
				targetI = map.GetPacman()->GetI(map);
				targetJ = map.GetPacman()->GetJ(map);
			}
			else {
				SetTarget(map, false);
			}
			break;
		}
	}
	else {
		switch (type) {
		case blinky:
			targetI = map.GetI(-1);
			targetJ = map.GetJ(0);
			break;
		case pinky:
			targetI = map.GetI(0);
			targetJ = map.GetJ(0);
			break;
		case inky:
			targetI = map.GetI(-1);
			targetJ = map.GetJ(-1);
			break;
		case clyde:
			targetI = map.GetI(0);
			targetJ = map.GetJ(-1);
			break;
		}
	}
}

void GameCharacterGhost::SetVulnerable(bool vulnerable)
{
	vulnerableTimer = vulnerable ? vulnerableTimerMax : 0.0f;
}

bool GameCharacterGhost::GetVulnerable()
{
	return vulnerableTimer > 0.0f;
}

void GameCharacterGhost::SetStartDelay(float value)
{
	startDelay = value;

	//if (value > 0.0f) {
	//	spawnDone = false;
	//}
}

void GameCharacterGhost::SetSpawnDone(bool value)
{
	spawnDone = value;
}

void GameCharacterGhost::SetDead(bool value, GameMap & map)
{
	GameCharacter::SetDead(value, map);

	if (value) {
		map.SpawnGhost(type, false);

		startDelay = 4.0f;
		vulnerableTimer = 0.0f;
	}
}
