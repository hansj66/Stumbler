#ifndef _LSM9DS1_h_
#define _LSM9DS1_h_

#include "MicroBit.h"
#include "sample.h"

namespace LSM9DS1
{
// Control registers

const char OUT_X_L_XL = 0x28;
const char OUT_X_L_G = 0x18;

// const char WHO_AM_I_AG_RSP = 0x68;
//const char WHO_AM_I_M_RSP = 0x3D;

const char CTRL_REG1_M = 0x20;
const char CTRL_REG5_XL = 0x1F;
const char CTRL_REG6_XL = 0x20;
const char CTRL_REG7_XL = 0x21;
const char CTRL_REG8 = 0x22;

const char CTRL_REG1_G = 0x10;
const char CTRL_REG2_G = 0x11;
const char CTRL_REG3_G = 0x12;
const char ORIENT_CFG_G = 0x13;
const char CTRL_REG4 = 0x1E;

const char STATUS_REG = 0x17;

const char WHO_AM_I_M = 0x0F;
const char WHO_AM_I_XG = 0x0F;

const char FIFO_CTRL = 0x2E;
const char CTRL_REG9 =0x23;


class Sensor
{
    public:
        Sensor(MicroBit & bit);
        void ReadRaw(Sample & s);
   

    private:
        MicroBit & _bit;
        char _buffer[12];
        bool _newGyroDataAvailable;
        uint8_t _AG_I2CAddress;
        uint8_t _M_I2CAddress;
        float _gSensitivity;
        float _aSensitivity;
          
        void Init();
        void LogDeviceId();
        void ResetDevice();
        void Calibrate();
        
        float _ay;
        float _prev_ay;
        float _az;
        float _prev_az;
        
        int16_t _gxZeroRate;
};

}
    
#endif