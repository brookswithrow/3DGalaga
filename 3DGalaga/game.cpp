#include "game.h"
#include <stdio.h>
#include <stdlib.h>

int checkBounds(int x, int y, int w, int h, int screenw, int screenh) {
	return (x - w) < -screenw || (x + w) > screenw || (y - h) < -screenh || (y + h) > screenh;
}

int checkCollision(int attackX, int attackY, int targetX, int targetY, int targetW, int targetH) {
	int hit = 0;
	if (attackX >= (targetX - targetW) && attackX<= (targetX + targetW)) {
		if (attackY>= (targetY - targetH) && attackY<= (targetY + targetH)) {
			hit = 1;
		}
	}
	if (attackX>= (targetX - targetW) && attackX <= (targetX + targetW)) {
		if (attackY>= (targetY - targetH) && attackY<= (targetY + targetH)) {
			hit = 1;
		}
	}
	return hit;
}

void makeShooters(Shooter shooters[], int numShooters) {
	int i;
	int row = 10;
	int col = -10;
	for (i = 0; i < numShooters; i++) {
		shooters[i].x = col;
		shooters[i].y = row;
		shooters[i].missileThreshold = (rand() % 500) + 100;
		shooters[i].missileCounter = 1;
		shooters[i].exists = 1;
		shooters[i].wiggle = 0;
		col += 5;
	}
}

void makeChasers(Chaser chasers[], int numChasers) {
	int i;
	int row = 7;
	int col = -10;
	for (i = 0; i < numChasers; i++) {
		chasers[i].x = col;
		chasers[i].y = row;
		chasers[i].hwidth = 0.5;
		chasers[i].hheight = 0.5;
		chasers[i].attackThreshold = (rand() % 800) + 100;
		chasers[i].attackCounter = 1;
		chasers[i].exists = 1;
		chasers[i].wiggle = 1;
		col += 5;
	}
}

void firePlayerMissile(Missile missiles[], int maxMissiles, Player player, int* missileCooldown) {
	int spotFound = 0;
	int index = 0;
	while (!spotFound) {
		if (index >= maxMissiles) {
			index = -1;
			spotFound = 1;
		}
		else if (!missiles[index].exists) {
			spotFound = 1;
		}
		else {
			index++;
		}
	}
	if (index != -1) {
		missiles[index].x = player.x;
		missiles[index].y = player.y + 1;
		missiles[index].speed = 0.5;
		missiles[index].color = 1;
		missiles[index].exists = 1;
		*missileCooldown = 0;
	}
}



void beginAttack(Chaser* chaser, Player* player) {
	chaser->attacking = 1;
	chaser->homeX = chaser->x;
	chaser->homeY = chaser->y;
	chaser->targetX = player->x;
	chaser->targetY = player->y;
	chaser->attackCounter = 0;
}

void continueAttack(Chaser* chaser) {
	if (chaser->x != chaser->targetX) {
		if (chaser->x > chaser->targetX) {
			chaser->x-= 0.1;
		}
		else {
			chaser->x+= 0.1;
		}
	} if (chaser->y != chaser->targetY) {
		if (chaser->y > chaser->targetY) {
			chaser->y-=0.1;
		}
		else {
			chaser->y+=0.1;
		}
	} if (((chaser->x - chaser->targetX) < 0.3) && ((chaser->y - chaser->targetY) < 0.3)) {
		chaser->returning = 1;
		chaser->attacking = 0;
	}
}

void continueReturn(Chaser* chaser) {
	if (chaser->x != chaser->homeX) {
		if (chaser->x > chaser->homeX) {
			chaser->x-=0.1;
		}
		else {
			chaser->x+=0.1;
		}
	} if (chaser->y != chaser->homeY) {
		if (chaser->y > chaser->homeY) {
			chaser->y-=0.1;
		}
		else {
			chaser->y+=0.1;
		}
	}
	else if (chaser->x == chaser->homeX && chaser->y == chaser->homeY) {
		chaser->returning = 0;
	}
}

int checkCollisionChaser(Chaser* chaser, Player* player) {
	if (checkCollision(chaser->x, chaser->y, player->x, player->y, player->hwidth, player->hheight)) {
		//TODO: clear player & chaser
		player->lives--;
		printf("Lives: %d (chaser collision)", player->lives);
		chaser->exists = 0;
		player->x = 0;
		player->y = 0;
		return 1;
	}
	return 0;
}



void fireShooterMissile(Missile missiles[], int maxMissiles, Shooter* shooter) {
	int spotFound = 0;
	int index = 0;
	while (!spotFound) {
		if (index >= maxMissiles) {
			index = -1;
			spotFound = 1;
		}
		else if (!missiles[index].exists) {
			spotFound = 1;
		}
		else {
			index++;
		}
	}
	missiles[index].x = shooter->x + 0.5;
	missiles[index].y = shooter->y + 0.5;
	missiles[index].speed = 0.5;
	missiles[index].color = 0;
	missiles[index].exists = 1;
	shooter->missileCounter = 0;
}

int checkCollisionShooter(Shooter* shooter, Player* player) {
	if (checkCollision(shooter->x, shooter->y, player->x, player->y, player->hwidth, player->hheight)) {
		//TODO: clear player & shooter
		printf("Lives: %d (shooter collision)", player->lives);
		shooter->exists = 0;
		player->reset();
		return 1;
	}
	return 0;
}

int checkMissileHitShooter(Shooter* shooter, Missile* missile) {
	if (shooter->exists && missile->color == 1
		&& checkCollision(missile->x, missile->y, shooter->x, shooter->y, shooter->hwidth, shooter->hheight)) {
		//TODO: clear missile & shooter
		shooter->exists = 0;
		missile->exists = 0;
		printf("Missile hit shooter: Missile =  %f, %f, Shooter =  %f, %f, %f, %f\n", missile->x, missile->y, shooter->x, shooter->y, shooter->hwidth, shooter->hheight);
		return 1;
	}
	return 0;
}

int checkMissileHitChaser(Chaser* chaser, Missile* missile) {
	if (chaser->exists && missile->color == 1
		&& checkCollision(missile->x, missile->y, chaser->x, chaser->y, chaser->hwidth, chaser->hheight)) {
		//TODO: clear missile & chaser
		chaser->exists = 0;
		missile->exists = 0;
		return 1;
	}
	return 0;
}

int checkMissileHitPlayer(Player* player, Missile* missile) {
	if (checkCollision(missile->x, missile->y, player->x, player->y, player->hwidth, player->hheight)) {
		// TODO: Clear player & missile
		player->lives--;
		printf("Lives: %d (missile)", player->lives);
		player->reset();
		missile->exists = 0;
		return 1;
	}
	return 0;
}

void gameLoop(int* missileCooldown, int* frameCounter, Player* player, Chaser* chasers,
	Shooter* shooters, Missile* missiles, int maxChasers, int maxShooters, int maxMissiles,
	int* numChasers, int* numShooters, int* numMissiles, int* gameState) {
	(*missileCooldown)++;
	(*frameCounter)++;
	//player->lives = 3;
	//clear(player.x, player.y, PLAYER_WIDTH, PLAYER_HEIGHT);
	int i, j;
	for (i = 0; i < maxChasers; i++) {
		if (chasers[i].exists) {
			Chaser* current = &chasers[i];
			if (!current->attacking && !current->returning) {
				current->updateWiggle(*frameCounter);
				current->attackCounter++;
				if (current->attackCounter > current->attackThreshold) {
					beginAttack(current, player);
				}
			}
			else if (current->attacking) {
				continueAttack(current);
			}
			else if (current->returning) {
				continueReturn(current);
			}
			if (checkCollisionChaser(current, player)) {
				numChasers--;
			}
		}
	}
	for (i = 0; i < maxShooters; i++) {
		if (shooters[i].exists) {
			Shooter* current = &shooters[i];
			current->updateWiggle(*frameCounter);
			current->missileCounter++;
			if (current->missileCounter > current->missileThreshold) {
				fireShooterMissile(missiles, maxMissiles, current);
				(*numMissiles)++;
			}
			if (checkCollisionShooter(current, player)) {
				(*numShooters)--;
			}
		}
	}

	for (i = 0; i < maxMissiles; i++) {
		if (missiles[i].exists) {
			Missile* current = &missiles[i];
			if (current->color == 1) {
				current->y += current->speed;
			}
			else {
				current->y -= current->speed;
			}
			if (checkBounds(current->x, current->y, 0.5, 0.5, 12, 12)) {
				current->exists = 0;
				(*numMissiles)--;
			}
			else {
				for (j = 0; j < maxShooters; j++) {
					Shooter* shooter = &shooters[j];
					if (checkMissileHitShooter(shooter, current)) {
						(*numShooters)--;
						(*numMissiles)--;
					}
				}
				for (j = 0; j < maxChasers; j++) {
					Chaser* chaser = &chasers[j];
					if (checkMissileHitChaser(chaser, current)) {
						(*numChasers)--;
						(*numMissiles)--;

					}
				}
				if (checkMissileHitPlayer(player, current)) {
					(*numMissiles)--;
				}
			}
		}
	}
	if ((*numChasers + *numShooters) == 0) {
		printf("You win!");
		(*frameCounter) = 0;
		(*gameState) = 3;
	}
	if (player->lives == 0) {
		printf("You lose!");
		(*frameCounter) = 0;
		(*gameState) = 3;
	}
}