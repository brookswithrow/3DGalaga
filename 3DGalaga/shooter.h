#pragma once
/* Represents an enemy type called a Shooter, which stays relatively stationary and only
shoots missiles. */
class Shooter {
public:
	// The shooter's position
	float x;
	float y;
	// The shooter's bounding box's width/height, used for collision detection
	int hwidth;
	int hheight;
	// Used to determine whether the Shooter should fire a missile. Shooter fires if
	// missileCounter > missileThreshold. missileThreshold is randomly set for each Shooter.
	int missileCounter;
	int missileThreshold;
	// Determines if the Shooter has been destroyed.
	int exists;
	// Used for simple animation
	int wiggle;
	// Makes the Shooter wiggle slightly
	void updateWiggle(int frameCounter);
};