#ifndef SONG_H
#define SONG_H

#define Song const uint8_t PROGMEM

Song themeSong[] = {
  0x1B,                   // Number of tracks
  0x00, 0x00,             // Address of track 0
  0x11, 0x00,             // Address of track 1
  0x12, 0x00,             // Address of track 2
  0x32, 0x00,             // Address of track 3
  0x38, 0x00,             // Address of track 4
  0x49, 0x00,             // Address of track 5
  0x4A, 0x00,             // Address of track 6
  0x67, 0x00,             // Address of track 7
  0x6B, 0x00,             // Address of track 8
  0x71, 0x00,             // Address of track 9
  0x7A, 0x00,             // Address of track 10
  0x81, 0x00,             // Address of track 11
  0x88, 0x00,             // Address of track 12
  0x8B, 0x00,             // Address of track 13
  0x96, 0x00,             // Address of track 14
  0xA1, 0x00,             // Address of track 15
  0xAC, 0x00,             // Address of track 16
  0xB5, 0x00,             // Address of track 17
  0xC2, 0x00,             // Address of track 18
  0xCF, 0x00,             // Address of track 19
  0xDA, 0x00,             // Address of track 20
  0xE7, 0x00,             // Address of track 21
  0xF4, 0x00,             // Address of track 22
  0x01, 0x01,             // Address of track 23
  0x10, 0x01,             // Address of track 24
  0x13, 0x01,             // Address of track 25
  0x23, 0x01,             // Address of track 26

  0x00,                   // Channel 0 entry track
  0x01,                   // Channel 1 entry track
  0x02,                   // Channel 2 entry track
  0x03,                   // Channel 3 entry track

  //"Intro channel 0"
  0x40, 36,               // FX: SET VOLUME: volume = 36
  0x9E, 4, 5, 6, 7,       // FX: GOTO advanced: ch0 = 0x04 / ch1 = 0x05 / ch2 = 0x06 / ch3 = 0x07
  0x41, (uint8_t)-4,               // FX: SLIDE VOLUME ON: -4
  0x54, 2,                // FX: SET NOTE CUT: 2
  0x9D, 24,               // FX: SET TEMPO: tempo = 24
  0xFD, 6, 9,             // REPEAT: count = 6 + 1 / track = 9
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Intro channel 1"
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Intro channel 2"
  0x40, 64,               // FX: SET VOLUME: volume = 64
  0x41, (uint8_t)-6,               // FX: SLIDE VOLUME ON: -6
  0xFD, 1, 24,            // REPEAT: count = 1 + 1 / track = 24
  0xFC, 13,               // GOTO track 13
  0xFC, 14,               // GOTO track 14
  0xFC, 15,               // GOTO track 15
  0xFC, 16,               // GOTO track 16
  0xFC, 17,               // GOTO track 17
  0xFC, 18,               // GOTO track 18
  0xFC, 19,               // GOTO track 19
  0xFC, 16,               // GOTO track 16
  0xFC, 20,               // GOTO track 20
  0xFC, 21,               // GOTO track 21
  0xFC, 22,               // GOTO track 22
  0xFC, 23,               // GOTO track 23
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Intro channel 3"
  0xFC, 26,               // GOTO track 26
  0xFD, 12, 25,           // REPEAT: count = 12 + 1 / track = 25
  0x9F,                   // FX: STOP CURRENT CHANNEL



  //"Track channel 4"
  0x40, 36,               // FX: SET VOLUME: volume = 36
  0x9E, 4, 5, 6, 7,       // FX: GOTO advanced: ch0 = 0x04 / ch1 = 0x05 / ch2 = 0x06 / ch3 = 0x07
  0x41, (uint8_t)-4,               // FX: SLIDE VOLUME ON: -4
  0x54, 2,                // FX: SET NOTE CUT: 2
  0x9D, 24,               // FX: SET TEMPO: tempo = 24
  0xFD, 5, 9,             // REPEAT: count = 5 + 1 / track = 9
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 5"
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 6"
  0x40, 64,               // FX: SET VOLUME: volume = 64
  0x41, (uint8_t)-6,               // FX: SLIDE VOLUME ON: -6
  0xFC, 13,               // GOTO track 13
  0xFC, 14,               // GOTO track 14
  0xFC, 15,               // GOTO track 15
  0xFC, 16,               // GOTO track 16
  0xFC, 17,               // GOTO track 17
  0xFC, 18,               // GOTO track 18
  0xFC, 19,               // GOTO track 19
  0xFC, 16,               // GOTO track 16
  0xFC, 20,               // GOTO track 20
  0xFC, 21,               // GOTO track 21
  0xFC, 22,               // GOTO track 22
  0xFC, 23,               // GOTO track 23
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 7"
  0xFD, 11, 25,           // REPEAT: count = 12 + 1 / track = 25
  0x9F,                   // FX: STOP CURRENT CHANNEL



  //"Track 8"
  0x40, 32,               // FX: SET VOLUME: volume = 32
  0x9F + 1,               // DELAY: ticks = 1
  0x40, 0,                // FX: SET VOLUME: volume = 0
  0xFE,                   // RETURN

  //"Track 9"
  0xFC, 10,                // GOTO track 10
  0xFC, 11,                // GOTO track 11
  0xFC, 10,                // GOTO track 10
  0xFC, 12,                // GOTO track 12
  0xFE,                   // RETURN

  //"Track 10"
  0x00 + 4,               // NOTE ON: note = 4
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 4,               // NOTE ON: note = 4
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 4,               // NOTE ON: note = 4
  0x9F + 12,              // DELAY: ticks = 12
  0xFE,                   // RETURN

  //"Track 11"
  0x00 + 2,               // NOTE ON: note = 2
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 2,               // NOTE ON: note = 2
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 2,               // NOTE ON: note = 2
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 12"
  0x00 + 7,               // NOTE ON: note = 7
  0x9F + 12,              // DELAY: ticks = 12
  0xFE,                   // RETURN

  //"Track 13"
  0x00 + 40,              // NOTE ON: note = 40
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 40,              // NOTE ON: note = 40
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 40,              // NOTE ON: note = 40
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 40,              // NOTE ON: note = 40
  0x9F + 8,               // DELAY: ticks = 8
  0x00 + 42,              // NOTE ON: note = 42
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 14"
  0x00 + 47,              // NOTE ON: note = 47
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 47,              // NOTE ON: note = 47
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 47,              // NOTE ON: note = 47
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 47,              // NOTE ON: note = 47
  0x9F + 8,               // DELAY: ticks = 8
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 15"
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 8,               // DELAY: ticks = 8
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 16"
  0x00 + 38,              // NOTE ON: note = 38
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 38,              // NOTE ON: note = 38
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 38,              // NOTE ON: note = 38
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 38,              // NOTE ON: note = 38
  0x9F + 12,              // DELAY: ticks = 12
  0xFE,                   // RETURN

  //"Track 17"
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 50,              // NOTE ON: note = 50
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 18"
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 47,              // NOTE ON: note = 47
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 40,              // NOTE ON: note = 40
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 19"
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 43,              // NOTE ON: note = 43
  0x9F + 8,               // DELAY: ticks = 8
  0x00 + 45,              // NOTE ON: note = 45
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 20"
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 55,              // NOTE ON: note = 55
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 57,              // NOTE ON: note = 57
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 21"
  0x00 + 59,              // NOTE ON: note = 59
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 59,              // NOTE ON: note = 59
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 59,              // NOTE ON: note = 59
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 59,              // NOTE ON: note = 59
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 62,              // NOTE ON: note = 62
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 55,              // NOTE ON: note = 55
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 22"
  0x00 + 57,              // NOTE ON: note = 57
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 57,              // NOTE ON: note = 57
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 57,              // NOTE ON: note = 57
  0x9F + 12,              // DELAY: ticks = 12
  0x00 + 57,              // NOTE ON: note = 57
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 55,              // NOTE ON: note = 55
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 50,              // NOTE ON: note = 50
  0x9F + 4,               // DELAY: ticks = 4
  0xFE,                   // RETURN

  //"Track 23"
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x40, 36,               // FX: SET VOLUME: volume = 36
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x40, 24,               // FX: SET VOLUME: volume = 24
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0x40, 12,               // FX: SET VOLUME: volume = 12
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 12,              // DELAY: ticks = 12
  0xFE,                   // RETURN

  //"Track 24"
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 48,              // DELAY: ticks = 48
  0xFE,                   // RETURN

  //"Track 25"
  0xFC, 8,                // GOTO track 8
  0x9F + 11,              // DELAY: ticks = 11
  0xFC, 8,                // GOTO track 8
  0x9F + 11,              // DELAY: ticks = 11
  0xFC, 8,                // GOTO track 8
  0x9F + 11,              // DELAY: ticks = 11
  0xFC, 8,                // GOTO track 8
  0x9F + 7,               // DELAY: ticks = 7
  0xFC, 8,                // GOTO track 8
  0x9F + 3,               // DELAY: ticks = 3
  0xFE,                   // RETURN

  //"Track 26"
  0x9F + 48,              // DELAY: ticks = 48
  0xFE,                   // RETURN
};


Song victorySong[] = {
  0x08,                   // Number of tracks
  0x00, 0x00,             // Address of track 0
  0x03, 0x00,             // Address of track 1
  0x0f, 0x00,             // Address of track 2
  0x1b, 0x00,             // Address of track 3
  0x1c, 0x00,             // Address of track 4
  0x31, 0x00,             // Address of track 5
  0x34, 0x00,             // Address of track 6
  0x3d, 0x00,             // Address of track 7
  0x00,                   // Channel 0 entry track
  0x01,                   // Channel 1 entry track
  0x02,                   // Channel 2 entry track
  0x03,                   // Channel 3 entry track

  //"Track channel 0"
  0x9D, 24,               // FX: SET TEMPO: tempo = 24
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 1"
  0x40, 48,               // FX: SET VOLUME: volume = 48
  0x41, (uint8_t)-1,               // FX: SLIDE VOLUME ON: -1
  0x50, 4, 1,             // FX: SET VIBRATO: 4 1
  0x54, 3,                // FX: SET NOTE CUT: 3
  0xFC, 5,                // GOTO track 5
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 2"
  0x40, 48,               // FX: SET VOLUME: volume = 48
  0x41, (uint8_t)-1,               // FX: SLIDE VOLUME ON: -1
  0x4E, 6, 1,             // FX: SET TREMOLO: 6 1
  0x40, 24,               // FX: SET VOLUME: volume = 24
  0xFC, 4,                // GOTO track 4
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 3"
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track Track 1"
  0x00 + 42,              // NOTE ON: note = 42
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 42,              // NOTE ON: note = 42
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 49,              // NOTE ON: note = 49
  0x9F + 20,              // DELAY: ticks = 20
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 49,              // NOTE ON: note = 49
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 56,              // NOTE ON: note = 56
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 54,              // NOTE ON: note = 54
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 52,              // NOTE ON: note = 52
  0x9F + 4,               // DELAY: ticks = 4
  0x00 + 54,              // NOTE ON: note = 54
  0x9F + 12,              // DELAY: ticks = 12
  0xFE,                   // RETURN

  //"Track Track 2"
  0x00 + 6,               // NOTE ON: note = 6
  0x9F + 64,              // DELAY: ticks = 64
  0xFE,                   // RETURN

  //"Track Track 4"
  0x00 + 6,               // NOTE ON: note = 6
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0x00 + 6,               // NOTE ON: note = 6
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0xFE,                   // RETURN

  //"Track Track 5"
  0x00 + 6,               // NOTE ON: note = 6
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0xFE,                   // RETURN
};


Song lostSong[] = {
  0x0a,                   // Number of tracks
  0x00, 0x00,             // Address of track 0
  0x0f, 0x00,             // Address of track 1
  0x10, 0x00,             // Address of track 2
  0x11, 0x00,             // Address of track 3
  0x18, 0x00,             // Address of track 4
  0x1f, 0x00,             // Address of track 5
  0x30, 0x00,             // Address of track 6
  0x45, 0x00,             // Address of track 7
  0x47, 0x00,             // Address of track 8
  0x57, 0x00,             // Address of track 9
  0x00,                   // Channel 0 entry track
  0x01,                   // Channel 1 entry track
  0x02,                   // Channel 2 entry track
  0x03,                   // Channel 3 entry track

  //"Track channel 0"
  0x40, 48,               // FX: SET VOLUME: volume = 48
  0x9D, 25,               // FX: SET TEMPO: tempo = 25
  0x41, (uint8_t)-1,               // FX: SLIDE VOLUME ON: -1
  0xFC, 5,                // GOTO track 5
  0xFC, 9,                // GOTO track 9
  0x41, (uint8_t)-12,              // FX: SLIDE VOLUME ON: -12
  0xFC, 6,                // GOTO track 6
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 1"
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 2"
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track channel 3"
  0x40, 0,                // FX: SET VOLUME: volume = 0
  0xFC, 7,                // GOTO track 7
  0xFC, 8,                // GOTO track 8
  0x9F,                   // FX: STOP CURRENT CHANNEL

  //"Track snare"
  0x40, 32,               // FX: SET VOLUME: volume = 32
  0x41, (uint8_t)-16,              // FX: VOLUME SLIDE ON: steps = -16
  0x9F + 2,               // DELAY: ticks = 2
  0x43,                   // FX: VOLUME SLIDE OFF
  0xFE,                   // RETURN

  //"Track Track 1"
  0x00 + 18,              // NOTE ON: note = 18
  0x9F + 23,              // DELAY: ticks = 23
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0x00 + 18,              // NOTE ON: note = 18
  0x9F + 7,               // DELAY: ticks = 7
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0x00 + 18,              // NOTE ON: note = 18
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0x00 + 18,              // NOTE ON: note = 18
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0xFE,                   // RETURN

  //"Track Track 2"
  0x00 + 13,              // NOTE ON: note = 13
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0x00 + 13,              // NOTE ON: note = 13
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 5,               // DELAY: ticks = 5
  0x00 + 13,              // NOTE ON: note = 13
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 5,               // DELAY: ticks = 5
  0x00 + 13,              // NOTE ON: note = 13
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 1,               // DELAY: ticks = 1
  0x00 + 13,              // NOTE ON: note = 13
  0x9F + 3,               // DELAY: ticks = 3
  0x00 + 0,               // NOTE ON: note = 0
  0x9F + 5,               // DELAY: ticks = 5
  0xFE,                   // RETURN

  //"Track Track 3"
  0x9F + 64,              // DELAY: ticks = 64
  0xFE,                   // RETURN

  //"Track Track 4"
  0xFC, 4,                // GOTO track 4
  0x9F + 2,               // DELAY: ticks = 2
  0xFC, 4,                // GOTO track 4
  0x9F + 6,               // DELAY: ticks = 6
  0xFC, 4,                // GOTO track 4
  0x9F + 6,               // DELAY: ticks = 6
  0xFC, 4,                // GOTO track 4
  0x9F + 2,               // DELAY: ticks = 2
  0xFC, 4,                // GOTO track 4
  0x9F + 6,               // DELAY: ticks = 6
  0xFE,                   // RETURN

  //"Track Track 5"
  0x00 + 13,              // NOTE ON: note = 13
  0x9F + 24,              // DELAY: ticks = 24
  0xFE,                   // RETURN
  0xEB, 0x00,             // Address of track 22
};


#endif
