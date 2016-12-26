#include "MicroBit.h"
#include "sample.h"
#include "motor.h"
#include "PID.h"
#include "kalman.h"
#include "mbed.h"
#include "BNO055.h"

MicroBit uBit;

Ticker stepInterrupt;

int main()
{
    
    BNO055::Sensor sensor(uBit);
    
    sensor.Calibrate();
 
   // Initialize PID controller
    PID pid;
    pid_zeroize(&pid);
    pid.windup_guard = 0; 
    pid.proportional_gain = 95;
    pid.integral_gain = 0.75;
    pid.derivative_gain = 0.5;
  
   // Initialize motor and set up step interrupt
    Motor motor(uBit);
    stepInterrupt.attach(&motor, &Motor::Step, STEP_PERIOD);  

    char logMessage[200];
    float error = 0;
    float dt;
    float loopTime = 0.003; 
    double heading = 0;
    double roll = 0;
    double pitch = 0;
    while(true)
    {
       Timer t;
        t.start();
      
        sensor.ReadEulerAngles(heading, roll, pitch);
        
        error = 89-roll;
        
        pid_update(&pid, error, dt); 
  
        if (error < 0)
            motor.SetDirection(FORWARD);
        else
            motor.SetDirection(REVERSE);
     
  
        motor.SetSpeed(pid.control);      
       
        t.stop();
        dt = t.read();
        if (dt < loopTime)
            wait(loopTime-dt);
   
        //sprintf(logMessage, "h: %f, r: %f, p: %f\n", heading, roll, pitch);
        //uBit.serial.send(logMessage);
 
    }
        
    release_fiber();
}



