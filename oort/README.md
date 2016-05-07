# CS 354R - Oort

## Milestone 4.3

### How to Build
* Refer to "HOW_TO_BUILD" file!

### How to play
* Press C - change the cameras
* Press M - mute music
* Press ESC to quit the game
* Press W & S control the pitch of the Spaceship
* Press A & D control the yaw  of the Spaceship
* Press Q & E control the roll of the Spaceship
* Press left mouse button to fire laser
* Press Mouse Scroll to throttle up and down the Spaceship's trust to move it along it's current trajectory

### Progress

#### Summary
* We created a fun and intuitive space asteroids shooter game.

#### Implemented
* Change music
* CEGUI to inform player of score
* Add asteroids
* Mesh slicing, mesh manipulation
* Add world boundaries with planes

#### Code and Outside resources used
* Spaceship model downloaded from:http://www.blendswap.com/blends/view/75404 credit: DeNapes
* Code from Itoro's Assignment 3 of CS354R
* Asteroid model downloaded from:http://www.blendswap.com/blends/view/69608 credit: S2
* Laser sound http://opengameart.org/content/63-digital-sound-effects-lasers-phasers-space-etc credit: Kenney.nl
* Loop music Star Fox (1993) - Space Armada OST credit: Hajime Hirasawa
* Menu click https://www.youtube.com/watch?v=HCqRNkiE0lI credit: Creative Commons Footage, Music, Template, Plugins
* Star Fox 64 (1997): https://www.youtube.com/watch?v=Lb4UY7Lqs8o
  * Ship destroyed sound
  * Asteroid hit sound

## Milestone 4.2

### Implemented
* Throttle based spaceship control system, uses scroll to manage throttle
* A better follow camera for the space ship
* Laser Instantiation, Firing and Movement
* Added Particle physics to laser movement

### TODO
* Change music
* CEGUI to inform player of ammo, minimap
* Add asteroids
* Mesh slicing, mesh manipulation
* Integrate Assimp to load mesh in real time
* Add world boundaries with planes

### ISSUES
We are on schedule and this was turned in on time.

### DIVISION OF LABOR
Itoro created the laser class and the camera toggle data structure and Kaleb implimented laser movement, camera movement and the spaceship throttle. 
We both did a part of everything that's been implemented. We kept adding and modifying our logic to be cohesive with the style of game we are creating.

### How to play
* Press C - change the cameras
* Press M - mute music
* Press ESC to quit the game
* Press W & S control the pitch of the Spaceship
* Press A & D control the yaw  of the Spaceship
* Press Q & E control the roll of the Spaceship
* Press left mouse button to fire laser
* Press Mouse Scroll to throttle up and down the Spaceship's trust to move it along it's current trajectory


### Code and Outside resources used
* Spaceship model downloaded from:http://www.blendswap.com/blends/view/75404 credit: DeNapes
* Code from Itoro's Assignment 3 of CS354R

## Milestone 4.1

### Project Description

This game is a 3D single-player Asteroids that focuses on mesh dividing, animation, and simulates fragmentation of an asteroid impacted by a spaceship's laser.

#### Overall Design

Translucent textures are on asteroids inside a wireframe room with the spaceship. 

#### Asteroids

When asteroids are hit by spaceship's beams and are sliced into debris. Astroids start outside of the room then follow a velocity directed toward the spaceship. They don't change direction when motion starts. If an asteroid leaves the room, another is spawn. 

#### Spaceship

Explosion animation when destroyed. Particle trail following the spaceship. Shoot lasers/beams at asteroids. Space flight is done with the controls below. 

#### Laser

The laser is a thin rectangular prism. It will travel in look vector direction of the space ship. 

#### Collision

When the laser and an asteroid collide, the laser must hit a face of the asteroid with some component in the look direction. That means that you cannot graze an asteroid and destroy it. When a laser hits an asteroid, it hits a point on the face of the asteroid. With that point, we can slice the astroid by an arbitrary plane which is represented by that point and a vector that is orthogonal to the look vector.

#### Cameras

A camera that follows the spaceship. 

#### Controls 

##### Ship controls
W & S control the pitch 
A & D control the yaw 
Q & E control the roll 
Arrow key up increases the throttle force
Arrow key down decreases the throttle force
Space to shoot the laser

Debating on having multiple firing types that would be controlled with number keys 1-4

#### Score Mechanic 

Simple point system: You score points by destroying asteroids.

#### Win/Lose Condition

You have a certain amount of lives. If you are hit by an asteroid, you are killed. When you ared out of lives, game ends. 

#### Architectural Changes

We'll gut the previous projects code down to it's base and incorporate the ideas above into our code. Overall structure design may be the same but this is no longer a ball game. 

#### Contributor
* Itoro Esiere
* Kaleb Alanis

#### Engines/Libraries
* Ogre3D
* Bullet physics
* SDL
