/*
  EEPROM.h - EEPROM library
  Original Copyright (c) 2006 David A. Mellis.  All right reserved.
  New version by Christopher Andrews 2015.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef EEPROM_h
#define EEPROM_h

extern "C" {
#include <stdint.h>
}

/***
    EEPROMClass class.
    This object represents the entire EEPROM space.
    It wraps the functionality of EEPtr and EERef into a basic interface.
    This class is also 100% backwards compatible with earlier Arduino core releases.
***/

#include <string>

struct EEPROMClass {

    //Basic user access methods.

    uint8_t read(int idx)
    {
        // TODO: revisit
//        std::string name = std::to_string(idx);
//        char* ccx = new char[name.length() + 1];
//        std::copy(name.begin(), name.end(), ccx);
//        pd->file->open(ccx, kFileReadData);
        return 0;
    }
    void write(int idx, uint8_t val)
    {

    }
    void update(int idx, uint8_t val)
    {

    }

    uint16_t length()
    {
        return 0;// TODO: E2END + 1;
    }

    //Functionality to 'get' and 'put' objects to and from EEPROM.
    template< typename T > T &get(int idx, T &t)
    {
        // TODO: revisit this
//        uint8_t *ptr = (uint8_t *) &t;
//        std::string name = std::to_string(idx);
//        char* ccx = new char[name.length() + 1];
//        std::copy(name.begin(), name.end(), ccx);
//        SDFile *file = pd->file->open(ccx, kFileReadData);
//
//        FileStat *stat = nullptr;
//        pd->file->stat(ccx, stat);
//        if (stat) {
//            pd->file->read(file, ptr, stat->size);
//            pd->file->close(file);
//            return t;
//        }

        return t;
    }

    template< typename T > const T &put(int idx, const T &t)
    {
        // TODO: revisit this
//        const uint8_t *ptr = (const uint8_t *) &t;
//        std::string name = std::to_string(idx);
//        char* ccx = new char[name.length() + 1];
//        std::copy(name.begin(), name.end(), ccx);
//        SDFile *file = pd->file->open(ccx, kFileWrite);
//        pd->file->write(file, ptr, sizeof(T));
//        pd->file->close(file);
        return t;
    }
};

static EEPROMClass EEPROM;
#endif