#include "mbed.h"
#include "motor.h"


Motor::Motor(MicroBit & bit)
    : _bit(bit),
      _speed(0),
      _targetSpeed(0)
{
    // 50% duty cycle will make a nice step pulse
    SetPWMDutyCycle(900);
}

void Motor::SetSpeed(float targetSpeed)
{
    _targetSpeed = targetSpeed;
}

float Motor::Speed()
{
    return _speed;
}

void Motor::Step()
{
    if ((_speed - _targetSpeed) > ACCELERATION)
        _speed -= ACCELERATION;
    else if ((_speed - _targetSpeed) < -ACCELERATION)
        _speed += ACCELERATION;
    else
        _speed = _targetSpeed;
 
    if ((_speed < 0.00005) && (_speed > -0.00005))
        _speed = 0.00005; // Prevent int overflow in SetPWMPeriod. 
        
    SetPWMPeriod(abs(100000/_speed));
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
    _bit.io.P8.setDigitalValue(direction);
    _bit.io.P16.setDigitalValue(direction);
}
  
