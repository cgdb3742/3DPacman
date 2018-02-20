#pragma once

enum Direction { undefined = 0, up = 1, left = 2, down = 3, right = 4 };

enum DataType {
	hole = 0, //black
	wall = 1, //blue
	path = 2, //white
	cross = 3, //green
	noupcross = 4, //yellow
	ghostspawn = 5, //red
	ghostexit = 6, //magenta
	pacmanstart = 7, //cyan
	tunnel = 8, //grey
	size = 9
};

enum GamePhase {
	freeroam = 0,
	eatdots = 1,
	findsupergum = 2,
	huntghosts = 3
};

enum GhostType { blinky = 0, pinky = 1, inky = 2, clyde = 3 };

struct ColorData
{
	ColorData() {
		red = 0;
		green = 0;
		blue = 0;
	}

	ColorData(unsigned int redValue, unsigned int greenValue, unsigned int blueValue) {
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

	unsigned int red;
	unsigned int green;
	unsigned int blue;
};

class Enums
{
public:
	Enums();
	~Enums();

	static float DirToAngle(Direction dir);
	static Direction AngleToDir(float angle);
	static Direction ComposeDirections(Direction dir1, Direction dir2);
	static bool AreClose(float angle1, float angle2);
	static bool AreClose(float angle, Direction dir);
	static int GetRandomNumber(int const min, int const max);
};

