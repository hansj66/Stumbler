#include "motor.h"

Motor::Motor(MicroBit & bit)
    : _bit(bit)
    , _periodMin(300) // Shorter period == higher motor speed
    , _periodMax(5000)
{
    SetPWMDutyCycle(STEPPER_OFF);
}

void Motor::SetPWMDutyCycle(int value) const
{
    _bit.io.P3.setAnalogValue(value);
    _bit.io.P4.setAnalogValue(value);
}
 
void Motor::SetPWMPeriod(int usValue) const
{
    _bit.io.P3.setAnalogPeriodUs(usValue); 
    _bit.io.P4.setAnalogPeriodUs(usValue);
}

void Motor::SetDirection(STEP_DIRECTION direction) const
{
    _bit.io.P13.setDigitalValue(direction);
    _bit.io.P15.setDigitalValue(direction);
}
  
int Motor::ClampPeriod(int period)
{
    if (period < _periodMin)
        return _periodMin;
    else if (period > _periodMax)
        return _periodMax;
    return period;
}

void Motor::Step(double period)
{
    SetPWMDutyCycle(STEPPER_ON);

    char buf[100];
    if (period > 0)
    {
        SetDirection(REVERSE);
    }
    else 
    {
        SetDirection(FORWARD);
    }
    period = ClampPeriod(static_cast<int>(abs(period)));
    SetPWMPeriod(abs(period));
      
}
