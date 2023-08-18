#include "MODULE_DIRECT_STEPMOTOR.h"

uint8_t DIRECT_STEPMOTOR::writeByte(uint8_t addr, uint8_t reg, uint8_t data) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->write(data);
    return _wire->endTransmission();
}

uint8_t DIRECT_STEPMOTOR::readByte(uint8_t addr, uint8_t reg, uint8_t* data) {
    uint8_t result;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    result = _wire->endTransmission();
    if (result != 0) {
        return result;
    }
    if (_wire->requestFrom((int)addr, 1) == 0) {
        return 4;
    }
    *data = _wire->read();
    return 0;
}

bool DIRECT_STEPMOTOR::init(TwoWire& wire_in, uint8_t addr) {
    _wire = &wire_in;
    _addr = addr;

    // IO 0 ~ 3 input, 4 ~ 7 out_put;
    return writeByte(_addr, 0x03, 0x0f) == 0;
}

void DIRECT_STEPMOTOR::setMicrostepResolution(
    MicrostepResolution_t micro_step) {
    uint8_t reg_data = 0x00;
    readByte(_addr, 0x01, &reg_data);
    reg_data &= 0x1f;
    reg_data |= micro_step << 5;
    writeByte(_addr, 0x01, reg_data);
}

void DIRECT_STEPMOTOR::enableMotor(uint8_t en) {
    uint8_t reg_data = 0x00;
    readByte(_addr, 0x01, &reg_data);
    reg_data &= 0xef;
    if (en == 0) {
        reg_data |= 0x10;
    }
    writeByte(_addr, 0x01, reg_data);
}

void DIRECT_STEPMOTOR::getExtIOStatus(uint8_t* status) {
    uint8_t io_status = 0x00;
    if (readByte(_addr, 0x00, &io_status) != 0) {
        return;
    }

    for (uint8_t i = 0; i < 4; i++) {
        ext_io_status[i] = (io_status & (0x01 << i)) ? 1 : 0;
    }

    if (status != NULL) {
        *status = io_status;
    }
}

void DIRECT_STEPMOTOR::getFaultStatus(uint8_t* status) {
    uint8_t fault_status = 0x00;
    if (readByte(_addr, 0x04, &fault_status) != 0) {
        return;
    }

    for (uint8_t i = 0; i < 3; i++) {
        fault_io_status[i] = (fault_status & (0x01 << i)) ? 1 : 0;
    }

    if (status != NULL) {
        *status = fault_status;
    }
}

void DIRECT_STEPMOTOR::resetMotor(uint8_t resmtr, uint8_t en) {
    uint8_t reg_data = 0x00;
    readByte(_addr, 0x05, &reg_data);
    reg_data &= 0x07;
    if (en)
        reg_data &= ~(0x01 << resmtr);
    else
        reg_data |= (0x01 << resmtr);
    writeByte(_addr, 0x05, reg_data);
}
