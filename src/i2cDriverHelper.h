#ifndef RPI_PATHFINDER_I2CDRIVERHELPER_H
#define RPI_PATHFINDER_I2CDRIVERHELPER_H

#include <cstddef> // missing from i2cDriver
#include <string>

extern "C" {
#include "i2cdriver.h"
};

#include "i2cdriver.h"
#include <string>
#include <vector>

namespace Aperture {
    namespace USB {
        class I2CDriverHelper {
        public:
            static const size_t BUFFER_LENGTH = 512;
            const uint8_t OP_WRITE = 1;
            const uint8_t OP_READ = 0;
            I2CDriverHelper();
            I2CDriverHelper(std::string port);
            void sendCommand(std::vector<std::string> command_arguments);
            static void sendCommand(I2CDriver &i2cDriver, std::vector<std::string> command_arguments);
            void beginTransmission(uint8_t address);
            void write(uint8_t reg);
            void endTransmission(bool sendStop);
            void requestFrom(uint8_t address, size_t size, bool sendStop);
        private:
            I2CDriver &_i2cDriver;
            uint8_t rxBuffer[BUFFER_LENGTH];
        };
    }
}

#endif //RPI_PATHFINDER_I2CDRIVERHELPER_H
