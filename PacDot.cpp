#include "PacDot.h"

#include "GameMap.h"
#include "GraphicData.h"

PacDot::PacDot()
{
}

PacDot::PacDot(int posi, int posj, bool superGum)
{
	i = posi;
	j = posj;
	isSuperGum = superGum;
}


PacDot::~PacDot()
{
}

void PacDot::SetPosition(int ivalue, int jvalue)
{
	i = ivalue;
	j = jvalue;
}

int PacDot::GetI()
{
	return i;
}

int PacDot::GetJ()
{
	return j;
}

float PacDot::GetX(GameMap & map)
{
	return map.GetCenterX(i);
}

float PacDot::GetY(GameMap & map)
{
	return map.GetCenterY(j);
}

void PacDot::SetActive(bool newValue)
{
	active = newValue;

	if (active) {
		eaten = false;
	}
}

bool PacDot::IsEaten()
{
	return eaten;
}

bool PacDot::CanBeEaten()
{
	return !eaten && active && activationTimer >= maxActivationTimer;
}

void PacDot::EatDot()
{
	eaten = true;
}

void PacDot::Update(float dt, GameMap & map)
{
	if (active && activationTimer < maxActivationTimer) {
		activationTimer += dt;

		if (activationTimer > maxActivationTimer) {
			activationTimer = maxActivationTimer;
		}
	}
	else if (!active && activationTimer > 0.0f) {
		activationTimer -= dt;

		if (activationTimer < 0.0f) {
			activationTimer = 0.0f;
		}
	}
}

void PacDot::DrawDot(GraphicData & graphicData, GameMap & map)
{
	DrawDot(graphicData, GetX(map), GetY(map), 0.0f);
}

void PacDot::DrawDot(GraphicData & graphicData, GameMap & map, float xoffset, float yoffset)
{
	float zoffset = 0.75f - 0.75f*(activationTimer / maxActivationTimer);

	DrawDot(graphicData, GetX(map) + xoffset, GetY(map) + yoffset, zoffset);
}

void PacDot::DrawDot(GraphicData & graphicData, float xpos, float ypos, float zpos)
{
	if (eaten) {
		return;
	}

	float dotScale = isSuperGum ? 0.5f : 0.25f;

	glPushMatrix();

	//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(xpos, ypos, zpos);
	glScalef(dotScale, dotScale, -dotScale);

	Model3D* model = graphicData.GetDotModel();

	glBegin(GL_TRIANGLES);

	for (int k = 0; k < model->nb_triangles; k++) {
		//glColor3f(red, green, blue);
		glVertex3f(model->points[model->faces[k].a].x, model->points[model->faces[k].a].y, model->points[model->faces[k].a].z);

		//glColor3f(red, green, blue);
		glVertex3f(model->points[model->faces[k].b].x, model->points[model->faces[k].b].y, model->points[model->faces[k].b].z);

		//glColor3f(red, green, blue);
		glVertex3f(model->points[model->faces[k].c].x, model->points[model->faces[k].c].y, model->points[model->faces[k].c].z);
	}

	glEnd();

	glPopMatrix();
}
