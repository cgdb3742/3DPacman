#include "Enums.h"

#include <random>

Enums::Enums()
{
}


Enums::~Enums()
{
}

float Enums::DirToAngle(Direction dir)
{
	switch (dir) {
	case up: return 90.0f;
	case left: return 180.0f;
	case down: return -90.0f;
	case right:
	case undefined:
	default:
		return 0.0f;
	}
}

Direction Enums::AngleToDir(float angle)
{
	while (angle <= -180.0f) {
		angle += 360.0f;
	}
	while (angle > 180.0f) {
		angle -= 360.0f;
	}

	if (angle > 45.0f && angle <= 135.0f) {
		return up;
	}
	if (angle > 135.0f || angle <= -135.0f) {
		return left;
	}
	if (angle > -135.0f && angle <= -45.0f) {
		return down;
	}
	if (angle > -45.0f && angle <= 45.0f) {
		return right;
	}

	return undefined;
}

Direction Enums::ComposeDirections(Direction dir1, Direction dir2)
{
	if (dir1 == undefined || dir2 == undefined) {
		return undefined;
	}

	int composedDir = (int)dir1 + (int)dir2;
	composedDir = composedDir % 4;
	if (composedDir == 0) composedDir = 4;

	return (Direction)composedDir;
}

bool Enums::AreClose(float angle1, float angle2)
{
	float threshold = 0.01;

	float diffAngle = angle1 - angle2;

	while (diffAngle <= -180.0f) {
		diffAngle += 360.0f;
	}
	while (diffAngle > 180.0f) {
		diffAngle -= 360.0f;
	}

	return (diffAngle > -threshold && diffAngle < threshold);
}

bool Enums::AreClose(float angle, Direction dir)
{
	if (dir == undefined) {
		return false;
	}

	return AreClose(angle, DirToAngle(dir));
}

int Enums::GetRandomNumber(int const min, int const max)
{
	static std::random_device rd;
	static std::default_random_engine engine(rd());
	std::uniform_int_distribution<> distribution(min, max);
	return distribution(engine);
}
