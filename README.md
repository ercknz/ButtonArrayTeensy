# ButtonArrayTeensy
#### By Erick Nunez

### Description
 Teensy program for controlling a 6 push button array for reaching experiment targets.
 This will be used in AIM 1 with the custom robotic arm support and the HapticMASTER.

 Controller send Start and Stop (First and last button presses for experiment).
 Controller runs at 8 milliSecond loop time and streams data every loop.
 Total number of targets can be adjusted in code. 
 Controller will continue until total targets have been pressed.
 Will attempt to randomize targets evenly.

#### Buttons
 - Button 1 = Center target queue button
 - Button 2 = Lower Left Button
 - Button 3 = Lower Right Button
 - Button 4 = Center Indented Button
 - Button 5 = Upper Left Button
 - Button 6 = Upper Right Button
 
#### Targets
 - Target 1 = Lower Left LED
 - Target 2 = Lower Right LED
 - Target 3 = Center LED
 - Target 4 = Upper Left LED
 - Target 5 = Upper Left LED
