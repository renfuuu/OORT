## Milestone 3.3

### How to Build
* Refer to "HOW_TO_BUILD" file!

### How to play
* To play alone, select single player
* Choose to Host a game or join a game (requires two computers)
* To join a game, one computer must be hosting and you must enter the public ip address of the machine that is hosting and press join game
* Move the mouse up/down and left/right on the game window to control the paddle
* Strike the ball with the paddle to propel it through the room and attempt to bypass your opponent's paddle
* M mutes the sounds
* Keys 1,2,3, and 4 change the camera angle (front, side,ball chase view, and free camera)
* Free camera works as a camera man (WSAD or Arrow keys to move) hold shift to speed up
* Escape key to quit or click the quit button

### Progress

#### Summary
* We created a fun and intuitive ball game that the user can choose to play in 1-player or 2-player mode. 

#### Implemented
* Networking that uses UDP protocol
* Prevent game start until all palyers joined
* Detect client disconnect
* Prevent crash if the player enters an incorrect ip address
* Parser for the messages we send over the network
* Converted from OGRE Overlay with CEGUI Text
* Finite states for game modes: Home, How to Play, Single Player, Server, Client, Replay, and End of Game
* Server mechanic, if you score a point then you get to serve
* Cloud skybox and stadium background
* Single Player mode + AI
* Remove highscore

#### Extra Credit
* Replay for after the game ends so you can watch it
* AI for single player

## Milestone 3.2

### Implemented
* Core game from Assignment-2
* Added giant stadium in the background
* Working server-client connection of a UDP protocol
* CEGUI buttons to host a game or join a game with a text box to enter the host's IP Address
* We send key-value pairs as a string over the network and it gets parsed and setup into a hashtable locally, the data sent is ball position and velocity as well as the paddle's orientations.
* The server is the ultimate authority as far as the bullet simulation and the client (opponent player) gets updated in real time of the ball's true position.

### TODO
* Single Player mode + AI
* Prevent game start until all palyers joined
* Detect client disconnect
* Changing scoring rules / who gets to server
* Prevent crash if the player enters an incorrect ip address
* Remove highscore
* Replace OGRE Overlay with CEGUI Text

### ISSUES
* Overlay Manager causes weird gray texture artifacting (can be cleared by restarting the assignment or by clearing the overlay window)
* Ball sometimes leaves the playable area

### How to play
* Choose to Host a game or join a game (requires two computers)
* To join a game, one computer must be hosting and you must enter the public ip address of the machine that is hosting and press join game
* Move the mouse up/down and left/right on the game window to control the paddle
* Strike the ball with the paddle to propel it through the room and attempt to bypass your opponent's paddle
* Space bar resets score and the ball back to its inital position (for the host)
* M mutes the sounds
* Keys 1,2,3, and 4 change the camera angle (front, side,ball chase view, and free camera)
* Free camera works as a camera man (WSAD or Arrow keys to move) hold shift to speed up
* Escape key to quit or click the quit button

### Code and Outside resources used
* Paddle model downloaded from: http://www.blendswap.com/blends/view/49333 credit: Bruno Matheus
* Started with some of the bullet example code from the lecture slides
* Sound files from the public domain
* Background Music - http://www.orangefreesounds.com/music-loop-120-bpm/
* Dameron font downloaded from http://www.dafont.com/dameron.font
* Wall image downloaded from http://4.bp.blogspot.com/-5xNMmxVjWKg/UA5bNp_0jaI/AAAAAAAAB4s/QXcwBUUPPKw/s1600/Seamless+wall+white+paint+stucco+plaster+texture.jpg
* Court image downloaded from http://www.ghpayouth.com/images/basketball-floor-texture.jpg
* Headshot sound downloaded from http://gs.bel-net.ru/cs/download/sound/misc/headshot.wav
* Stadium mesh downloaded from http://www.blendswap.com/blends/view/52154 credit: MrChimp2313
* NetManager.h and NetManager.cpp taken from Wade Burch, supplied by Sarah
* OGRE Basic tutorial 7 for CEGUI

## Milestone 3.1
Architecture Upgrade:
Player object that stores each Player's paddle location, camera views, and scores.

Client-Server model: 
We will create a CEGUI window at the beginning of the game. Which asks the player whether they would like to "Host a Game" or "Join a Game". The host (or server) does the bullet simulations with the received information from the players (or clients) and sends ball information to them. Players send paddle information (or the Player object in the form of packets) to the host so it can process the simulations. The CEGUI has a simple lobby containing players hosting when a new player selects to "Join a game".

Scoring change:
Instead of gaining points by hitting the ball it, you gain points by getting the ball over the net behind your opponent. Similar to playing tennis or real paddle ball. 

Graphic change:
We changed our room with a basketball court floor to a stadium skybox while the game is played inside glass walls.


## Milestone 2.3

### How to Build
* Refer to "HOW_TO_BUILD" file!

### How to play
* Move the mouse up/down and left/right on the game window to control the paddle
* Strike the ball with the paddle to propel it through the room and increase your score
* Space bar resets score and the ball back to its inital position
* M mutes the sounds
* Keys 1,2 and 3 change the camera angle (front, side, and ball chase view)
* Game ends if the ball bounces on the ground twice without hitting any other wall or the paddle
* Escape key to quit

### Progress

#### Summary
We feel that this game is fun and intuitive. It has been textured, lit and polished in a manner that doesn't leave the player confused or dizzzy. 

#### Implemented
* We were able to get gameobjects of types paddle, wall and ball set up and working perfectly.
* Bullet simulator with motionstate wrapper that handles motions between bullet transforms and ogre scene transforms
* Working collisions / collision detection
* User controls
* Paddle moves on parabolic arc in 3d space that gives a more realsitic feel for the motion of the paddle
* Ball loses velocity over time until the player hits it again with the paddle
* Different sounds played for collision between ball/paddle and ball/wall
* Added scoring system that increases points with each strike of the ball
* Added GUI to display score to the user
* Textures for the walls, paddle and the ground
* Ball reset
* Sound muting
* Additional camera angles
* Particle effects (purple lazer trail on the ball)
* Highscore is saved and written out to a file so it remains between loads of the game
* HEADSHOT! If the player hits the back wall directly with the ball an extra point is awarded and a headshot sound is played
* "Game Over" is displayed after the game is over and is removed on the start of a new game.

#### Extra Credit
* Improved Camera Controls (1 point) - We added the option for the user to switch between 3 different camera modes. Each mode has a seperate camera in the scene. The last camera mode actually lets the user follow the ball in a chase-like camera.
* Background Music (2 points) - Plays in a loop, can also be muted at the player's will with the M key.
* End Condition (2 points) - The game ends if the ball bounces twice on the ground without colliding with another object. If a new highscore has been reached then that score is saved and everything is reset (ball goes back to center and the player's current score is 0).
* Particle Effects (2 points) - Ball has a purple plasma trail behind it that is attached to the ball.
* Paddle Position (Suggested) - We would like to add our system for positioning the paddle here since we believe it is worthy of some extra points. The paddle actually follows an elliptical paraboloid shape that is a function of the x, and y inputs from the screen. A z value is computed from a function f(x,y) in order to create an arc like shape that feels more like a player is using his shoulder. In addition to this, the paddle needed to be oriented in a way that made sense. To achieve this, the normal from the paraboloid to the paddle is computed. That normal then becomes the local Z axis for the new paddle orientation. An arbitrary orthonormal x, and y axis are computed by the cross product of the z-axis with the global y-axis (0,1,0), then the final taking the cross product of that result with the z-axis gives us the last orthonormal vector for constructing a quaterinion. This keeps the paddle's handle tangent to the paraboloid. The final challenge was making sure the paddle faces forward. So we rotate (roll) the paddle by angle computed from the arctan2 of the cos(a) and the sin(a) between the paddle and the global y-axis (0,1,0). The results came out to be very high quality and we wanted to see if Sarah or the grader would agree to give out some extra points for our hard work.

#### C++ Classes Used
* Application - Sets up scene, controls game logic
* Simulator - Holds a list of gameobjects, steps them inside the bullet dynamic world
* BulletContactCallback - Bullet calls this class when a collision between two objects is detected
* CollisionContext - Stores information about the collision that is passed through the contact callback
* OgreMotionState - Updates transforms between Ogre world and bullet world
* GameObject - A wrapper for storing bullet/ogre information and methods to abstract their usage
* Paddle - Child of GameObject, adds a collider shape around a paddle mesh
* Ball - Child of GameObject, adds a collider sphere shape around a ball mesh
* Wall - Child of GameObject, adds a cube collider shape around a plane mesh
* OISManager - Event driven user input detection class
* TextOverlay - Allows us to put 2D gui and text elements onto the screen
* GameManager - A combination of SoundManager & ScoreManager.
* SoundManager - Stores sounds and plays them into the ogre scene
* ScoreManager - Manages and updates the score of the player
* SceneHelper - Allows us to get information faster from scennodes to entities
* MultiPlatFormHelper - Abstract usages for things like timing and printing on different platforms

### Code and Outside resources used
* Paddle model downloaded from: http://www.blendswap.com/blends/view/49333 credit: Bruno Matheus
* Started with some of the bullet example code from the lecture slides
* Sound files from the public domain
* Background Music - http://www.orangefreesounds.com/music-loop-120-bpm/
* Dameron font downloaded from http://www.dafont.com/dameron.font
* Wall image downloaded from http://4.bp.blogspot.com/-5xNMmxVjWKg/UA5bNp_0jaI/AAAAAAAAB4s/QXcwBUUPPKw/s1600/Seamless+wall+white+paint+stucco+plaster+texture.jpg
* Court image downloaded from http://www.ghpayouth.com/images/basketball-floor-texture.jpg
* Headshot sound downloaded from http://gs.bel-net.ru/cs/download/sound/misc/headshot.wav


## Milestone 2.2

### How to play
* Move the mouse up/down and left/right on the game window to control the paddle
* Strike the ball with the paddle to propel it through the room and increase your score
* Space bar resets score and the ball back to its inital position

### Progress

#### Summary
We feel that we are actually ahead of schedule and everything went smoothly this week and we did not get behind on any of our deliverables.

#### Implemented
* We were able to get gameobjects of types paddle, wall and ball set up and working perfectly.
* Bullet simulator with motionstate wrapper that handles motions between bullet transforms and ogre scene transforms
* Working collisions / collision detection
* User controls are there for the most part especially for the mouse
* Paddle moves on parabolic arc in 3d space that gives a more realsitic feel for the motion of the paddle
* Ball loses velocity over time until the player hits it again with the paddle
* Different sounds played for collision between ball/paddle and ball/wall
* Added scoring system that increases points with each strike of the ball
* Added GUI to display score to the user
* Ball reset

#### Stil Need Implementation
* Camera controls - not yet implemented, need to add an option for user to change his view
* Game polishing - need to smooth out the interaction with the paddle and the paddle to the ball
* Add extra credits - End condition, particles, user interface GUI
* Muting - mute volume and sounds specifically?

#### C++ Classes Used
* Application - Sets up scene, controls game logic
* Simulator - Holds a list of gameobjects, steps them inside the bullet dynamic world
* BulletContactCallback - Bullet calls this class when a collision between two objects is detected
* CollisionContext - Stores information about the collision that is passed through the contact callback
* OgreMotionState - Updates transforms between Ogre world and bullet world
* GameObject - A wrapper for storing bullet/ogre information and methods to abstract their usage
* Paddle - Child of GameObject, adds a collider shape around a paddle mesh
* Ball - Child of GameObject, adds a collider sphere shape around a ball mesh
* Wall - Child of GameObject, adds a cube collider shape around a plane mesh
* OISManager - Event driven user input detection class
* SoundAdapter - Stores sounds and plays them into the ogre scene
* OgreText - Quick and dirty text overlay for Ogre
* SceneHelper - Allows us to get information faster from scennodes to entities
* MultiPlatFormHelper - Abstract usages for things like timing and printing on different platforms

### Code and Outside resources used
* Paddle model downloaded from: http://www.blendswap.com/blends/view/49333 credit: Bruno Matheus
* Started with some of the bullet example code from the lecture slides
* Sound files from the public domain
* OgreText - Copied from http://ogre3d.org/tikiwiki/tiki-index.php?page=Simple+text


## Milestone 2.1

### Project Description

#### Overall Design

![Alt text](http://i.imgur.com/wXcjUxh.jpg "A2")

Assignment 2 is building upon assignment 1 where we take out room and ball and add a paddle to play a game. Additions will be Bullet's physics engine, sound effects from the SDL library, a paddle/racquet, and a scoring mechanic. 

##### Room

5-sided room with an invisible mesh in front of the camera.

Three "types" of collision to be handled
* Ball and paddle
* Ball and wall
* Ball escapes through mesh

Velocity loss by collision will be handled with physics engine.

##### Paddle

![Alt text](http://i.imgur.com/tyWzFVS.jpg "Paddle")

The paddle is controlled by the mouse. If the user moves the mouse forward then the paddle moves to up and if the user moves the mouse toward themselves (backwards) then the paddles moves down as illustrated in the left figure. The paddle moves on a parabola left to right by a point at the bottom of the handle as the user moves the mouse left to right.

Collision with the paddle accelerates the ball by a base velocity.

##### Camera Controls

Stationary camera outside the room looking through the invisible mesh.

##### Scoring Mechanism

Increment sore by one point whenever the ball and paddle collide. The game is loss when the ball bounces twice on the ground. After a game loss, highscore is recorded and game loss music plays.

#### Architecture

##### Application Layer

Contains the scene, set up, and gameloop. The Scene has a ball and paddle entity, soundadapter, and a scoremanager. Inside the gameloop we will have an update function that uses a global timer to modify the frames per second to 60 fps. We'll see getSingleton to use the Input Event Manager

##### Sound Adapter

A class that will handle our sounds when called.

Types of sounds:
* Ball collides with wall
* Ball collides with paddle
* Player loses

The player is able to mute the background music by pressing M.

##### Score Manager

A class that keeps track of points.

##### Input Manager 

A class to handle input from the player.

#### Division of Labor
* David: Bullet, Input Event Manager, Application
* Frank: SDL, Sound Adapter, Application
* Itoro: Doc, Score Manager, Application

#### Credit:
* Paddle model downloaded from: http://www.blendswap.com/blends/view/49333 credit: Bruno Matheus
