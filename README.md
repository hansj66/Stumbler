# Stumbler
3D Printed self balancing robot. Hardware stack consist of a BBC Micro:Bit, DRV882, BNO055/LSM9DS1, Nema17 steppers

The firmware can be compiled using the mBed online compiler.

The current version of the firmware uses the the BNO055 IMU. There is also code for integrating with LSM9DS1, but I recommend using a sensor that also does the sensor fusion (Like the BNO055). This makes the implementation a lot easier, since you don't have to worry about accelerometer noise filtering and kalmanfiltering / sensor fusion.  

##Videos:
* First test (not having executed the calibration steps): https://www.youtube.com/watch?v=KoEledFhL0o
* Second test (after calibration - balancing on the sofa :)): https://www.youtube.com/watch?v=54taJUBaHX8



##Parts list:

* 1 x BBC Micro:Bit
* 2 x DRV8825 stepper controller
* 1 x LSM9DS1 breakout (Sparkfun)
* 1 x 3V regulator + capacitors (100uF/10uF)
* 8 x AA Battery pack
* 8 x NiMH batteries + charger
* 9V battery clip
* On/off pushbutton switch (12mm mounting)
* RC tyres. ID: X, width: Y
* 2 x lengths of 12MM OD carbon fiber tubing. 
* 12 x 6mm M3 screws
* 6 x 20mm M3 screws
* 4 x M4 set screws
* 1 x Inventor’s KIT for BBC micro:bit + optional (Kitronic)
* 1 x Perma Proto Half-sized Breadboard (Adafruit)
* 2 x strip of 0.1” female header (Adafruit)
* 1 x strip of 0.1” breakaway pin strip (Adafruit)
* Hook up wire. 
* 2 xModel Car Racing Tyres W: 26mm: ID: 50mm 


![Electronics](http://www.timeexpander.com/wordpress/wp-content/uploads/IMG_3272.jpg)

![Electronics](http://www.timeexpander.com/wordpress/wp-content/uploads/IMG_3271-e1474837176267-590x357.jpg)

![Electronics](http://www.timeexpander.com/wordpress/wp-content/uploads/IMG_3277.jpg)

Hans Jørgen Grimstad
www.TimeExpander.com
