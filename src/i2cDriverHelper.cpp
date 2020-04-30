#include "i2cDriverHelper.h"
#include "i2cdriver.h"
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

Aperture::USB::I2CDriverHelper::I2CDriverHelper(std::string port) {
    i2c_connect(&_i2cDriver, port.c_str());
}

Aperture::USB::I2CDriverHelper::I2CDriverHelper() {
    Aperture::USB::I2CDriverHelper::I2CDriverHelper("DO029IJ8"); // HARDCODED
}

void Aperture::USB::I2CDriverHelper::sendCommand(std::vector<std::string> command_arguments) {
    return Aperture::USB::i2cDriver::sendCommand(this->_i2cDriver, command_arguments);
}

void Aperture::USB::I2CDriverHelper::sendCommand(I2CDriver &i2cDriver, std::vector<std::string> command_arguments) {
    std::vector<char*> commands_v;
    for (int i=0; i < command_arguments.size(); i++) {
        std::string item = command_arguments[i];
        char* cstr = command_arguments[i].data();
        commands_v.push_back(cstr);
    }
    commands_v.push_back(nullptr);
    i2c_commands(&i2cDriver, 1, commands_v.data());
    if (i2cDriver.connected != 1) {
        std::cerr << "i2c driver could not be connected" << std::endl;
        exit(1);
    }
}

void Aperture::USB::I2CDriverHelper::beginTransmission(uint8_t address, uint8_t op = 0) {
    i2c_start(_i2cDriver, address, op); // 0 = write, 1 = read
}

void Aperture::USB::I2CDriverHelper::write(uint8_t reg) {
    i2c_write(_i2cDriver, reg, OP_WRITE);
}

void Aperture::USB::I2CDriverHelper::endTransmission(bool sendStop) {
    if (sendStop) {
        i2c_stop(_i2cDriver);
    }
}

void Aperture::USB::I2CDriverHelper::requestFrom(uint8_t address, size_t size, bool sendStop) {
    i2c_start(_i2cDriver, address, OP_READ);
    if (size > BUFFER_LENGTH)
    {
        size = BUFFER_LENGTH;
    }
    i2c_read(_i2cDriver, address, rxBuffer, size);
    endTransmission(sendStop);
    return size;
}