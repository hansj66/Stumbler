#include "BNO055.h"
#include <vector>

namespace BNO055
{
/*
*  Sensor constructor. This will take care of all necessary initialization and sensor calibration
*/    
Sensor::Sensor(MicroBit & bit) : 
    _bit(bit),
    _I2CAddress(0x52),  // 0x28 : 0x29
    _currentPage(0xFF)       
 {
    LogDeviceId();
    Init();
 }
 
 /*
 *  Writes out accelerometer/gyro and magnetometer device IDs to the serial port. 
 *  This is just a sanity check. If you get a 104 (0x68) / 61 (0x3D) response, you're good to go.
 */
void Sensor::LogDeviceId()
{
      SelectPage(0); 
 
    char error[200];
    char cmd[7] = {CHIP_ID_REGISTER, 0, 0, 0, 0, 0, 0};
    _bit.i2c.write(_I2CAddress, cmd, 1, true);
    _bit.i2c.read(_I2CAddress, cmd, 7, false);
    
    if (cmd[0] == BNO055_CHIP_ID) {
        _bit.serial.send("BNO055 CHIP ID - OK\n");
    } else {
        sprintf(error, "BNO055 CHIP ID - ERROR. Got: %d\n", cmd[0]);
        _bit.serial.send(error);
    }
    if (cmd[1] == BNO055_ACC_ID) {
        _bit.serial.send("BNO055 ACCELEROMETER ID - OK\n");
    } else {
        sprintf(error, "BNO055 ACCELEROMETER ID - ERROR. Got: %d\n", cmd[1]);
        _bit.serial.send(error);
    }
    if (cmd[2] == BNO055_MAG_ID) {
        _bit.serial.send("BNO055 MAGNETOMETER ID - OK\n");
    } else {
        sprintf(error, "BNO055 ACCELERMAGNETOMETEROMETER ID - ERROR. Got: %d\n", cmd[2]);
        _bit.serial.send(error);
    }
    if (cmd[3] == BNO055_GYRO_ID) {
      _bit.serial.send("BNO055 GYRO ID - OK\n");
    } else {
        sprintf(error, "BNO055 GYRO ID - ERROR. Got: %d\n", cmd[3]);
        _bit.serial.send(error);
    }
}

uint8_t Sensor::CalibrationStatus(void)
{
    SelectPage(0);
    char cmd[1] = {CALIBRATION_STATUS};
    _bit.i2c.write(_I2CAddress, cmd, 1, true);
    _bit.i2c.read(_I2CAddress, cmd, 1, false);
    return cmd[0];
}

void Sensor::Calibrate()
{
    _bit.display.scroll("CALIBRATE");
    while  (true) {
        uint8_t status = (CalibrationStatus() & 0b11000000) >> 6;
        _bit.display.scroll(status);
        if (3 == status)
            break;
        wait(0.1);
    }
    
    _bit.display.scroll("OK");
    
    for (int i=5; i>= 0; i--) {
        _bit.display.printChar(i+48);
        wait(0.5);
    }
  
    
}


void Sensor::SelectNDOFFusion()
{
   SelectPage(0); 
    char cmd[2] = { OPR_MODE_REGISTER, NDOF_FUSION_MODE };
    _bit.i2c.write(_I2CAddress, cmd, 2); 
}
   
void Sensor::SelectUnits()
{
     SelectPage(0);
     char cmd[2] = {UNIT_SELECTION_REGISTER, UNIT_ORI_WIN + UNIT_ACC_MSS + UNIT_GYR_DPS + UNIT_EULER_DEG + UNIT_TEMP_C};
    _bit.i2c.write(_I2CAddress, cmd, 2, false);
}

void Sensor::Init()
{
    SelectUnits();
    SelectNDOFFusion();
}

void Sensor::SelectPage(uint8_t page)
{
    if (page != _currentPage)
    {
        char cmd[2];
        cmd[0] = PAGE_ID_REGISTER;
        if (page == 1) {
            cmd[1] = 1;  // select page 1
        } else {
            cmd[1] = 0;  // select page 0
        }
        _bit.i2c.write(_I2CAddress, cmd, 2, false);
        _currentPage = page; // Happy path ;)
    }
}

void Sensor::ReadEulerAngles(double & heading, double & roll, double & pitch)
{
    char cmd[6] = {EULER_H_REGISTER_LSB, 0, 0, 0, 0, 0};
    int16_t _heading;
    int16_t _roll;
    int16_t _pitch;
    SelectPage(0);
    _bit.i2c.write(_I2CAddress, cmd, 1, true);
    _bit.i2c.read(_I2CAddress, cmd, 6, false);
    _heading = cmd[1] << 8 | cmd[0];
    _pitch = cmd[3] << 8 | cmd[2];
    _roll = cmd[5] << 8 | cmd[4];
    heading = (double)_heading / 16;
    roll = (double)_roll / 16;
    pitch = (double)_pitch / 16;
}

}
