#include "LSM9DS1.h"
#include <vector>

namespace LSM9DS1
{

/*
*  Sensor constructor. This will take care of all necessary initialization and sensor calibration
*/    
Sensor::Sensor(MicroBit & bit) : 
    _bit(bit),
    _AG_I2CAddress(0xD6),       // 0xD6 : 0xD4
    _M_I2CAddress(0x3C),        // Magnetometer: 0x3C : 0x38
    _gSensitivity(245.0 / 32768.0),    // Defaulting to 245 degrees pr second
    _aSensitivity(16.0 / 32768.0),       // Defaulting to 8G
    _newGyroDataAvailable(false),
    _ay(0),
    _prev_ay(0),
    _az(0),
    _prev_az(0)
{
    LogDeviceId();
    Init();
    Calibrate();
     
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
void Sensor::Calibrate()
{
    _bit.display.printChar('C'); // 'C' is for "Calibratin'" ;)
    wait(1);
    
    Sample s;
    
    float gx = 0;
    
    int samples = 1000;
    for (int i=0; i<samples; i++)
    {
        ReadRaw(s); 
        gx += s._gx;
    }
    
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
    cmd[1] = 0b01111000; // Decimate every 2 samples. Enable accelerometer Z, Y and X axis
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
    
    cmd[0] = CTRL_REG6_XL;
    cmd[1] = 0b01101100; //952Hz 16G scale. BW: 50Hz
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 

    cmd[0] = CTRL_REG7_XL;
    cmd[1] = 0b00000000; // ODR/50. Default res. Bypass filters
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 

    cmd[0] = CTRL_REG1_G; 
    cmd[1] = 0b11000011;// ODR: 952. Cutoff: 38Hz // 245 DPS
    _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
    
     
    cmd[0] = CTRL_REG3_G;
    cmd[1] = 0b00000000; // Bypass hig pass filter
     _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
     
    cmd[0] = FIFO_CTRL;
    cmd[1] = 0b00000000; // Bypass FIFO
     _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
     
    cmd[0] = CTRL_REG9;
    cmd[1] = 0b00000000; // Disable FIFO
     _bit.i2c.write(_AG_I2CAddress, cmd, 2); 
     
     
}


void Sensor::ReadRaw(Sample & s)
{
    float alpha = 0.3; 
    
    char registerAddress;
  
    registerAddress = OUT_X_L_G;
    _bit.i2c.write(_AG_I2CAddress, &registerAddress, 1, true);
    _bit.i2c.read(_AG_I2CAddress, _buffer, 6);
    registerAddress = OUT_X_L_XL;
    _bit.i2c.write(_AG_I2CAddress, &registerAddress, 1, true);
    _bit.i2c.read(_AG_I2CAddress, _buffer+6, 6);

    int16_t gxRate = (_buffer[0] | (_buffer[1] << 8));
    
    _prev_ay = _ay;
    _prev_az = _az;
    int16_t y = _buffer[8] | (_buffer[9] << 8);
    int16_t z = _buffer[10] | (_buffer[11] << 8);
    
    // Low pass filter for accelerometer (without it, the stepper motor vibrations
    // will cause it to go haywire
    _ay = alpha * y + (1-alpha)*_prev_ay;
    _az = alpha * z + (1-alpha)*_prev_az;
    s._gx = (gxRate -_gxZeroRate)*_gSensitivity;
    s._ay = _ay*_aSensitivity;
    s._az = _az*_aSensitivity;
}

}
