/**
 * @file Arduboy2Beep.cpp
 * \brief
 * Classes to generate simple square wave tones on the Arduboy speaker pins.
 */

#include "Arduboy2Beep.h"

// Speaker pin 1, Timer 3A, Port C bit 6, Arduino pin 5

uint8_t BeepPin1::duration = 0;
PDSynth* synth1 = nullptr;
PDSynth* synth2 = nullptr;

void BeepPin1::begin()
{
    synth1 = pd->sound->synth->newSynth();
    pd->sound->source->setVolume((SoundSource*)synth1, 0.3, 0.3);
    pd->sound->synth->setWaveform(synth1, kWaveformSquare);
}

void BeepPin1::tone(uint16_t count)
{
    tone(count, -1);
}

void BeepPin1::tone(uint16_t count, uint8_t dur)
{
    duration = dur;
    pd->sound->synth->playNote(synth1, count / 60, 1, dur, 0);
}

void BeepPin1::timer()
{
    if (duration && (--duration == 0)) {
        pd->sound->synth->stop(synth1);
    }
}

void BeepPin1::noTone()
{
    duration = 0;
    pd->sound->synth->stop(synth1);
}


// Speaker pin 2, Timer 4A, Port C bit 7, Arduino pin 13

uint8_t BeepPin2::duration = 0;

void BeepPin2::begin()
{
    synth2 = pd->sound->synth->newSynth();
    pd->sound->source->setVolume((SoundSource*)synth2, 0.3, 0.3);
    pd->sound->synth->setWaveform(synth2, kWaveformSquare);
}

void BeepPin2::tone(uint16_t count)
{
    tone(count, -1);
}

void BeepPin2::tone(uint16_t count, uint8_t dur)
{
    duration = dur;
    pd->sound->synth->playNote(synth2, count / 60, 1, dur, 0);
}

void BeepPin2::timer()
{
    if (duration && (--duration == 0)) {
        pd->sound->synth->stop(synth2);
    }
}

void BeepPin2::noTone()
{
    duration = 0;
    pd->sound->synth->stop(synth2);
}