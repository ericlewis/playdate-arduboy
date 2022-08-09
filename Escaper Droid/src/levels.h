#ifndef LEVELS_H
#define LEVELS_H

#define MAX_AMOUNT_OF_ROOMS                       32
#define MAX_AMOUNT_OF_INFLUENCING_OBJECTS         16
#define MAX_AMOUNT_OF_TRANSPORTERS                16
#define AMOUNT_OF_ROOMS_AT_BYTE                   0
#define ROOMS_DATA_START_AT_BYTE                  3
#define DOORS_DATA_START_AT_BYTE                  ROOMS_DATA_START_AT_BYTE + 1
#define ELEMENTS_DATA_START_AT_BYTE               ROOMS_DATA_START_AT_BYTE + 5
#define BYTES_USED_FOR_EVERY_ROOM                 13

// ROOM ORDER OF TILES
//              /\
//             /  \
//            / 00 \
//           /\    /\
//          /  \  /  \
//         / 01 \/ 05 \
//        /\    /\    /\
//       /  \  /  \  /  \
//      / 02 \/ 06 \/ 10 \
//     /\    /\    /\    /\
//    /  \  /  \  /  \  /  \
//   / 03 \/ 07 \/ 11 \/ 15 \
//  /\    /\    /\    /\    /\
// /  \  /  \  /  \  /  \  /  \
/// 04 \/ 08 \/ 12 \/ 16 \/ 20 \
//\    /\    /\    /\    /\    /
// \  /  \  /  \  /  \  /  \  /
//  \/ 09 \/ 13 \/ 17 \/ 21 \/
//   \    /\    /\    /\    /
//    \  /  \  /  \  /  \  /
//     \/ 14 \/ 18 \/ 22 \/
//      \    /\    /\    /
//       \  /  \  /  \  /
//        \/ 19 \/ 23 \/
//         \    /\    /
//          \  /  \  /
//           \/ 24 \/
//            \    /
//             \  /
//              \/


//const unsigned char PROGMEM centerOfTiles[][2] =
//{
//  { 58, 18}, {46, 24}, {34, 30}, {22, 36}, {10, 42}, // Tile  0  1  2  3  4
//  { 70, 24}, {58, 30}, {46, 36}, {34, 42}, {22, 48}, // Tile  5  6  7  8  9
//  { 82, 30}, {70, 36}, {58, 42}, {46, 48}, {34, 54}, // Tile 10 11 12 13 14
//  { 94, 36}, {82, 42}, {70, 48}, {58, 54}, {46, 60}, // Tile 15 16 17 18 19
//  {106, 42}, {94, 48}, {82, 56}, {70, 60}, {58, 66}, // Tile 20 21 22 23 24
//};

// DOORS         NORTH        EAST       SOUTH        WEST       ENEMY1      ENEMY2     OBJECT3     FLOOR1      FLOOR2      FLOOR3      FLOOR4      FLOOR5
//0b11001110, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
//  ||||||||    ||||||||                                        ||||||||
//  ||||||||    ||||||||                                        |||||||└->  \
//  ||||||||    ||||||||                                        ||||||└-->   |  these 3 bits are used to determine kind of sprite used for the object
//  ||||||||    ||||||||                                        |||||└--->  /
//  ||||||||    ||||||||                                        ||||└---->  \
//  ||||||||    ||||||||                                        |||└----->   |
//  ||||||||    ||||||||                                        ||└------>   |  these 5 bits are used to determine on what floor tile the object is
//  ||||||||    ||||||||                                        |└------->   |
//  ||||||||    ||||||||                                        └-------->  /   if all 8 bits == 0 => no object
//  ||||||||    ||||||||
//  ||||||||    ||||||||
//  ||||||||    |||||||└->  \  these 2 bits are used to determine what door you'll go to
//  ||||||||    ||||||└-->  /
//  ||||||||    |||||└--->  \
//  ||||||||    ||||└---->   |
//  ||||||||    |||└----->   | these 6 bits are used for the roomnumber you'll go to
//  ||||||||    ||└------>   |
//  ||||||||    |└------->   |
//  ||||||||    └-------->  /
//  ||||||||
//  ||||||||
//  |||||||└->  DOOR WEST  is closed (0 = false / 1 = true)
//  ||||||└-->  DOOR SOUTH is closed (0 = false / 1 = true)
//  |||||└--->  DOOR EAST  is closed (0 = false / 1 = true)
//  ||||└---->  DOOR NORTH is closed (0 = false / 1 = true)
//  |||└----->  DOOR WEST  exists    (0 = false / 1 = true)
//  ||└------>  DOOR SOUTH exists    (0 = false / 1 = true)
//  |└------->  DOOR EAST  exists    (0 = false / 1 = true)
//  └-------->  DOOR NORTH exists    (0 = false / 1 = true)
//
//
//
// TRANSPORT
//0b00000001,
//  |||||||└->  \
//  ||||||└-->   |
//  |||||└--->   | these 6 bits are used for the roomnumber you'll go to
//  ||||└---->   |
//  |||└----->   |
//  ||└------>  /
//  |└-------> NOT USED
//  └--------> NOT USED
//
//
//
//    ROOM      ELEMENT
//0b00000001, 0b00000001,
//  ||||||||    ||||||||
//  ||||||||    |||||||└->  7 => 0 FLOOR  5 INFLUENCED (0 = false / 1 = true)
//  ||||||||    ||||||└-->  6 => 1 FLOOR  4 INFLUENCED (0 = false / 1 = true)
//  ||||||||    |||||└--->  5 => 2 FLOOR  3 INFLUENCED (0 = false / 1 = true)
//  ||||||||    ||||└---->  4 => 3 FLOOR  2 INFLUENCED (0 = false / 1 = true)
//  ||||||||    |||└----->  3 => 4 FLOOR  1 INFLUENCED (0 = false / 1 = true)
//  ||||||||    ||└------>  2 => 5 OBJECT 3 INFLUENCED (0 = false / 1 = true)
//  ||||||||    |└------->  1 => 6 ENEMY  2 INFLUENCED (0 = false / 1 = true)
//  ||||||||    └-------->  0 => 7 ENEMY  1 INFLUENCED (0 = false / 1 = true)
//  ||||||||
//  ||||||||
//  |||||||└->  \
//  ||||||└-->   |
//  |||||└--->   | these 6 bits are used for the roomnumber where the elements are influenced
//  ||||└---->   |
//  |||└----->   |
//  ||└------>  /
//  |└-------> RESERVED FOR SWITCH (0 = OFF / 1 = ON)
//  └--------> NOT USED



const unsigned char PROGMEM level01[] =
{
  8, // amount of rooms
  2, // amount of transporters
  3, // amount of influencing objects
  // DOORS         NORTH       EAST       SOUTH       WEST         ENEMY1      ENEMY2        OBJECT3       FLOOR1      FLOOR2      FLOOR3      FLOOR4      FLOOR5
  0b11111001,   0b00000000, 0b00001111, 0b00000100, 0b00000000,   0b00000000, 0b00000000,   0b00000000,   0b00000010, 0b00001010, 0b00010010, 0b00011010, 0b00100010, // room00
  0b11000000,   0b00000010, 0b00001011, 0b00000000, 0b00000000,   0b00100000, 0b11000001,   0b01100110,   0b00001001, 0b00110001, 0b00000000, 0b00000000, 0b00000000, // room01
  0b10010000,   0b00001110, 0b00000000, 0b00000000, 0b00000101,   0b00000000, 0b00000000,   0b10100001,   0b01111011, 0b10000011, 0b10101011, 0b00000000, 0b00000000, // room02
  0b00110000,   0b00000000, 0b00000000, 0b00001000, 0b00000001,   0b11000010, 0b00000000,   0b01100101,   0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room03

  0b01000000,   0b00000000, 0b00001011, 0b00000000, 0b00000000,   0b00000000, 0b00000000,   0b00000000,   0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room04
  0b01010000,   0b00000000, 0b00001111, 0b00000000, 0b00000101,   0b00000000, 0b00000000,   0b00000000,   0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room05
  0b01010000,   0b00000000, 0b00010011, 0b00000000, 0b00001001,   0b00000000, 0b00000000,   0b00000000,   0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room06
  0b00010000,   0b00000000, 0b00000000, 0b00000000, 0b00001101,   0b00000000, 0b00000000,   0b00000000,   0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // room07

  // transporters data, the order of the data is by ascending numbers
  // ROOM+DIR
  0b00000101,
  0b00000101,

  // data about the elements that get influenced
  // ROOM         ELEMENT
  0b00000001,  0b00000001,
  0b00000011,  0b00000011,
  0b00000100,  0b00011111,

};

const unsigned char PROGMEM level02[] =
{
  4, // amount of rooms
  1, // amount of transporters (the actual amount is this number double)
  3, // amount of influencing objects
  0b10001111, 0b10000000, 0b11000000, 0b00000000, 0b01000000,     0b00000000, 0b00000000,    0b00000000,  0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,  //room00
  255, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, //room01
  255, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, //room02
  255, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, //room03

  // transporters data, the order of the data is by ascending numbers
  // Room going to    tile going to
  0b00000001,         0b00000001,
  0b00000011,         0b00000011,

  // data about the elements that get influenced
  // ROOM         ELEMENT
  0b00000001,  0b00000001,
  0b00000011,  0b00000011,
  0b00000100,  0b00011111,
};


const unsigned char *levels[] =
{
  level01, level02,
};


#endif
