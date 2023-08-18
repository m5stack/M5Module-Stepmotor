#ifndef _MODULE_STEPMOTOR_H_
#define _MODULE_STEPMOTOR_H_

#include "Wire.h"

class Module_Stepmotor {
   private:
    uint8_t writeByte(uint8_t addr, uint8_t reg, uint8_t data);

    uint8_t readByte(uint8_t addr, uint8_t reg, uint8_t* data);

   public:
    typedef enum {
        kMicrosteps1  = 0x00,
        kMicrosteps2  = 0x04,
        kMicrosteps4  = 0x02,
        kMicrosteps8  = 0x06,
        kMicrosteps16 = 0x01,
        kMicrosteps32 = 0x07,
    } MicrostepResolution_t;

    uint8_t _addr;
    uint8_t ext_io_status[4];
    uint8_t fault_io_status[3];
    TwoWire* _wire;

   public:
    bool init(TwoWire& wire_in = Wire, uint8_t addr = 0x27);

    void setMicrostepResolution(MicrostepResolution_t micro_step);

    void enableMotor(uint8_t en);

    void getExtIOStatus(uint8_t* status = NULL);

    void getFaultStatus(uint8_t* status = NULL);

    void resetMotor(uint8_t resmtr, uint8_t en);
};

#endif
