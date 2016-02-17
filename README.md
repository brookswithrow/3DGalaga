# 3DGalaga
Adapting an old, 2D Galaga clone written in C for a class project to use OpenGL. Written in Visual Studio.

## Currently working features:
* Player can move & fire missiles
* Enemies can move & fire missiles
* Collision detection between ships & between ship and missile
* All entities use 3D models

## Planned features:
Generally listed in order of priority:
* Different models for enemies
* Better hit detection (currently have to hit the target dead center)
* Smoother movement for player & enemies
* Shading (including using missiles as light sources)
* Not boring background
* Title screen, controls screen, and game over screen
* Multiple waves of enemies/levels (randomly generated?)
* Moveable camera
* Explosion animation
* Sound

## Referenced materials:
* Almost all code that deals with Assimp was written following the model loading tutorial on http://learnopengl.com/, written by Joey De Vries.
* The current ship model being used for everything (including missiles) was downloaded at tf3dm.com, specifically  http://tf3dm.com/3d-model/low-poly-spaceship-37605.html. The (incredibly simple) textures are mine.
