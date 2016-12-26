#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "MicroBit.h"

// If you change step frequency, you will implicitly also change acceleration
// If you change acceleration, you will - wait for it - change acceleration
const float STEP_FREQUENCY = 4000; // kHz
const float STEP_PERIOD = 1.0/STEP_FREQUENCY; 
const float ACCELERATION = 8;

typedef enum 
{
    FORWARD = 1,
    REVERSE = 0
} STEP_DIRECTION;

class Motor
{
public:
    Motor(MicroBit & bit);
    void Step();
    void SetSpeed(float targetSpeed);
    float Speed();
    void SetDirection(STEP_DIRECTION direction) const;
 
private:
    MicroBit & _bit;
    void SetPWMDutyCycle(int value) const;
    void SetPWMPeriod(int usValue) const;
    
    float _speed;
    float _targetSpeed;
};


#endif