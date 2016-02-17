#pragma once
/* Holds information about missiles. */
class Missile {
public:
	// The missile's position
	float x;
	float y;
	// The missile's speed
	float speed;
	// Indicates whether the missile was fired by the player or an enemy-
	// player missiles are yellow & enemy missiles are red.
	int color;
	// Indicates whether the missile should be drawn.
	int exists;
};