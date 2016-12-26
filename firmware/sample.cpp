#include "sample.h"
#include "MicroBit.h"

Sample::Sample(float gx, float ay, float az) 
            : _gx(gx)
            , _ay(ay)
            , _az(az) 
            {}
   
double Sample::GyroRate()
{
    return -_gx;
}
double Sample::AccelerometerAngle()
{
    return (atan2(_ay, _az)+pi)*360/pi; // Noise-O-rama
}

