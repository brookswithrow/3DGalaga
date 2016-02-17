#include "player.h"
#include "shooter.h"
#include "chaser.h"
#include "missile.h"

/* Contains all the game logic, including the main game loop. */

/* Checks to see if a missile has left the bounds of the game. Used to delete
missiles that have left the screen. */
int checkBounds(int x, int y, int w, int h, int screenw, int screenh);

/* Checks collision between any two objects.*/
int checkCollision(int attackX, int attackY, int targetX, int targetY, int targetW, int targetH);

/* Initializes Shooters at the start of the game. */
void makeShooters(Shooter shooters[], int numShooters);

/* Initializes the Chasers at the start of the game. */
void makeChasers(Chaser chasers[], int numChasers);

/* Shoots a missle from the player. */
void firePlayerMissile(Missile missiles[], int maxMissiles, Player player, int* missileCooldown);

/* Starts a Chaser's attack. */
void beginAttack(Chaser* chaser, Player* player);

/* Chaser continues moving at its target. */
void continueAttack(Chaser* chaser);

/* Chaser has finished its attack and continues returning to its starting position. */
void continueReturn(Chaser* chaser);

/* Checks if a Chaser has hit the player. */
int checkCollisionChaser(Chaser* chaser, Player* player);

/* Fires a missile from a Shooter. */
void fireShooterMissile(Missile missiles[], int maxMissiles, Shooter* shooter);

/* Checks if the player has hit a Shooter (Shooters don't really move). */
int checkCollisionShooter(Shooter* shooter, Player* player);

/* Checks if a missile has hit a Shooter. */
int checkMissileHitShooter(Shooter* shooter, Missile* missile);

/* Checks if a missile has hit a Chaser. */
int checkMissileHitChaser(Chaser* chaser, Missile* missile);

/* Checks if a missile has hit the player. */
int checkMissileHitPlayer(Player* player, Missile* missile);

/* The main game loop. Checks for collision, moves enemies, determines whether
they should start attacking & makes them attack if needed, changes the game state
if the player has won or lost, etc.*/
void gameLoop(int* missileCooldown, int* frameCounter, Player* player, Chaser* chasers,
	Shooter* shooters, Missile* missiles, int maxChasers, int maxShooters, int maxMissiles,
	int* numChasers, int* numShooters, int* numMissiles, int* gameState);
