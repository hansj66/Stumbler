#ifndef _BNO055_h_
#define _BNO055_h_

#include "MicroBit.h"

// Registers
#define CHIP_ID_REGISTER          0x00
#define PAGE_ID_REGISTER          0x07
#define OPR_MODE_REGISTER         0x3d
#define UNIT_SELECTION_REGISTER   0x3b
#define EULER_H_REGISTER_LSB      0x1a
#define EULER_H_REGISTER_MSB      0x1b
#define CALIBRATION_STATUS        0x35


// IDs
#define BNO055_CHIP_ID        0xa0    
#define BNO055_ACC_ID         0xfb    
#define BNO055_MAG_ID         0x32    
#define BNO055_GYRO_ID         0x0f    

// Units
#define UNIT_ACC_MSS            0x00 
#define UNIT_GYR_DPS            0x00 
#define UNIT_EULER_DEG          0x00 
#define UNIT_TEMP_C             0x00 
#define UNIT_ORI_WIN            0x00 


#define NDOF_FUSION_MODE 0x0C
#define IMU_FUSION_MODE 0x08


namespace BNO055
{


class Sensor
{
    public:
        Sensor(MicroBit & bit);
        void Calibrate();

        void ReadEulerAngles(double & heading, double & roll, double & pitch);
 
    private:
        MicroBit & _bit;
        char _buffer[12];
        uint8_t _I2CAddress;
        
        void LogDeviceId();
        void SelectPage(uint8_t page);
        void SelectNDOFFusion();
        void SelectUnits();
        void Init();
        void CheckID();
        uint8_t CalibrationStatus(void);
        
        uint8_t _currentPage;
};

}
    
#endif