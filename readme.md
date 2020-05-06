# UNO Robotics code for Yamaraz and others

Moksha has the latest codes! Mad Max and Fury Road both have similar functionalities but are unpolished old code that eventually led to the development of the clean API provided by Moksha. There are also sensor-specific codes in the repository which are not cleaned up like the motion code.

Here's the functionality provided by Moksha's Motion source base:

Structs(Objects):
- Motors: The state of the motors including speed, step counts, etc.
- Debug: The debugging state that will enable/disable the respective debugging

Enumerations:
- Directions: The directions that the robot can move which includes diagonal, strafing, and rotations
- Colors: The color of the target boxes (used to make navigation decisions)

Functions:

low level:
- set_direction: sets direction of the motors
- init_timers: initializes counter timers
- set_timers: sets counter timer delays
- set_steps: calculates and sets the number of steps the motor needs turning based on given values in inches or degrees.
- update_motors: updates motor's differential values to allow higher-level motion such as rotation, acceleration, etc

high level:
- correct_angle: rotates the robot so that it is parallel to the grid
- strafe_align: strafes the robot so that it is at the center of the grid
- correct_front: moves front or back so that it is at the center of the grid
- correct_right: makes sure the robot is following the line underneath it
- go: moves with the give Direction, amount (inches/deg), speed (optional), and option to align while moving
- go_const: same as "go" function, but without acceleration/de-acceleration

There are also functionalities for IR sensors, color sensors and high-level navigation which is very specific to competition rules.
