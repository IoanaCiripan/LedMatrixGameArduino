# LedMatrixGameArduino
#Components
 - Arduino UNO
 - 2 x Breadboard
 - 1 Mini Breadboard
 - 8 X 8 LED Matrix
 - MAX7219 Driver
 - 16x2 LCD Display
 - Joystick
 - 100k resistence
 - 220 resistence
 - 100UF 50V
 - 104 ceramic capacitor
 - Wires

#Gameplay
The game scope is to hit as much monsters as you can.
You can move on the X axis and shot the monsters with the joystick.
The more points you get, the faster is falling the monster (at each 10 points, the speed increase with 20 miliseconds).
Initial, you have 2 lives, and you can die in two ways: lose the lives beacuse the monsters hits you or you let too many pices of monsters to fall, (the maximum number accepted is 30).
The game display on LCD the score, lives and the maximum number of how much pices of monster you can miss.
Also, using the EEPROM, the game keep a highscore.
