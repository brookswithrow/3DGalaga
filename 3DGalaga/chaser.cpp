#include "chaser.h"

void Chaser::updateWiggle(int frameCounter) {
	if (frameCounter % 10 == 0) {
		if (wiggle < 0) {
			if (wiggle >= -2) {
				wiggle--;
				x -= 0.3;
			}
			else {
				wiggle = 1;
				x -= 0.3;
			}
		}
		else {
			if (wiggle <= 2) {
				wiggle++;
				x += 0.3;
			}
			else {
				wiggle = -1;
				x += 0.3;
			}
		}
	}
}