#ifndef RPI_PATHFINDER_I2CDRIVERHELPER_H
#define RPI_PATHFINDER_I2CDRIVERHELPER_H
/**
 * Intended to be roughly Wire.h compatible
 */

#include <cstddef> // missing from i2cDriver
#include <string>
#include "arduino/Print.h"
#include "arduino/Stream.h"

extern "C" {
#include "i2cdriver.h"
};

#include "i2cdriver.h"
#include <string>
#include <vector>

namespace Aperture {
    namespace USB {
        class I2CDriverHelper : public Stream {
        public:
            static const size_t BUFFER_LENGTH = 512;
            const uint8_t OP_WRITE = 1;
            const uint8_t OP_READ = 0;

            I2CDriverHelper();
            I2CDriverHelper(std::string port);

            void sendCommand(std::vector<std::string> command_arguments);
            static void sendCommand(I2CDriver &i2cDriver, std::vector<std::string> command_arguments);

            void begin(uint8_t address);
            void begin(void);
            void begin(int address);
            void beginTransmission(uint8_t address);
            void beginTransmission(int address);
            uint8_t endTransmission(void);
            uint8_t endTransmission(bool sendStop);
            size_t requestFrom(uint8_t address, size_t size, bool sendStop);
            uint8_t status();

            uint8_t requestFrom(uint8_t address, uint8_t quantity);
            uint8_t requestFrom(uint8_t address, uint8_t quantity, bool sendStop);
            uint8_t requestFrom(int address, int quantity);
            uint8_t requestFrom(int address, int quantity, bool sendStop);

            virtual size_t write(uint8_t reg);
            virtual size_t write(const uint8_t * reg, size_t);
            virtual int available(void);
            virtual int read(void);
            virtual int peek(void);
            virtual void flush(void);
            void onReceive(void (*)(int));      // arduino api
            void onReceive(void (*)(size_t));   // legacy esp8266 backward compatibility
            void onRequest(void (*)(void));

            using Print::write;
        private:
            std::string _portName;
            I2CDriver &_i2cDriver;

            uint8_t rxBuffer[BUFFER_LENGTH];
            uint8_t rxBufferIndex;
            uint8_t rxBufferLength;

            uint8_t transmitting = 0;
            uint8_t txAddress = 0;
            uint8_t txBuffer[BUFFER_LENGTH];
            uint8_t txBufferIndex = 0;
            uint8_t txBufferLength = 0;

            static void (*user_onRequest)(void);
            static void (*user_onReceive)(size_t);
            static void onRequestService(void);
            static void onReceiveService(uint8_t*, size_t);
        };
    }
}

#endif //RPI_PATHFINDER_I2CDRIVERHELPER_H
