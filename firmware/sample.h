#ifndef _SAMPLE_H
#define _SAMPLE_H

const float pi = 3.1415926;

struct Sample
{
    public:
        Sample(float gx = 0, float ay = 0, float az = 0);
        double GyroRate();
        double AccelerometerAngle();
        
    float _gx;
    float _ay;
    float _az;
};

#endif