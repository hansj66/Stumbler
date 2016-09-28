#include "MicroBit.h"
#include "sensor.h"
#include "motor.h"
#include "PID.h"

MicroBit uBit;

const float pi = 3.1415926;

int main()
{
    uBit.display.scroll("KH!"); 

    // Initialize motors and sensor
    uBit.display.printChar('Y'); // 'Y' is for "Yay !" 
    LSM9DS1::Sensor sensor(uBit);
    uBit.display.printChar(' ');
    Motor motor(uBit);
  
    // Initialize magic
    PID pid;
    pid_zeroize(&pid);
    pid.windup_guard = 0.0; // Magic spell #1
    pid.proportional_gain = 2;
    pid.integral_gain = 0;
    pid.derivative_gain = 0;

    float dt = 0.001; // nonzero, just to avoid blowing up the first PID update
    float angle = 0;
    while (true)
    {
       Timer t;
        t.start();
        LSM9DS1::Sample s = sensor.Read();
        // Gyro unit is DPS. Multiply by time to estimate the delta angle
        // The gyro drifts _significantly_ in a relatively short amount of time, so
        // we can only trust the high frequency component.
        // Oh, well. Let's multiply the DPS value by time in this loop to get an estimated 
        // value for the angle from the gyro (in radians)
        float gyroAngle = s._gx * dt * pi/180;
        
        // Unless you have mounted the accelerometer on a piece of foam, please do so now...
        // They are sensitive to vibrations
        
        // The accelerometer y axis is pointing down and the x-axis is parallel to the wheel axis
        float accelerometerAngle = atan(s._ay / s._az); // convert from radians to degrees
        
        // We'll use a complementary filter for sensor fusion. This will combine the high
        // frequency component of the gyro angle and the low frequency component from
        // the accelerometer angle.
        // The complementary filter performs almost as well as a Kalman filter, but it only
        // consist of one line of code
        angle = 0.98*(angle + gyroAngle) + 0.02*accelerometerAngle;
        
        float error = 0;
        if (angle > 0)
            error = pi/2 - angle;
        else
            error = -(pi/2 + angle);
        pid_update(&pid, error, dt); 
        
        float PWMPeriod = 200 / pid.control; // Because...
        
        motor.Step(PWMPeriod);
        
        t.stop();
        dt = t.read();
    }

    // If main exits, there may still be other fibers running or registered event handlers etc.
    // Simply release this fiber, which will mean we enter the scheduler. Worse case, we then
    // sit in the idle task forever, in a power efficient sleep.
    release_fiber();
}

