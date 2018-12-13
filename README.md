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

![alt text](https://github.com/necroTaxonomist/FirefighterBot-public/blob/master/LogicDiagram.png)

The ideal robot would follow the following system:

  1. The robot is patrolling around a certain set area and looking for fires using a camera through a Raspberry Pi.
  2. When a fire is detected, the robot uses data from the camera to determine the direction the fire is in and the path to take towards it.
  3. When the robot has arrived at the fire, it will turn on the pump and shoot out all the water in the reservoir to put out the fire.
  4. When the fire has been put out, it will return to its designated patrol path.

Some difficulties involved with this system include:

  * Ideal path planning is a difficult task because the robot should be able to adjust to different environments
  * Propagating error due to insufficient motor control
  * Compatibility issues when running the fire detection classifier on the Pi
  * Best fire detection classifier implementation choice for different environments 

Prototype:
![alt text](https://github.com/necroTaxonomist/FirefighterBot-public/blob/master/images/IMG_0682.jpg)

Demo:

FirefighterBot Water Test
https://youtu.be/GWK5LJAiSWc

FirefighterBot Real Fire Test
https://youtu.be/jWqstQ8AiOc

FirefighterBot Demo and Instructions
https://youtu.be/bm2bm8v9Nbk
