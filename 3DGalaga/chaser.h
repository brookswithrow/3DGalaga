#pragma once
/* Represents an enemy type that attacks the player by
charging at their position, then returning to their starting
position. */
class Chaser {
public:
	// Current position
	float x;
	float y;
	// Bounding box width & height, used for collision detection
	int hwidth;
	int hheight;
	// Attacks when attackCounter > attackThreshold. attackThreshold is randomly
	// set for each Chaser.
	int attackCounter;
	int attackThreshold;
	// Determines if the Chaser is attacking, returning to its starting position, or neither
	int attacking;
	int returning;
	// The player's location when the Chaser began its attack- will return home after
	// reaching this position
	int targetX;
	int targetY;
	// The Chaser's original location
	int homeX;
	int homeY;
	// Used for simple animation
	int wiggle;
	// Used to determine whether to draw the Chaser
	int exists;
	// Makes the Chaser wiggle slightly
	void updateWiggle(int frameCounter);
};