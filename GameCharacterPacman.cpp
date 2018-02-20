#include "GameCharacterPacman.h"

#include <cmath>

#include "GameMap.h"
#include "GraphicData.h"
#include "ogl.h"


GameCharacterPacman::GameCharacterPacman() :
	GameCharacter()
{
	red = 255;
	green = 255;
	//dir = left;
	angle = 180.0;

	targetI = 14;
	targetJ = 18;

	playerControl = true;
	directControl = true;
}


GameCharacterPacman::~GameCharacterPacman()
{
}

void GameCharacterPacman::Update(float dt, GameMap & map)
{
	if (isDead) {
		deathAnimationTimer -= dt;

		if (deathAnimationTimer <= 0.0f) {
			//map.SpawnPacman();
			map.ResetAll();
		}

		return;
	}

	mouthAnimationTimer += dt;

	while (mouthAnimationTimer >= mouthAnimationCycleDuration) {
		mouthAnimationTimer -= mouthAnimationCycleDuration;
	}

	GameCharacter::Update(dt, map);
}

void GameCharacterPacman::ProcessInput(MY_INPUT * inp)
{
	GameCharacter::ProcessInput(inp);
}

void GameCharacterPacman::DrawCharacter(GraphicData & graphicData, float xpos, float ypos, float zpos)
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

	if (isDead) {
		//glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);

		float mouthAngleRatio = 1.0f - (deathAnimationTimer / deathAnimationDuration);

		if (mouthAngleRatio < 0.0f) {
			mouthAngleRatio = 0.0f;
		}

		if (mouthAngleRatio > 1.0f) {
			mouthAngleRatio = 1.0f;
		}

		DrawPacmanModel(graphicData, 90.0f, 360.0f * mouthAngleRatio);
	}
	else {
		float mouthAngleRatio = mouthAnimationTimer / mouthAnimationCycleDuration * 2.0f;

		if (mouthAngleRatio > 1.0f) {
			mouthAngleRatio = 2.0f - mouthAngleRatio;
		}

		DrawPacmanModel(graphicData, 0.0f, 60.0f * mouthAngleRatio);
	}

	if (isGhost) {
		glDisable(GL_CULL_FACE);
	}

	glBegin(GL_TRIANGLES);

	if (!isDead) {
		DrawEyes(graphicData);
	}

	glEnd();

	glPopMatrix();
}

void GameCharacterPacman::DrawCharacter(GraphicData & graphicData, float xoffset, float yoffset)
{
	GameCharacter::DrawCharacter(graphicData, xoffset, yoffset);
}

void GameCharacterPacman::DrawPacmanModel(GraphicData & graphicData, float mouthPosAngle, float mouthOpenAngle)
{
	float pi = 3.14159265358979f;

	float r = 0.5f;

	int isize = 24;
	int jsize = 24;

	float phi0 = mouthPosAngle * pi / 180.0f;
	float phimin = phi0 + mouthOpenAngle * pi / 360.0f;
	float phimax = phi0 - mouthOpenAngle * pi / 360.0f;

	while (phimin < 0.0f) {
		phimin += 2.0f * pi;
	}
	while (phimin >= 2.0f * pi) {
		phimin -= 2.0f * pi;
	}
	while (phimax < phimin) {
		phimax += 2.0f * pi;
	}
	while (phimax >= phimax + 2.0f * pi) {
		phimax -= 2.0f * pi;
	}


	glBegin(GL_TRIANGLES);

	for (int i = 0; i < isize; i++) {
		for (int j = 0; j < jsize; j++) {
			int ip = i + 1;
			int jp = j + 1;

			float theta = pi * (float)i / (float)isize;
			float thetap = pi * (float)ip / (float)isize;
			float phi = phimin + (phimax - phimin) * (float)j / (float)jsize;
			float phip = phimin + (phimax - phimin) * (float)jp / (float)jsize;

			float x1 = r * sinf(theta) * cosf(phi);
			float x2 = r * sinf(theta) * cosf(phip);
			float x3 = r * sinf(thetap) * cosf(phi);
			float x4 = r * sinf(thetap) * cosf(phip);
			float y1 = r * sinf(theta) * sinf(phi);
			float y2 = r * sinf(theta) * sinf(phip);
			float y3 = r * sinf(thetap) * sinf(phi);
			float y4 = r * sinf(thetap) * sinf(phip);
			float z12 = r * cosf(theta);
			float z34 = r * cosf(thetap);

			glVertex3f(x1, z12, y1);
			glVertex3f(x2, z12, y2);
			glVertex3f(x3, z34, y3);

			glVertex3f(x2, z12, y2);
			glVertex3f(x3, z34, y3);
			glVertex3f(x4, z34, y4);

			if (j == 0) {
				float x0 = 0.0f;
				float y0 = 0.0f;
				float z0 = r;

				glVertex3f(x1, z12, y1);
				glVertex3f(x3, z34, y3);
				glVertex3f(x0, z0, y0);
			} else if (jp == jsize) {
				float x0 = 0.0f;
				float y0 = 0.0f;
				float z0 = r;

				glVertex3f(x0, z0, y0);
				glVertex3f(x2, z12, y2);
				glVertex3f(x4, z34, y4);
			}
		}
	}

	glEnd();
}

void GameCharacterPacman::SetDead(bool value, GameMap & map)
{
	if (value) {
		deathAnimationTimer = deathAnimationDuration;

		mouthAnimationTimer = 0.0f;
	}

	GameCharacter::SetDead(value, map);
}
