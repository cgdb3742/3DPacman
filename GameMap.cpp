#include "GameMap.h"

#include <cmath>
#include "GameControl.h"
#include "GraphicData.h"

GameMap::GameMap()
{
	dataMap = new Image();
	colorMap = new Image();
	dotMap = new Image();
	colorTexture = new Texture();

	pacman = new GameCharacterPacman();
	ghostBlinky = new GameCharacterGhost();
	ghostPinky = new GameCharacterGhost();
	ghostInky = new GameCharacterGhost();
	ghostClyde = new GameCharacterGhost();

	pacDots = std::list<PacDot*>();
	superGums = std::list<PacDot*>();

	ghostBlinky->SetGhostType(blinky, *this);
	ghostPinky->SetGhostType(pinky, *this);
	ghostInky->SetGhostType(inky, *this);
	ghostClyde->SetGhostType(clyde, *this);
	
	res = 1.0f;
	repeatDist = 1;
	LoadMaps("Pacman_base_data.tga", "Pacman_base_dot.tga", "Pacman_base_color.tga");
	//LoadMaps("Pacman_google_data.tga", "Pacman_google_dot.tga", "Pacman_google_color.tga");

	ResetAll();

	//ghostBlinky->SetVulnerable(true);

	//ActivatePacDots(false);

	alternateTimer = 0.0f;
	maxAlternateTimer = 0.2f;
	alternate = false;

	//pacman->SetPosition(13, 14, 20, 20, *this);
	//ghostBlinky->SetPosition(13, 14, 14, 14, *this);
	//ghostPinky->SetPosition(13, 14, 17, 17, *this);
	//ghostInky->SetPosition(11, 12, 17, 17, *this);
	//ghostClyde->SetPosition(15, 16, 17, 17, *this);

	currentPhase = freeroam;
	freeRoamDuration = 5.0f;
	freeRoamTimer = freeRoamDuration;
}


GameMap::~GameMap()
{
	if (dataMap != nullptr) {
		delete dataMap;
	}
	if (colorMap != nullptr) {
		delete colorMap;
	}
	if (colorTexture != nullptr) {
		delete colorTexture;
	}
	if (pacman != nullptr) {
		delete pacman;
	}
	if (ghostBlinky != nullptr) {
		delete ghostBlinky;
	}
	if (ghostPinky != nullptr) {
		delete ghostPinky;
	}
	if (ghostInky != nullptr) {
		delete ghostInky;
	}
	if (ghostClyde != nullptr) {
		delete ghostClyde;
	}
}

void GameMap::Update(float dt)
{
	if (freeRoamTimer > 0.0f) {
		freeRoamTimer -= dt;

		if (freeRoamTimer < 0.0f) {
			freeRoamTimer = 0.0f;
		}
	}

	if (IsCurrentGamePhaseDone()) {
		StartNextGamePhase();
	}

	alternateTimer += dt;

	if (alternateTimer >= maxAlternateTimer) {
		alternateTimer -= maxAlternateTimer;
		alternate = !alternate;
	}

	if (pacman != nullptr) {
		pacman->Update(dt, *this);
	}
	if (ghostBlinky != nullptr) {
		ghostBlinky->Update(dt, *this);
	}
	if (ghostPinky != nullptr) {
		ghostPinky->Update(dt, *this);
	}
	if (ghostInky != nullptr) {
		ghostInky->Update(dt, *this);
	}
	if (ghostClyde != nullptr) {
		ghostClyde->Update(dt, *this);
	}

	for (auto&& child : pacDots) {
		child->Update(dt, *this);
	}
	for (auto&& child : superGums) {
		child->Update(dt, *this);
	}

	ComputeContacts();
}

//TODO Multiple inputs ?
void GameMap::ProcessInput(MY_INPUT * inp)
{
	if (pacman != nullptr) {
		pacman->ProcessInput(inp);
	}
	if (ghostBlinky != nullptr) {
		ghostBlinky->ProcessInput(inp);
	}
	if (ghostPinky != nullptr) {
		ghostPinky->ProcessInput(inp);
	}
	if (ghostInky != nullptr) {
		ghostInky->ProcessInput(inp);
	}
	if (ghostClyde != nullptr) {
		ghostClyde->ProcessInput(inp);
	}
}

void GameMap::Draw(GraphicData& graphicData)
{
	int k = repeatHorizontal ? -repeatDist : 0;
	bool kdone = false;
	int l = repeatVertical ? -repeatDist : 0;
	bool ldone = false;

	//Draw Map
	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			float x = GetCenterX((int)i);
			float y = GetCenterY((int)j);

			float red = GetColorRed((int)i, (int)j) / 255.0f;
			float green = GetColorGreen((int)i, (int)j) / 255.0f;
			float blue = GetColorBlue((int)i, (int)j) / 255.0f;

			GLfloat mat_specular[] = { red / 2.0f + 0.5f, green / 2.0f + 0.5f, blue / 4.0f + 0.5f, 1.0f };
			GLfloat mat_shininess[] = { 50.0 };

			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

			k = repeatHorizontal ? -repeatDist : 0;
			kdone = false;

			while (!kdone) {
				l = repeatVertical ? -repeatDist : 0;
				ldone = false;

				while (!ldone) {
					if (GetDataType(i, j) == ghostexit) {
						DrawGate(graphicData, x + k * (int)width, y + l * (int)height, 0.0f, ghostExitOffset);

						int ifloor = i;
						int jfloor = j;

						switch (ghostSpawnDir) {
						case up: jfloor--; break;
						case left: ifloor--; break;
						case down: jfloor++; break;
						case right: ifloor++; break;
						}

						red = GetColorRed((int)ifloor, (int)jfloor) / 255.0f;
						green = GetColorGreen((int)ifloor, (int)jfloor) / 255.0f;
						blue = GetColorBlue((int)ifloor, (int)jfloor) / 255.0f;

						GLfloat mat_specular_floor[] = { red / 2.0f + 0.5f, green / 2.0f + 0.5f, blue / 4.0f + 0.5f, 1.0f };

						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_specular_floor);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_floor);

						DrawFloor(graphicData, x + k * (int)width, y + l * (int)height, 0.0f);

						glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_specular);
						glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
					}
					else if (IsWall(i, j)) {
						DrawWall(graphicData, x + k * (int)width, y + l * (int)height, 0.0f);
					}
					else {
						DrawFloor(graphicData, x + k * (int)width, y + l * (int)height, 0.0f);
						//DrawDot(graphicData, x + k * (int)width, y + l * (int)height, 0.0f, false);
					}

					ldone = !repeatVertical || l >= repeatDist;
					l++;
				}

				kdone = !repeatHorizontal || k >= repeatDist;
				k++;
			}
		}
	}

	//Draw pacdots
	GLfloat mat_specular_pacdot[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat mat_shininess_pacdot[] = { 50.0 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_specular_pacdot);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular_pacdot);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess_pacdot);

	k = repeatHorizontal ? -repeatDist : 0;
	kdone = false;

	while (!kdone) {
		l = repeatVertical ? -repeatDist : 0;
		ldone = false;

		while (!ldone) {
			for (auto&& child : pacDots) {
				child->DrawDot(graphicData, *this, k*GetXSize(), l*GetYSize());
			}
			for (auto&& child : superGums) {
				child->DrawDot(graphicData, *this, k*GetXSize(), l*GetYSize());
			}
			ldone = !repeatVertical || l >= repeatDist;
			l++;
		}

		kdone = !repeatHorizontal || k >= repeatDist;
		k++;
	}

	if (currentPhase == findsupergum) {
		if (pacman != nullptr && currentSuperGum != nullptr) {
			DrawArrow(graphicData, pacman->GetX(), pacman->GetY(), -1.0f, currentSuperGum->GetX(*this), currentSuperGum->GetY(*this));
		}
	}

	//Draw Characters
	k = repeatHorizontal ? -repeatDist : 0;
	kdone = false;

	while (!kdone) {
		l = repeatVertical ? -repeatDist : 0;
		ldone = false;

		while (!ldone) {
			if (k == 0 && l == 0) {
				if (pacman != nullptr) {
					pacman->Draw(graphicData);
				}
				if (ghostBlinky != nullptr) {
					ghostBlinky->Draw(graphicData);
				}
				if (ghostPinky != nullptr) {
					ghostPinky->Draw(graphicData);
				}
				if (ghostInky != nullptr) {
					ghostInky->Draw(graphicData);
				}
				if (ghostClyde != nullptr) {
					ghostClyde->Draw(graphicData);
				}
			} else {
				if (pacman != nullptr) {
					pacman->DrawCharacter(graphicData, k * (int)width, l * (int)height);
				}
				if (ghostBlinky != nullptr) {
					ghostBlinky->DrawCharacter(graphicData, k * (int)width, l * (int)height);
				}
				if (ghostPinky != nullptr) {
					ghostPinky->DrawCharacter(graphicData, k * (int)width, l * (int)height);
				}
				if (ghostInky != nullptr) {
					ghostInky->DrawCharacter(graphicData, k * (int)width, l * (int)height);
				}
				if (ghostClyde != nullptr) {
					ghostClyde->DrawCharacter(graphicData, k * (int)width, l * (int)height);
				}
			}

			ldone = !repeatVertical || l >= repeatDist;
			l++;
		}

		kdone = !repeatHorizontal || k >= repeatDist;
		k++;
	}
}

void GameMap::DrawWall(GraphicData & graphicData, float xpos, float ypos, float zpos)
{
	glPushMatrix();

	//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(xpos, ypos, zpos);
	glScalef(1.0f, 1.0f, -1.0f);

	Model3D* model = graphicData.GetWallModel();

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

void GameMap::DrawGate(GraphicData & graphicData, float xpos, float ypos, float zpos, Direction dir)
{
	glPushMatrix();

	glRotatef(90.0f *  (int)dir, 0.0f, 0.0f, 1.0f);	
	glTranslatef(xpos, ypos, zpos);
	glScalef(1.0f, 1.0f, -1.0f);

	Model3D* model = graphicData.GetGateModel();

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

void GameMap::DrawFloor(GraphicData & graphicData, float xpos, float ypos, float zpos)
{
	glPushMatrix();

	//glRotatef(0.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(xpos, ypos, zpos);
	glScalef(1.0f, 1.0f, -1.0f);

	Model3D* model = graphicData.GetWallModel();

	glBegin(GL_TRIANGLES);


	//glColor3f(red, green, blue);
	glVertex3f(-res / 2.0f, -res / 2.0f, -res / 2.0f);
	//glColor3f(red, green, blue);
	glVertex3f(-res / 2.0f, res / 2.0f, -res / 2.0f);
	//glColor3f(red, green, blue);
	glVertex3f(res / 2.0f, -res / 2.0f, -res / 2.0f);

	//glColor3f(red, green, blue);
	glVertex3f(res / 2.0f, res / 2.0f, -res / 2.0f);
	//glColor3f(red, green, blue);
	glVertex3f(res / 2.0f, -res / 2.0f, -res / 2.0f);
	//glColor3f(red, green, blue);
	glVertex3f(-res / 2.0f, res / 2.0f, -res / 2.0f);

	glEnd();

	glPopMatrix();
}

void GameMap::DrawArrow(GraphicData & graphicData, float xpos, float ypos, float zpos, float xtarget, float ytarget)
{
	float pi = 3.14159265358979f;
	float arrowAngle = atan2(ytarget - ypos, xtarget - xpos);

	glPushMatrix();

	glTranslatef(xpos, ypos, zpos);
	glRotatef(arrowAngle * 180.0f / pi, 0.0f, 0.0f, 1.0f);
	glScalef(1.0f, 1.0f, -1.0f);

	Model3D* model = graphicData.GetArrowModel();

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

void GameMap::DrawMap(GraphicData& graphicData, float leftCorner, float upCorner, float sizex, float sizey, bool stretch)
{
	float sizeRatio = sizex / sizey;
	float mapRatio = (float)dataMap->lenx / (float)dataMap->leny;

	if (sizeRatio > mapRatio && !stretch) {
		leftCorner += (sizex - sizey * mapRatio) / 2.0f;
		sizex = sizey * mapRatio;
	}
	if (sizeRatio < mapRatio && !stretch) {
		upCorner += (sizey - sizex / mapRatio) / 2.0f;
		sizey = sizex / mapRatio;
	}

	graphicData.DrawImage(colorTexture, leftCorner, upCorner, sizex, sizey, 0.0f);

	//pacman->DrawMap(graphicData, *this, leftCorner, upCorner, sizex, size y);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.8f);

	

	if (pacman != nullptr) {
		pacman->DrawMap(graphicData, *this, leftCorner, upCorner, sizex, sizey);
	}
	if (ghostBlinky != nullptr) {
		ghostBlinky->DrawMap(graphicData, *this, leftCorner, upCorner, sizex, sizey);
	}
	if (ghostPinky != nullptr) {
		ghostPinky->DrawMap(graphicData, *this, leftCorner, upCorner, sizex, sizey);
	}
	if (ghostInky != nullptr) {
		ghostInky->DrawMap(graphicData, *this, leftCorner, upCorner, sizex, sizey);
	}
	if (ghostClyde != nullptr) {
		ghostClyde->DrawMap(graphicData, *this, leftCorner, upCorner, sizex, sizey);
	}

	glDisable(GL_ALPHA_TEST);
}

void GameMap::SetCamera(bool firstPerson)
{
	GameCharacter* focusChar = GetPacman();

	//focusChar = GetGhost(pinky);

	focusChar->SetCamera(firstPerson);
}

void GameMap::ComputeContacts()
{
	if (pacman != nullptr && ghostBlinky != nullptr) {
		if (!pacman->GetDead() && !ghostBlinky->GetDead() && Contact(pacman, ghostBlinky)) {
			if (ghostBlinky->GetVulnerable()) {
				ghostBlinky->SetDead(true, *this);
			}
			else {
				pacman->SetDead(true, *this);
			}
		}
	}
	if (pacman != nullptr && ghostPinky != nullptr) {
		if (!pacman->GetDead() && !ghostPinky->GetDead() && Contact(pacman, ghostPinky)) {
			if (ghostPinky->GetVulnerable()) {
				ghostPinky->SetDead(true, *this);
			}
			else {
				pacman->SetDead(true, *this);
			}
		}
	}
	if (pacman != nullptr && ghostInky != nullptr) {
		if (!pacman->GetDead() && !ghostInky->GetDead() && Contact(pacman, ghostInky)) {
			if (ghostInky->GetVulnerable()) {
				ghostInky->SetDead(true, *this);
			}
			else {
				pacman->SetDead(true, *this);
			}
		}
	}
	if (pacman != nullptr && ghostClyde != nullptr) {
		if (!pacman->GetDead() && !ghostClyde->GetDead() && Contact(pacman, ghostClyde)) {
			if (ghostClyde->GetVulnerable()) {
				ghostClyde->SetDead(true, *this);
			}
			else {
				pacman->SetDead(true, *this);
			}
		}
	}

	for (auto&& child : pacDots) {
		if (child->CanBeEaten()) {
			if (Contact(pacman, child)) {
				child->EatDot();
				eatenDots++;
			}
		}
	}
	for (auto&& child : superGums) {
		if (child->CanBeEaten()) {
			if (Contact(pacman, child)) {
				child->EatDot();
			}
		}
	}
}

void GameMap::ActivatePacDots(bool activateSuperGums)
{
	if (activateSuperGums) {
		for (auto&& child : superGums) {
			child->SetActive(true);
		}
	}
	else {
		for (auto&& child : pacDots) {
			child->SetActive(true);
		}
		eatenDots = 0;
	}
}

void GameMap::DeactivatePacDots(bool deactivateSuperGums)
{
	if (deactivateSuperGums) {
		for (auto&& child : superGums) {
			child->SetActive(false);
		}
	}
	else {
		for (auto&& child : pacDots) {
			child->SetActive(false);
		}
	}
}

void GameMap::ActivateRandomPacDot(bool activateSuperGum)
{
	if (activateSuperGum) {
		int id = Enums::GetRandomNumber(0, superGums.size() - 1);

		for (auto&& child : superGums) {
			if (id == 0) {
				child->SetActive(true);
				currentSuperGum = child;
			}
			id--;
		}
	}
	else {
		int id = Enums::GetRandomNumber(0, pacDots.size() - 1);

		for (auto&& child : pacDots) {
			if (id == 0) {
				child->SetActive(true);
				currentSuperGum = child;
			}
			id--;
		}
	}
}

void GameMap::SetGhostsVulnerable(bool vulnerable)
{
	if (ghostBlinky != nullptr) {
		ghostBlinky->SetVulnerable(vulnerable);
	}
	if (ghostPinky != nullptr) {
		ghostPinky->SetVulnerable(vulnerable);
	}
	if (ghostInky != nullptr) {
		ghostInky->SetVulnerable(vulnerable);
	}
	if (ghostClyde != nullptr) {
		ghostClyde->SetVulnerable(vulnerable);
	}
}

bool GameMap::LoadMaps(char * newData, char * newDot, char * newColor)
{
	if (dataMap != nullptr) {
		delete dataMap;
	}
	if (dotMap != nullptr) {
		delete dotMap;
	}
	if (colorMap != nullptr) {
		delete colorMap;
	}
	if (colorTexture != nullptr) {
		delete colorTexture;
	}

	dataMap = new Image();
	dotMap = new Image();
	colorMap = new Image();
	colorTexture = new Texture();

	dataMap->load_tga(newData);
	dotMap->load_tga(newDot);
	colorMap->load_tga(newColor);

	colorTexture->load_texture(newColor, NULL);

	glGenTextures(1, colorTexture->OpenGL_ID);                // crée un "nom" de texture (un identifiant associé à la texture)
													 //glBindTexture(GL_TEXTURE_2D, tex->OpenGL_ID[0]);    // et on active ce "nom" comme texture courante (définie plus bas)

	width = dataMap->lenx;
	height = dataMap->leny;

	repeatHorizontal = false;
	repeatVertical = false;

	for (unsigned int i = 0; i < width; i++) {
		if (!IsWall(i, 0)) {
			repeatVertical = true;
		}
	}

	for (unsigned int j = 0; j < height; j++) {
		if (!IsWall(0, j)) {
			repeatHorizontal = true;
		}
	}

	bool pacmanStartFound = false;
	bool ghostSpawnFound = false;
	bool ghostExitFound = false;

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			DataType spaceType = GetDataType(i, j);

			if (spaceType == pacmanstart && !pacmanStartFound) {
				pacmanStartI = i;
				pacmanStartJ = j;

				pacmanStartOffset = down;
				pacmanStartFound = true;

				if (i < width - 1) {
					if (GetDataType(i + 1, j) == pacmanstart) {
						pacmanStartOffset = right;
					}
				}
			}
			if (spaceType == ghostspawn && !ghostSpawnFound) {
				ghostSpawnI = i;
				ghostSpawnJ = j;

				ghostSpawnOffset = down;
				ghostSpawnFound = true;

				if (i < width - 1) {
					if (GetDataType(i + 1, j) == ghostspawn) {
						ghostSpawnOffset = right;
					}
				}
			}
			if (spaceType == ghostexit && !ghostExitFound) {
				ghostExitI = i;
				ghostExitJ = j;

				ghostExitOffset = down;
				ghostExitFound = true;

				if (i < width - 1) {
					if (GetDataType(i + 1, j) == ghostexit) {
						ghostExitOffset = right;
					}
				}
			}
		}
	}

	if (ghostSpawnI == ghostExitI) {
		if (ghostSpawnJ > ghostExitJ) {
			ghostSpawnDir = up;
		} else {
			ghostSpawnDir = down;
		}
	} else {
		if (ghostSpawnI > ghostExitI) {
			ghostSpawnDir = left;
		}
		else {
			ghostSpawnDir = right;
		}
	}

	ClearPacDots();
	ClearSuperGums();

	for (unsigned int i = 0; i < width; i++) {
		for (unsigned int j = 0; j < height; j++) {
			if (HasSuperGum(i, j)) {
				superGums.push_back(new PacDot(i, j, true));
				pacDots.push_back(new PacDot(i, j, false));
			} else if (HasPacDot(i, j)) {
				pacDots.push_back(new PacDot(i, j, false));
			}
		}
	}

	return true;
}

unsigned int GameMap::GetDataRed(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return (unsigned int)dataMap->data[3 * (i + width*j)];
}

unsigned int GameMap::GetDataGreen(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return (unsigned int)dataMap->data[3 * (i + width*j) + 1];
}

unsigned int GameMap::GetDataBlue(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return (unsigned int)dataMap->data[3 * (i + width*j) + 2];
}

unsigned int GameMap::GetColorRed(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return (unsigned int)colorMap->data[3 * (i + width*j)];
}

unsigned int GameMap::GetColorGreen(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return (unsigned int)colorMap->data[3 * (i + width*j) + 1];
}

unsigned int GameMap::GetColorBlue(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return (unsigned int)colorMap->data[3 * (i + width*j) + 2];
}

unsigned int GameMap::GetDataTypeRed(DataType dataType)
{
	switch (dataType) {
	case hole: return 0;
	case wall: return 0;
	case path: return 255;
	case cross: return 0;
	case noupcross: return 255;
	case ghostspawn: return 255;
	case ghostexit: return 255;
	case pacmanstart: return 0;
	case tunnel: return 128;
	default: return 0;
	}

	return 0;
}

unsigned int GameMap::GetDataTypeGreen(DataType dataType)
{
	switch (dataType) {
	case hole: return 0;
	case wall: return 0;
	case path: return 255;
	case cross: return 255;
	case noupcross: return 255;
	case ghostspawn: return 0;
	case ghostexit: return 0;
	case pacmanstart: return 255;
	case tunnel: return 128;
	default: return 0;
	}

	return 0;
}

unsigned int GameMap::GetDataTypeBlue(DataType dataType)
{
	switch (dataType) {
	case hole: return 0;
	case wall: return 255;
	case path: return 255;
	case cross: return 0;
	case noupcross: return 0;
	case ghostspawn: return 0;
	case ghostexit: return 255;
	case pacmanstart: return 255;
	case tunnel: return 128;
	default: return 0;
	}

	return 0;
}

DataType GameMap::GetDataType(unsigned int redValue, unsigned int greenValue, unsigned int blueValue)
{
	for (int i = 0; i < size; i++) {
		DataType testedType = (DataType)i;

		if (redValue == GetDataTypeRed(testedType) && greenValue == GetDataTypeGreen(testedType) && blueValue == GetDataTypeBlue(testedType)) {
			return testedType;
		}
	}

	return hole;
}

DataType GameMap::GetDataType(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	return GetDataType(GetDataRed(i, j), GetDataGreen(i, j), GetDataBlue(i, j));
}

void GameMap::ClearPacDots()
{
	for (auto&& child : pacDots) {
		delete child;
	}
	pacDots.clear();
}

void GameMap::ClearSuperGums()
{
	for (auto&& child : superGums) {
		delete child;
	}
	superGums.clear();
}

bool GameMap::HasPacDot(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	int red = (unsigned int)dotMap->data[3 * (i + width*j)];
	int green = (unsigned int)dotMap->data[3 * (i + width*j) + 1];
	int blue = (unsigned int)dotMap->data[3 * (i + width*j) + 2];

	return ((red == 255 && green == 255 && blue == 255) || (red == 255 && green == 255 && blue == 0));
}

bool GameMap::HasSuperGum(int i, int j)
{
	while (i < 0) {
		i += width;
	}
	while (i >= (int)width) {
		i -= width;
	}
	while (j < 0) {
		j += height;
	}
	while (j >= (int)height) {
		j -= height;
	}

	int red = (unsigned int)dotMap->data[3 * (i + width*j)];
	int green = (unsigned int)dotMap->data[3 * (i + width*j) + 1];
	int blue = (unsigned int)dotMap->data[3 * (i + width*j) + 2];

	return (red == 255 && green == 255 && blue == 0);
}

int GameMap::GetI(int i)
{
	int pos = i;

	while (pos < 0) {
		pos += width;
	}

	while (pos >= (int)width) {
		pos -= width;
	}

	return pos;
}

int GameMap::GetJ(int j)
{
	int pos = j;

	while (pos < 0) {
		pos += height;
	}

	while (pos >= (int)height) {
		pos -= height;
	}

	return pos;
}

int GameMap::GetI(float x)
{
	float ratio = x / res;

	while (ratio < 0) {
		ratio += width;
	}

	while (ratio >= (int)width) {
		ratio -= width;
	}

	return (int)floorf(ratio);
}

int GameMap::GetJ(float y)
{
	float ratio = y / res;

	while (ratio < 0) {
		ratio += height;
	}

	while (ratio >= (int)height) {
		ratio -= height;
	}

	return (int)floorf(ratio);
}

float GameMap::GetCenterX(int i)
{
	int pos = i;

	while (pos < 0) {
		pos += width;
	}

	while (pos >= (int)width) {
		pos -= width;
	}

	return pos * res + res / 2.0f;
}

float GameMap::GetCenterY(int j)
{
	int pos = j;

	while (pos < 0) {
		pos += height;
	}

	while (pos >= (int)height) {
		pos -= height;
	}

	return pos * res + res / 2.0f;
}

float GameMap::GetCenterX(float x)
{
	return GetCenterX(GetI(x));
}

float GameMap::GetCenterY(float y)
{
	return GetCenterY(GetJ(y));
}

float GameMap::GetCenterX(int i1, int i2)
{
	return (GetCenterX(i1) + GetCenterX(i2)) / 2.0f;
}

float GameMap::GetCenterY(int j1, int j2)
{
	return (GetCenterY(j1) + GetCenterY(j2)) / 2.0f;
}

float GameMap::GetPacmanStartX()
{
	float toReturn = GetCenterX(pacmanStartI);

	if (pacmanStartOffset == left) {
		toReturn -= res / 2.0f;
	}
	else if (pacmanStartOffset == right) {
		toReturn += res / 2.0f;
	}

	return toReturn;
}

float GameMap::GetPacmanStartY()
{
	float toReturn = GetCenterY(pacmanStartJ);

	if (pacmanStartOffset == up) {
		toReturn -= res / 2.0f;
	}
	else if (pacmanStartOffset == down) {
		toReturn += res / 2.0f;
	}

	return toReturn;
}

float GameMap::GetGhostSpawnX()
{
	float toReturn = GetCenterX(ghostSpawnI);

	if (ghostSpawnOffset == left) {
		toReturn -= res / 2.0f;
	}
	else if (ghostSpawnOffset == right) {
		toReturn += res / 2.0f;
	}

	return toReturn;
}

float GameMap::GetGhostSpawnY()
{
	float toReturn = GetCenterY(ghostSpawnJ);

	if (ghostSpawnOffset == up) {
		toReturn -= res / 2.0f;
	}
	else if (ghostSpawnOffset == down) {
		toReturn += res / 2.0f;
	}

	return toReturn;
}

float GameMap::GetGhostExitX()
{
	float toReturn = GetCenterX(ghostExitI);

	if (ghostExitOffset == left) {
		toReturn -= res / 2.0f;
	}
	else if (ghostExitOffset == right) {
		toReturn += res / 2.0f;
	}

	return toReturn;
}

float GameMap::GetGhostExitY()
{
	float toReturn = GetCenterY(ghostExitJ);

	if (ghostExitOffset == up) {
		toReturn -= res / 2.0f;
	} else if (ghostExitOffset == down) {
		toReturn += res / 2.0f;
	}

	return toReturn;
}

float GameMap::WrapX(float x, Direction moveDir)
{
	float toReturn = x;

	while (moveDir == left && toReturn < res * 3.0f / 4.0f) {
		toReturn += width * res;
	}
	while (moveDir == right && toReturn >= width * res - res * 3.0f / 4.0f) {
		toReturn -= width * res;
	}

	return toReturn;
}

float GameMap::WrapY(float y, Direction moveDir)
{
	float toReturn = y;

	while (moveDir == up && toReturn < res * 3.0f / 4.0f) {
		toReturn += height * res;
	}
	while (moveDir == down && toReturn >= height * res - res * 3.0f / 4.0f) {
		toReturn -= height * res;
	}

	return toReturn;
}

float GameMap::GetXSize()
{
	return width * res;
}

float GameMap::GetYSize()
{
	return height * res;
}

float GameMap::GetRes()
{
	return res;
}

bool GameMap::GetAlternate()
{
	return alternate;
}

bool GameMap::IsWall(int i, int j)
{
	DataType spaceType = GetDataType(i, j);

	return (spaceType == wall || spaceType == ghostexit || spaceType == hole);
	//return (spaceType == wall || spaceType == ghostexit);
}

bool GameMap::CanMoveTo(int i, int j)
{
	DataType spaceType = GetDataType(i, j);

	return (spaceType == path || spaceType == cross || spaceType == noupcross || spaceType == pacmanstart || spaceType == tunnel);
}

bool GameMap::CanMoveUp(int i, int j, bool isGhost)
{
	DataType spaceType = GetDataType(i, j);

	if (spaceType == noupcross && isGhost && useNoUpCrosses) {
		return false;
	}

	return CanMoveTo(i, j - 1);
}

bool GameMap::CanMoveLeft(int i, int j, bool isGhost)
{
	return CanMoveTo(i - 1, j);
}

bool GameMap::CanMoveDown(int i, int j, bool isGhost)
{
	return CanMoveTo(i, j + 1);
}

bool GameMap::CanMoveRight(int i, int j, bool isGhost)
{
	return CanMoveTo(i + 1, j);
}

GameCharacterPacman * GameMap::GetPacman()
{
	return pacman;
}

GameCharacterGhost * GameMap::GetGhost(GhostType type)
{
	switch (type) {
	case blinky: return ghostBlinky;
	case pinky: return ghostPinky;
	case inky: return ghostInky;
	case clyde: return ghostClyde;
	}

	return nullptr;
}

void GameMap::ResetAll()
{
	if (pacman != nullptr) {
		SpawnPacman();
	}
	if (ghostBlinky != nullptr) {
		SpawnGhost(blinky, true);
	}
	if (ghostPinky != nullptr) {
		SpawnGhost(pinky, true);
	}
	if (ghostInky != nullptr) {
		SpawnGhost(inky, true);
	}
	if (ghostClyde != nullptr) {
		SpawnGhost(clyde, true);
	}

	ResetGamePhase();
}

void GameMap:: SpawnPacman()
{
	int i1 = pacmanStartI;
	int i2 = pacmanStartI;
	int j1 = pacmanStartJ;
	int j2 = pacmanStartJ;

	switch (pacmanStartOffset) {
	case up: j2 -= 1; break;
	case left: i2 -= 1; break;
	case down: j2 += 1; break;
	case right: i2 += 1; break;
	}

	GameCharacterPacman* pacmanChar = GetPacman();

	pacmanChar->SetDead(false, *this);

	pacmanChar->SetDirection(Enums::ComposeDirections(pacmanStartOffset, left));

	pacmanChar->SetPosition(i1, i2, j1, j2, *this);
}

void GameMap::SpawnGhost(GhostType type, bool firstSpawn)
{
	int i1 = ghostSpawnI;
	int i2 = ghostSpawnI;
	int j1 = ghostSpawnJ;
	int j2 = ghostSpawnJ;
	Direction ghostOffset = ghostSpawnOffset;

	GameCharacterGhost* ghostChar = GetGhost(type);

	ghostChar->SetDead(false, *this);
	ghostChar->SetSpawnDone(false);

	ghostChar->SetDirection(ghostSpawnDir);

	if (firstSpawn) {
		if (type == blinky) {
			i1 = ghostExitI;
			i2 = ghostExitI;
			j1 = ghostExitJ;
			j2 = ghostExitJ;
			switch (ghostSpawnDir) {
			case up: j1 -= 1; j2 -= 1; break;
			case left: i1 += 1; i2 += 1; break;
			case down: j1 += 1; j2 += 1; break;
			case right: i1 -= 1; i2 -= 1; break;
			}
			ghostOffset = ghostExitOffset;
			ghostChar->SetSpawnDone(true);
			ghostChar->SetDirection(Enums::ComposeDirections(ghostExitOffset, left));
		}
		else if (type == inky) {
			switch (ghostSpawnDir) {
			case up: i1 -= 2; i2 -= 2; break;
			case left: j1 += 2; j2 += 2; break;
			case down: i1 += 2; i2 += 2; break;
			case right: j1 -= 2; j2 -= 2; break;
			}
			ghostChar->SetStartDelay(0.5f);
			//ghostChar->SetDirection(Enums::ComposeDirections(ghostSpawnDir, up));
		}
		else if (type == clyde) {
			switch (ghostSpawnDir) {
			case up: i1 += 2; i2 += 2; break;
			case left: j1 -= 2; j2 -= 2; break;
			case down: i1 -= 2; i2 -= 2; break;
			case right: j1 += 2; j2 += 2; break;
			}
			ghostChar->SetStartDelay(2.0f);
			//ghostChar->SetDirection(Enums::ComposeDirections(ghostSpawnDir, down));
		}
	}
	else {
		ghostChar->SetStartDelay(2.0f);
	}

	switch (ghostOffset) {
	case up: j2 -= 1; break;
	case left: i2 -= 1; break;
	case down: j2 += 1; break;
	case right: i2 += 1; break;
	}

	ghostChar->SetPosition(i1, i2, j1, j2, *this);
}

int GameMap::GetDistance0(int i1, int j1, int i2, int j2)
{
	int di = i1 - i2;
	int dj = j1 - j2;

	return max(abs(di), abs(dj));
}

int GameMap::GetDistance1(int i1, int j1, int i2, int j2)
{
	int di = i1 - i2;
	int dj = j1 - j2;

	return abs(di) + abs(dj);
}

int GameMap::GetDistance2(int i1, int j1, int i2, int j2)
{
	int di = i1 - i2;
	int dj = j1 - j2;

	return di*di + dj*dj;
}

int GameMap::GetDistance0(GameCharacter * char1, GameCharacter * char2)
{
	return GetDistance0(char1->GetI(*this), char1->GetJ(*this), char2->GetI(*this), char2->GetJ(*this));
}

int GameMap::GetDistance1(GameCharacter * char1, GameCharacter * char2)
{
	return GetDistance1(char1->GetI(*this), char1->GetJ(*this), char2->GetI(*this), char2->GetJ(*this));
}

int GameMap::GetDistance2(GameCharacter * char1, GameCharacter * char2)
{
	return GetDistance2(char1->GetI(*this), char1->GetJ(*this), char2->GetI(*this), char2->GetJ(*this));
}

bool GameMap::Contact(GameCharacter * char1, GameCharacter * char2)
{
	if (useDistanceForCollision) {
		float x1 = char1->GetX();
		float y1 = char1->GetY();
		float x2 = char2->GetX();
		float y2 = char2->GetY();
		float dx = x2 - x1;
		float dy = y2 - y1;
		return (dx * dx + dy * dy < res);
	} else {
		return (char1->GetI(*this) == char2->GetI(*this) && char1->GetJ(*this) == char2->GetJ(*this));
	}
}

bool GameMap::Contact(GameCharacter * char1, PacDot * dot)
{
	if (useDistanceForCollision) {

		float x1 = char1->GetX();
		float y1 = char1->GetY();
		float x2 = dot->GetX(*this);
		float y2 = dot->GetY(*this);
		float dx = x2 - x1;
		float dy = y2 - y1;
		return (dx * dx + dy * dy < res/2.0f);
	}
	else {
		return (char1->GetI(*this) == dot->GetI() && char1->GetJ(*this) == dot->GetJ());
	}
}

bool GameMap::UseAStarPathing()
{
	return useAStarPathing;
}

bool GameMap::IsCurrentGamePhaseDone()
{
	if (currentPhase == freeroam) {
		return freeRoamTimer <= 0.0f;
	}
	else if (currentPhase == eatdots) {
		return eatenDots >= targetEatentDots;
	}
	else if (currentPhase == findsupergum) {
		if (currentSuperGum == nullptr) {
			return true;
		}
		else {
			return currentSuperGum->IsEaten();
		}
	}
	else if (currentPhase == huntghosts) {
		bool allGhostsDone = true;

		if (ghostBlinky != nullptr) {
			allGhostsDone = allGhostsDone && !ghostBlinky->GetVulnerable();
		}
		if (ghostPinky != nullptr) {
			allGhostsDone = allGhostsDone && !ghostPinky->GetVulnerable();
		}
		if (ghostInky != nullptr) {
			allGhostsDone = allGhostsDone && !ghostInky->GetVulnerable();
		}
		if (ghostClyde != nullptr) {
			allGhostsDone = allGhostsDone && !ghostClyde->GetVulnerable();
		}

		return allGhostsDone;
	}

	return false;
}

void GameMap::StartNextGamePhase()
{
	if (currentPhase == freeroam) {
		currentPhase = eatdots;

		freeRoamTimer = 0.0f;
		ActivatePacDots(false);
	}
	else if (currentPhase == eatdots) {
		currentPhase = findsupergum;

		DeactivatePacDots(false);
		ActivateRandomPacDot(true);
	}
	else if (currentPhase == findsupergum) {
		currentPhase = huntghosts;

		DeactivatePacDots(true);

		if (ghostBlinky != nullptr) {
			ghostBlinky->SetVulnerable(true);
		}
		if (ghostPinky != nullptr) {
			ghostPinky->SetVulnerable(true);
		}
		if (ghostInky != nullptr) {
			ghostInky->SetVulnerable(true);
		}
		if (ghostClyde != nullptr) {
			ghostClyde->SetVulnerable(true);
		}
	}
	else if (currentPhase == huntghosts) {
		currentPhase = freeroam;

		if (ghostBlinky != nullptr) {
			ghostBlinky->SetVulnerable(false);
		}
		if (ghostPinky != nullptr) {
			ghostPinky->SetVulnerable(false);
		}
		if (ghostInky != nullptr) {
			ghostInky->SetVulnerable(false);
		}
		if (ghostClyde != nullptr) {
			ghostClyde->SetVulnerable(false);
		}

		freeRoamTimer = freeRoamDuration;
	}
}

void GameMap::ResetGamePhase()
{
	currentPhase = freeroam;
	freeRoamTimer = freeRoamDuration;

	DeactivatePacDots(false);
	DeactivatePacDots(true);

	if (ghostBlinky != nullptr) {
		ghostBlinky->SetVulnerable(false);
	}
	if (ghostPinky != nullptr) {
		ghostPinky->SetVulnerable(false);
	}
	if (ghostInky != nullptr) {
		ghostInky->SetVulnerable(false);
	}
	if (ghostClyde != nullptr) {
		ghostClyde->SetVulnerable(false);
	}
}
