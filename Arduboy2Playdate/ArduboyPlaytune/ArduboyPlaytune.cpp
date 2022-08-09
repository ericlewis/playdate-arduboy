/**
 * @file ArduboyPlaytune.cpp
 * \brief An Arduino library that plays a one or two part musical score and
 * generates tones. Intended for the Arduboy game system.
 */

/*****************************************************************************
* ArduboyPlaytune
*
* Plays a one or two part musical score and generates tones.
*
* Derived from:
* Playtune: An Arduino tune player library
* https://github.com/LenShustek/arduino-playtune
*
* Modified to work well with the Arduboy game system
* https://www.arduboy.com/
*
* The MIT License (MIT)
*
* (C) Copyright 2016, Chris J. Martinez, Kevin Bates, Josh Goebel, Scott Allen
* Based on work (C) Copyright 2011, 2015, Len Shustek
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* This was inspired by and adapted from "A Tone Generator Library",
* written by Brett Hagman, http://www.roguerobotics.com/
*
*****************************************************************************/
/* 27 March 2018, L. Shustek
This was adapted from my Playtune library, but it was missing this fix that
prevents a "stuttering" playback because of timing errors:
  15 August 2016, L. Shustek,
      - Fixed a timing error: T Wasiluk's change to using a 16-bit timer instead
        of an 8-bit timer for score waits exposed a old bug that was in the original
        Brett Hagman code: when writing the timer OCR value, we need to clear the
        timer counter, or else (the manual says) "the counter [might] miss the compare
        match...and will have to count to its maximum value (0xFFFF) and wrap around
        starting at 0 before the compare match can occur". This caused an error that
        was small and not noticeable for the 8-bit timer, but could be hundreds of
        milliseconds for the 16-bit counter. Thanks go to Joey Babcock for pushing me
        to figure out why his music sounded weird, and for discovering that it worked
        ok with the 2013 version that used the 8-bit timer for score waits.
*/

#include "ArduboyPlaytune.h"

static byte _tune_num_chans = 0;
static volatile boolean tune_playing = false; // is the score still playing?
static volatile boolean tone_playing = false;
static volatile boolean tone_mutes_score = false;
static volatile boolean tone_only = false; // indicates don't play score on tone channel
static volatile boolean mute_score = false; // indicates tone playing so mute other channels

// pointer to a function that indicates if sound is enabled
static boolean (*outputEnabled)();

// pointers to your musical score and your position in said score
static volatile const byte *score_start = 0;
static volatile const byte *score_cursor = 0;

PDSynth *chan1 = nullptr;
PDSynth *chan2 = nullptr;

void init()
{
    if (chan1 == nullptr) {
        chan1 = pd->sound->synth->newSynth();
        pd->sound->synth->setWaveform(chan1, kWaveformSquare);
    }
    if (chan2 == nullptr) {
        chan2 = pd->sound->synth->newSynth();
        pd->sound->synth->setWaveform(chan2, kWaveformSquare);
    }
}

ArduboyPlaytune::ArduboyPlaytune(boolean (*outEn)())
{
    outputEnabled = outEn;
}

void ArduboyPlaytune::initChannel(byte pin)
{
    init();
    _tune_num_chans = 2;
}

void ArduboyPlaytune::playNote(byte chan, byte note)
{
    if (chan >= _tune_num_chans) {
        return;
    }

    // we only have frequencies for 128 notes
    if (note > 127) {
        return;
    }

    float freq;
    if (note < 48) {
        freq = note + 32;
    } else {
        freq = note - 4;
    }

    if (chan == 0) {
        pd->sound->synth->playMIDINote(chan1, freq, 1, -1, 0);
    }
    if (chan == 1) {
        pd->sound->synth->playMIDINote(chan2, freq, 1, -1, 0);
    }
}

void ArduboyPlaytune::stopNote(byte chan)
{
    if (chan == 0) {
        pd->sound->synth->stop(chan1);
    }
    if (chan == 1) {
        pd->sound->synth->stop(chan2);
    }
}

void ArduboyPlaytune::playScore(const byte *score)
{
    init();
    score_start = score;
    score_cursor = score_start;
    step();  /* execute initial commands */
    tune_playing = true;  /* release the interrupt routine */
}

void ArduboyPlaytune::stopScore()
{
    for (uint8_t i = 0; i < _tune_num_chans; i++) {
        stopNote(i);
    }
    tune_playing = false;
}

boolean ArduboyPlaytune::playing()
{
    return tune_playing;
}

volatile int32_t duration = 0;
void ArduboyPlaytune::updateCallback()
{
    if (duration > 0) {
        duration--;
        if (duration == 0) {
            if (tone_playing) {
                pd->sound->synth->stop(chan1);
                pd->sound->synth->stop(chan2);
            }
            if (tune_playing) {
                step();
            }
        }
    }
}

void ArduboyPlaytune::step()
{
    byte command, opcode, chan;
    while (1) {
        command = *(score_cursor++);
        opcode = command & 0xf0;
        chan = command & 0x0f;
        if (opcode == TUNE_OP_STOPNOTE) { /* stop note */
            stopNote(chan);
        }
        else if (opcode == TUNE_OP_PLAYNOTE) { /* play note */
            playNote(chan, *(score_cursor++));
        }
        else if (opcode < 0x80) { /* wait count in msec. */
            duration = ((unsigned)command << 8) | (*(score_cursor++));
            break;
        }
        else if (opcode == TUNE_OP_RESTART) { /* restart score */
            score_cursor = score_start;
        }
        else if (opcode == TUNE_OP_STOP) { /* stop score */
            tune_playing = false;
            break;
        }
    }
}

void ArduboyPlaytune::closeChannels()
{
    stopNote(0);
    stopNote(1);
    _tune_num_chans = 0;
    tune_playing = tone_playing = tone_only = mute_score = false;
}

void ArduboyPlaytune::tone(unsigned int frequency, unsigned long tone_duration)
{
    if (_tune_num_chans < 2) {
        return;
    }

    tone_playing = true;
    mute_score = tone_mutes_score;
    pd->sound->synth->stop(chan2);
    pd->sound->synth->playNote(chan1, frequency, 1, -1, 0);
    duration = tone_duration;
}

void ArduboyPlaytune::toneMutesScore(boolean mute)
{
    tone_mutes_score = mute;
}