#define SUIT_EARTH 0
#define SUIT_WATER 1
#define SUIT_FIRE  2
#define SUIT_AIR   3

#define DECK_SIZE 48

#define ABILITY_TOXIC   0  //Earth -1
#define ABILITY_BURN    1  //Fire  -1
#define ABILITY_SWOOP   2  //Air   -1
#define ABILITY_SPEAR   3  //Water -1 
#define ABILITY_MAGIC   4  //Air, Fire, Water  -2
#define ABILITY_HUNT    5  //Earth -2

PROGMEM const  char card_info_data[] = {
//suit, power, img, text
SUIT_AIR,4,18,-1,
SUIT_AIR,4,18,-1,
SUIT_AIR,4,18,-1,
SUIT_AIR,4,18,-1,
SUIT_AIR,5,19, 4,
SUIT_AIR,5,19, 4,
SUIT_AIR,5,19, 4,
SUIT_AIR,6,20, 2,
SUIT_AIR,6,20, 2,  
SUIT_AIR,7,21,-1,
SUIT_AIR,8,22,-1,
SUIT_AIR,9,23,-1,

SUIT_EARTH,4,0,0,
SUIT_EARTH,4,0,0,
SUIT_EARTH,4,0,0,
SUIT_EARTH,4,0,0,
SUIT_EARTH,5,1,5,
SUIT_EARTH,5,1,5,
SUIT_EARTH,5,1,5,
SUIT_EARTH,6,2,-1,
SUIT_EARTH,6,2,-1,  
SUIT_EARTH,7,3,-1,
SUIT_EARTH,8,4,-1,
SUIT_EARTH,9,5,-1,  

SUIT_WATER,4,6,-1,
SUIT_WATER,4,6,-1,
SUIT_WATER,4,6,-1,
SUIT_WATER,4,6,-1,
SUIT_WATER,5,7,4,
SUIT_WATER,5,7,4,
SUIT_WATER,5,7,4,
SUIT_WATER,6,8,3,
SUIT_WATER,6,8,3,  
SUIT_WATER,7,9,-1,
SUIT_WATER,8,10,-1,
SUIT_WATER,9,11,-1, 

SUIT_FIRE,4,12,1,
SUIT_FIRE,4,12,1,
SUIT_FIRE,4,12,1,
SUIT_FIRE,4,12,1,
SUIT_FIRE,5,13,4,
SUIT_FIRE,5,13,4,
SUIT_FIRE,5,13,4,
SUIT_FIRE,6,14,-1,
SUIT_FIRE,6,14,-1,  
SUIT_FIRE,7,15,-1,
SUIT_FIRE,8,16,-1,
SUIT_FIRE,9,17,-1,   
};

#define CARD_INFO_SIZE 
struct CardInfo {
   char suit;
   char power;
   char img;
   char ability;
};

const unsigned char PROGMEM map_info_data[] {
48,32, // 20
60,44, // 24
60,28, // 16
44,44, // 23

44,28, // 15
12,44, // 21
8,26,  // 13
0,34,  // 17

28,42, // 22
16,32, // 18
16,0,  // 2
0,16,  // 9

10,11, // 5
0,0,   // 1
33,33, // 19
27,12, // 6

32,18, // 11
17,15, // 10
28,25, // 14
60,12, // 8

41,10, // 7
48,16, // 12
48,0,  // 4
32,3,  // 3
};

#define MAP_INFO_SIZE 
struct MapInfo {
    char x;
    char y;
};
