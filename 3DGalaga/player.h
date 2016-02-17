#pragma once
/* Represents the player's ship.*/
class Player {
public:
	// The player's position.
	float x;
	float y;
	// The ship's bounding box dimensions, used for collision detection.
	int hwidth;
	int hheight;
	// Number of lives the player has- starts at 3
	int lives;
	// Currently not in use- will eventually be used to give the player
	// some invincibility upon respawning.
	int iframes;
	// Resets the player's position.
	void reset();
};