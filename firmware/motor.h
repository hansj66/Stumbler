#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "MicroBit.h"

const int STEPPER_ON = 512;  // 50 % duty cycle makes a nice step pulse
const int STEPPER_OFF = 0;

typedef enum 
{
    FORWARD = 1,
    REVERSE = 0
} STEP_DIRECTION;

class Motor
{
public:
    Motor(MicroBit & bit);
    void Step(double period);

private:
    MicroBit & _bit;
    void SetPWMDutyCycle(int value) const;
    void SetPWMPeriod(int usValue) const;
    void SetDirection(STEP_DIRECTION direction) const;
    
    // Non-insane analog period range is 330 - 5000'ish
    int ClampPeriod(int period);
    int _periodMin;
    int _periodMax;
};


#endif
