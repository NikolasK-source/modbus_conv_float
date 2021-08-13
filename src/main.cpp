#include "split_string.hpp"

#include "sysexits.h"
#include <iostream>
#include <string>

int main(int argc, char **) {
    if (argc != 1) {
        std::cerr << "usage: modbus_conv_float" << std::endl;
        std::cerr << "  Converts float values to use them with stdin_to_modbus_shm." << std::endl;
        std::cerr << "  If line starts with 'f:', the 'f:' is removed and the float value is converted to two hex "
                     "registers."
                  << std::endl;
        std::cerr << "  All other lines are passed through." << std::endl;
        std::cerr << std::endl;
        std::cerr << "  example:" << std:: endl;
        std::cerr << "    input : f:ao:0xcf:123.4" << std::endl;
        std::cerr << "    output: ao:0xcf:0x42f6" << std::endl;
        std::cerr << "            ao:0xd0:0xcccd" << std::endl;
                exit(EX_USAGE);
    }

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        if (line.size() >= 2 && line[0] == 'f' && line[1] == ':') {
            line.erase(0, 2);

            auto split_line = split_string(line, ':');
            if (split_line.size() != 3) {
                std::cerr << "discarded invalid line '" << line << "'" << std::endl;
                continue;
            }

            float f;
            try {
                f = std::stof(split_line[2]);
            } catch (const std::exception &) {
                std::cerr << "discarded invalid line '" << line << "': unable to parse '" << split_line[2]
                          << "' as float" << std::endl;
                continue;
            }

            std::size_t addr;
            try {
                addr = std::stoull(split_line[1], nullptr, 0);
            } catch (const std::exception &) {
                std::cerr << "discarded invalid line '" << line << "': unable to parse '" << split_line[1]
                          << "' as address" << std::endl;
                continue;
            }

            static_assert(sizeof(float) == 2 * sizeof(uint16_t));
            void *         v  = &f;
            const uint16_t lo = *reinterpret_cast<uint32_t *>(v) & 0xFFFF;
            const uint16_t hi = (*reinterpret_cast<uint32_t *>(v)) >> 16 & 0xFFFF;

            std::cout << split_line[0] << ":0x" << std::hex << addr << ":0x" << std::hex << hi << std::endl;
            std::cout << split_line[0] << ":0x" << std::hex << addr + 1 << ":0x" << std::hex << lo << std::endl;
        } else {
            std::cout << line << std::endl;
        }
    }
}
