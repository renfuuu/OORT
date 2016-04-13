# CS 354R - Oort

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