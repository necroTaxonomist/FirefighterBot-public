# Fire Supression Robot

Dian Guo, Tanner Muldoon, Kamran Sirohi, Alex McArthur

The purpose of the project is to program a robot that automatically detects a fire, drives to it, and sprays water on the fire to put it out.
The robot uses a Raspberry Pi and mbed to control it and gather information from the camera.
It can also be manually controlled using a Bluetooth controller like a phone, and will send an alert to the phone if there is a fire.

The robot has the following modes with different functionality:

  * Patrol: The robot moves around a certain set area looking for fires
  * Detection: The robot uses data from the camera to check for whether there is a fire in its area
  * Path Creation: The data from the camera is used to plot best path to fire
  * Suppression: The robot travels the best path, turns on the pump, and releases the water

The ideal robot would follow the following system:

  1. The robot is patrolling around a certain set area and looking for fires using a camera through a Raspberry Pi.
  2. When a fire is detected, the robot uses data from the camera to determine the direction the fire is in and the path to take towards it.
  3. When the robot has arrived at the fire, it will turn on the pump and shoot out all the water in the reservoir to put out the fire.
  4. When the fire has been put out, it will return to its designated patrol path.

Some difficulties involved with this system include:

  * Ideal path planning is a difficult task because the robot should be able to adjust to different environments
  * Propagating error due to insufficient motor control
  * Compatibility issues when running the fire detection classifier on the Pi

Patrol Implementation:
  The robot moves in a 2' by 2' square, stopping every few inches to run through Detection mode.  Since the camera uses motion to detect a fire, it is necessary that the robot be still when looking for fire.  If a fire is not detected, or if a fire is detected and then put out, the robot resumes its patrol.

Detection Implementation:
  The camera uses a background subtractor to remove any still images from its view.  Once the background subtractor is finished (it takes a couple of seconds to realize things have stopped moving), the camera will only see things that are moving.  Since the idea of this robot is to leave it to patrol a room checking for fires while no one is there, it can be reasonably assumed that any movement is a fire.  The patrol path is somewhat small and limited, but the camera angle is wide, and the robot faces enough directions during its patrol that it can see the entire room without having to pan the camera.  If motion is detected, the robot enters Path Creation mode.  If not, it resumes Patrol mode.

Path Creation Implementation:
  Once motion is detected by the camera, the robot sums the locations of all pixels that detect motion, in order to find the center of the area that is moving.  It then uses that position relative to the camera's view as a whole to determine what angle the robot needs to face in order to point at the fire.  Once this angle has been determined, the robot enters Suppression mode.
  
Suppression Implementation:
 Turns on the pump, spraying water forward. The robot then moves forward, stops, goes into Detection mode.  If it sees fire, it sprays again, repeating this until it no longer detects fire.  Then it returns to its original position and reenters patrol mode.

![alt text](https://raw.githubusercontent.com/tmuldoon3/FirefighterBot/blod/masterLogicDiagram.png)
