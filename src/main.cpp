#include "split_string.hpp"

#include "sysexits.h"
#include <iostream>
#include <string>

int main(int argc, char **argv) {
    if (argc != 1) {
        std::string arg(argv[1]);

        if (arg == "-v" || arg == "--version") {
            std::cout << "modbus_conv_float " << PROJECT_VERSION << std::endl;
            exit(EX_OK);
        }

        const bool    ARG_HELP = arg == "--help" || arg == "-h";
        std::ostream &out      = ARG_HELP ? std::cout : std::cerr;

        out << "usage: modbus_conv_float [OPTION...]" << std::endl;
        out << std::endl;
        out << "    -h --help     print this message." << std::endl;
        out << "    -v --version  print version information." << std::endl;
        out << std::endl;
        out << "  Converts float values to use them with stdin_to_modbus_shm." << std::endl;
        out << "  If line starts with 'f:', the 'f:' is removed and the float value is converted to two hex "
               "registers."
            << std::endl;
        out << "  All other lines are passed through." << std::endl;
        out << std::endl;
        out << "  example:" << std::endl;
        out << "    input : f:ao:0xcf:123.4" << std::endl;
        out << "    output: ao:0xcf:0x42f6" << std::endl;
        out << "            ao:0xd0:0xcccd" << std::endl;
        out << std::endl;
        out << std::endl;
        out << "MIT License:" << std::endl;
        out << std::endl;
        out << "Copyright (c) 2021-2022 Nikolas Koesling" << std::endl;
        out << std::endl;
        out << "Permission is hereby granted, free of charge, to any person obtaining a copy" << std::endl;
        out << "of this software and associated documentation files (the \"Software\"), to deal" << std::endl;
        out << "in the Software without restriction, including without limitation the rights" << std::endl;
        out << "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell" << std::endl;
        out << "copies of the Software, and to permit persons to whom the Software is" << std::endl;
        out << "furnished to do so, subject to the following conditions:" << std::endl;
        out << std::endl;
        out << "The above copyright notice and this permission notice shall be included in all" << std::endl;
        out << "copies or substantial portions of the Software." << std::endl;
        out << std::endl;
        out << "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR" << std::endl;
        out << "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY," << std::endl;
        out << "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE" << std::endl;
        out << "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER" << std::endl;
        out << "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM," << std::endl;
        out << "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE" << std::endl;
        out << "SOFTWARE." << std::endl;
        exit(ARG_HELP ? EX_OK : EX_USAGE);
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
            void          *v  = &f;
            const uint16_t LO = static_cast<uint16_t>(*reinterpret_cast<uint32_t *>(v) & 0xFFFF);
            const uint16_t HI = static_cast<uint16_t>((*reinterpret_cast<uint32_t *>(v)) >> 16 & 0xFFFF);

            std::cout << split_line[0] << ":0x" << std::hex << addr << ":0x" << std::hex << HI << std::endl;
            std::cout << split_line[0] << ":0x" << std::hex << addr + 1 << ":0x" << std::hex << LO << std::endl;
        } else {
            std::cout << line << std::endl;
        }
    }
}
