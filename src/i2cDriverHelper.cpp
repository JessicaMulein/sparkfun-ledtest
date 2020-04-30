#include "i2cDriverHelper.h"
#include "i2cdriver.h"
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

Aperture::USB::I2CDriverHelper::~I2CDriverHelper() {
    _i2cDriver->connected = 0;
}

Aperture::USB::I2CDriverHelper::I2CDriverHelper(std::string port) : _portName(port) {
    i2c_connect(&_i2cDriver, port.c_str());
}

Aperture::USB::I2CDriverHelper::I2CDriverHelper() {
    Aperture::USB::I2CDriverHelper::I2CDriverHelper("DO029IJ8"); // HARDCODED
}

void Aperture::USB::I2CDriverHelper::sendCommand(std::vector <std::string> command_arguments) {
    return Aperture::USB::i2cDriver::sendCommand(this->_i2cDriver, command_arguments);
}

void Aperture::USB::I2CDriverHelper::sendCommand(I2CDriver &i2cDriver, std::vector <std::string> command_arguments) {
    std::vector<char *> commands_v;
    for (int i = 0; i < command_arguments.size(); i++) {
        std::string item = command_arguments[i];
        char *cstr = command_arguments[i].data();
        commands_v.push_back(cstr);
    }
    commands_v.push_back(nullptr);
    i2c_commands(&i2cDriver, 1, commands_v.data());
    if (i2cDriver.connected != 1) {
        std::cerr << "i2c driver could not be connected" << std::endl;
        exit(1);
    }
}

void Aperture::USB::I2CDriverHelper::begin()
{
    i2c_connect(_i2cDriver, _portName);
    flush();
}

void Aperture::USB::I2CDriverHelper::begin(uint8_t address)
{
    txAddress = address;
    i2c_connect(_i2cDriver, _portName);
    attachSlaveTxEvent(onRequestService);
    attachSlaveRxEvent(onReceiveService);
    flush();
}

void Aperture::USB::I2CDriverHelper::begin(int address)
{
    begin((uint8_t)address);
}

void Aperture::USB::I2CDriverHelper::requestFrom(uint8_t address, size_t size, bool sendStop) {
    i2c_start(_i2cDriver, address, Aperture::USB::I2CDriverHelper::OP_READ);
    if (size > BUFFER_LENGTH) {
        size = BUFFER_LENGTH;
    }
    i2c_read(_i2cDriver, address, rxBuffer, size);
    rxBufferIndex = 0;
    rxBufferLength = size;
    endTransmission(sendStop);
    return size;
}

uint8_t Aperture::USB::I2CDriverHelper::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(address, static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t Aperture::USB::I2CDriverHelper::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(address, static_cast<size_t>(quantity), true);
}

uint8_t Aperture::USB::I2CDriverHelper::requestFrom(int address, int quantity)
{
    return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t Aperture::USB::I2CDriverHelper::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

void Aperture::USB::I2CDriverHelper::beginTransmission(uint8_t address) {
    transmitting = 1;
    txAddress = address;
    txBufferIndex = 0;
    txBufferLength = 0;
}

void Aperture::USB::I2CDriverHelper::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t Aperture::USB::I2CDriverHelper::endTransmission(uint8_t sendStop)
{
    i2c_write(_i2cDriver, txBuffer, txBufferLength);
    if (sendStop) {
        i2c_stop(_i2cDriver);
    }
    txBufferIndex = 0;
    txBufferLength = 0;
    transmitting = 0;
    return ret;
}

uint8_t Aperture::USB::I2CDriverHelper::endTransmission(void)
{
    return endTransmission(true);
}

size_t Aperture::USB::I2CDriverHelper::write(uint8_t data)
{
    if (transmitting)
    {
        if (txBufferLength >= BUFFER_LENGTH)
        {
            setWriteError();
            return 0;
        }
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        txBufferLength = txBufferIndex;
    }
    else
    {
        i2c_write(_i2cDriver, data, 1);
    }
    return 1;
}

size_t Aperture::USB::I2CDriverHelper::write(const uint8_t *data, size_t quantity)
{
    if (transmitting)
    {
        for (size_t i = 0; i < quantity; ++i)
        {
            if (!write(data[i]))
            {
                return i;
            }
        }
    }
    else
    {
        i2c_write(_i2cDriver, data, quantity);
    }
    return quantity;
}

int Aperture::USB::I2CDriverHelper::available(void)
{
    int result = rxBufferLength - rxBufferIndex;

    return result;
}

int Aperture::USB::I2CDriverHelper::read(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }
    return value;
}

int Aperture::USB::I2CDriverHelper::peek(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }
    return value;
}

void Aperture::USB::I2CDriverHelper::flush(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
}

void Aperture::USB::I2CDriverHelper::onReceiveService(uint8_t* inBytes, size_t numBytes)
{
    // don't bother if user hasn't registered a callback
    if (!user_onReceive)
    {
        return;
    }
    // // don't bother if rx buffer is in use by a master requestFrom() op
    // // i know this drops data, but it allows for slight stupidity
    // // meaning, they may not have read all the master requestFrom() data yet
    // if(rxBufferIndex < rxBufferLength){
    //   return;
    // }

    // copy twi rx buffer into local read buffer
    // this enables new reads to happen in parallel
    for (uint8_t i = 0; i < numBytes; ++i)
    {
        rxBuffer[i] = inBytes[i];
    }

    // set rx iterator vars
    rxBufferIndex = 0;
    rxBufferLength = numBytes;

    // alert user program
    user_onReceive(numBytes);
}

void Aperture::USB::I2CDriverHelper::onRequestService(void)
{
    // don't bother if user hasn't registered a callback
    if (!user_onRequest)
    {
        return;
    }

    // reset tx buffer iterator vars
    // !!! this will kill any pending pre-master sendTo() activity
    txBufferIndex = 0;
    txBufferLength = 0;

    // alert user program
    user_onRequest();
}

void Aperture::USB::I2CDriverHelper::onReceive(void (*function)(int))
{
    // arduino api compatibility fixer:
    // really hope size parameter will not exceed 2^31 :)
    static_assert(sizeof(int) == sizeof(size_t), "something is wrong in Arduino kingdom");
    user_onReceive = reinterpret_cast<void(*)(size_t)>(function);
}

void Aperture::USB::I2CDriverHelper::onReceive(void (*function)(size_t))
{
    user_onReceive = function;
}

void Aperture::USB::I2CDriverHelper::onRequest(void (*function)(void))
{
    user_onRequest = function;
}