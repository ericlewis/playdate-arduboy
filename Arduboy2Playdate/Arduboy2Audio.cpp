/**
 * @file Arduboy2Audio.cpp
 * \brief
 * The Arduboy2Audio class for speaker and sound control.
 */

#include "Arduboy2.h"

bool Arduboy2Audio::audio_enabled = false;

void Arduboy2Audio::on()
{
    audio_enabled = true;
}

void Arduboy2Audio::off()
{
    audio_enabled = false;
}

void Arduboy2Audio::toggle()
{
    if (audio_enabled)
        off();
    else
        on();
}

void Arduboy2Audio::saveOnOff()
{
    // TODO: implement, EEPROM.update(Arduboy2Base::eepromAudioOnOff, audio_enabled);
}

void Arduboy2Audio::begin()
{
// TODO: implement
//    if (EEPROM.read(Arduboy2Base::eepromAudioOnOff))
//        on();
//    else
//        off();
    on();
}

bool Arduboy2Audio::enabled()
{
    return audio_enabled;
}