#ifndef _SAMPLE_H
#define _SAMPLE_H

namespace LSM9DS1
{

struct Sample
{
    public:
        Sample(float gx, float gy, float gz, float ax, float ay, float az) 
            : _gx(gx)
            , _gy(gy)
            , _gz(gz)
            , _ax(ax)
            , _ay(ay)
            , _az(az) 
            {}
    float _gx;
    float _gy;
    float _gz;
    float _ax;
    float _ay;
    float _az;
};

}

#endif