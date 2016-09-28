#include "sensor.h"

namespace LSM9DS1
{

/*
*  Sensor constructor. This will take care of all necessary initialization and sensor calibration
*/    
Sensor::Sensor(MicroBit & bit) : 
    _bit(bit),
    _AG_I2CAddress(0xD6),       // 0xD6 : 0xD4
    _M_I2CAddress(0x3C),        // Magnetometer: 0x3C : 0x38
    _gScale(245.0 / 32768.0),    // Defaulting to 245 degrees pr second
    _aScale(2.0 / 32768.0)       // Defaulting to 2G scale
{
    LogDeviceId();
    Init();
    CalibrateGyro();
 }
 
 /*
 *  Writes out accelerometer/gyro and magnetometer device IDs to the serial port. 
 *  This is just a sanity check. If you get a 104 (0x68) / 61 (0x3D) response, you're good to go.
 */
void Sensor::LogDeviceId()
{
    char IDString[50];
    char cmd[2] = { WHO_AM_I_XG, 0 };
    _bit.i2c.write(_AG_I2CAddress, cmd, 1, true);
    _bit.i2c.read(_AG_I2CAddress, cmd+1, 1);
    uint8_t accelerometerId = cmd[1];
    
    cmd[0] = WHO_AM_I_M;
    _bit.i2c.write(_M_I2CAddress, cmd, 1, true);
    _bit.i2c.read(_M_I2CAddress, cmd+1, 1);
    uint8_t magnetometerId = cmd[1];
    
    sprintf(IDString, "\r\nAccelerator / Gyro ID: %d\r\nMagnetometer ID: %d\r\n", accelerometerId, magnetometerId);
    _bit.serial.send(IDString);
}

/*
*  All gyros will have different offsets. We will run through a 1000 samples and average the results.
*  We can then use these values as offsets for our gyro samples
*/
void Sensor::CalibrateGyro()
{
    _bit.display.printChar('C'); // 'C' is for "Calibratin'" ;)
    wait(1);

    int32_t gx = 0;
    int32_t gy = 0;
    int32_t gz = 0;

    int samples = 1000;
    for (int i=0; i<samples; i++)
    {
        char registerAddress = OUT_X_L_G;
        _bit.i2c.write(_AG_I2CAddress, &registerAddress, 1, true);
        _bit.i2c.read(_AG_I2CAddress, _buffer, 6);
    
        gx += _buffer[0] | (_buffer[1] << 8);
        gy += _buffer[2] | (_buffer[3] << 8);
        gz += _buffer[4] | (_buffer[5] << 8);
    }
    _gxOffset = gx / samples;
    _gyOffset = gy / samples;
    _gzOffset = gz / samples;
    
    char buf[100];
    sprintf(buf, "Gyro calibration values [%d, %d, %d]\r\n" 
                    , _gxOffset
                    , _gyOffset
                    , _gzOffset);
    _bit.serial.send(buf);
}


void Sensor::ResetDevice()
{
    char cmd[2];
    cmd[0] = CTRL_REG8;
    cmd[1] = 0b10000101; // SW reset
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
}


void Sensor::Init()
{
    char cmd[2];

    cmd[0] = CTRL_REG4;
    cmd[1] = 0b00111000; // enable gyro z y x
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
    
    cmd[0] = CTRL_REG5_XL;
    cmd[1] = 0b00111000; // No decimation. Enable accelerometer Z, Y and X axis
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
    
    cmd[0] = CTRL_REG6_XL;
    cmd[1] = 0b11000000; // ODR: 952 Hz. 2G scale. BW determined by ODR. 408Hz anti aliasing
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 

    cmd[0] = CTRL_REG7_XL;
    cmd[1] = 0b11100000; // High resolution mode. High/lowpass filter: ODR/400
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 

    cmd[0] = CTRL_REG1_G; // ODR: 952Hz. Cutoff: 100Hz
    cmd[1] = 0b01000000;
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
}

Sample Sensor::Read()
{
    char registerAddress = OUT_X_L_G;
    _bit.i2c.write(_AG_I2CAddress, &registerAddress, 1, true);
    _bit.i2c.read(_AG_I2CAddress, _buffer, 6);
    registerAddress = OUT_X_L_XL;
    _bit.i2c.write(_AG_I2CAddress, &registerAddress, 1, true);
    _bit.i2c.read(_AG_I2CAddress, _buffer+6, 6);

    _gx = (_buffer[0] | (_buffer[1] << 8)) - _gxOffset;
    _gy = (_buffer[2] | (_buffer[3] << 8)) - _gyOffset;
    _gz = (_buffer[4] | (_buffer[5] << 8)) - _gzOffset;
    _ax = _buffer[6] | (_buffer[7] << 8);
    _ay = _buffer[8] | (_buffer[9] << 8);
    _az = _buffer[10] | (_buffer[11] << 8);
    
    return Sample(_gx*_gScale
                , _gy*_gScale
                , _gz*_gScale
                , _ax*_aScale
                , _ay*_aScale
                , _az*_aScale);
 }
    

}
