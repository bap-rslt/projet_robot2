
# Autonomous finder robot

The primary goal of this project was to code an autonomous object-finder robot project. The robot is powered by a PIC18 microcontroller, and is equipped with a sonar sensor to detect its surroundings.



## Operation

To initiate the operation of the robot, the user presses on the central button of a remote control.

#### 1 - Detection
The robot rotates on itself until it finds a target within a certain distance range (100 to 150 cm). 

#### 2 - Refining
It scans a wide area for 10 seconds, storing the distance measured by the sonar every 0.5 second. 
Then, the robot rotates until it is facing the closest part of the target. 

#### 3 - Approach
Finally, it approaches the target by 1 meter.

#### 4 - Looping operation cycle
The operation of the robot can be repeated by pressing on the remote button again.



## Other features

#### Battery monitoring
The robot is equipped a battery monitoring functionality. As soon as the battery voltage drops below a predefined threshold, a warning LED lights up to warn the user.

#### Pause between steps
The robot operation can be stopped and resumed between operation steps.

#### Remote debounce filter 
The remote buttons bouncing caused problems during the testing phase : the robot would not start because a double input was sent on a single press from time to time.

To eliminate this problem, a debounce filter was added to the code registering the recieved reomte inputs : only a single input can be recieved every 0.5 second.

#### Improved precision
As the sonar is prone to making errors because of artifacts. To mitigate this issue, the scanning process does an averaging of three consecutive sonar readings.


## Simulation

The code was tested in a simulated environment on the Proteus software. 


## Possible improvement 
The main improvement that is considered is the possibility to pause the robot operation during its steps, and not only between them. 

It is currently made difficult by the way the code handles timing : a singe 0.1s timer.
