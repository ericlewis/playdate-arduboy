 
/*
*/

#include "Arduboy2.h"
#include <Arduboy2Beep.h>
#include "Images.h"

#define MAX(A,B) A > B ? A : B


#define block_air 0
#define block_grass 1
#define block_dirt 2
#define block_stone 3
#define block_iron 4
#define block_coal 5
#define block_nickel 6
#define block_gold 7
#define block_bedrock 8
#define block_door 9
#define block_chrome 10
#define block_spawn 11
#define block_spawn_no_loot 12
#define block_star 13

  

/*
each must be 4 wide. first 4 bytes are moving left,
second 4 bytes are moving right.
*/
PROGMEM const unsigned char enemies [] = {
 0x60,0x90,0xA0,0xC0,0xC0,0xA0,0x90,0x60,    //slime_0
 0xC0,0xA0,0xA0,0xC0,0xC0,0xA0,0xA0,0xC0,    //slime_1
 0xC0,0xA0,0x90,0x60,0x60,0x90,0xA0,0xC0,    //slime_2
 
 0x36,0x49,0xAB,0x26,0x26,0xAB,0x49,0x36,    //skele_0
 0x36,0xC9,0x2B,0x16,0x16,0x2B,0xC9,0x36,    //skele_1
 0xB6,0x49,0x2B,0x26,0x26,0x2B,0x49,0xB6,    //skele_2
 
 0x9B,0x35,0xCA,0x10,0x10,0xCA,0x35,0x9B,    //zombs_0
 0x1A,0xF5,0x8B,0x08,0x08,0x8B,0xF5,0x1A,    //zombs_1
 0xDA,0x35,0x8A,0x08,0x08,0x8A,0x35,0xDA,    //zombs_2  
};

#define level_height 32

long next_seed;
unsigned char i,j,k,l;

//add overlap top and bottom
unsigned short mined[] = {
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00,
  0x00
};

unsigned char gamebuf [] = { 
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
 0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
};

unsigned char inventory [] = { 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned char blocksInInventory;
long recipe_evaluation;

PROGMEM const unsigned char state_idle = 1;
PROGMEM const unsigned char state_walk = 2;
PROGMEM const unsigned char state_action = 4;
PROGMEM const unsigned char state_slash = 8;
#define state_crafting 16

PROGMEM const unsigned char action_mine = 0;
PROGMEM const unsigned char action_eat = 1;
PROGMEM const unsigned char action_craft = 2;
PROGMEM const unsigned char action_place = 3;
PROGMEM const unsigned char action_sentinel = 4;

#define FACING_RIGHT 0
#define FACING_LEFT 1

#define player_bb_w 4
#define player_bb_h 8

typedef struct Pos {
  unsigned char xpos, ypos,facing;
  char upvel;
} Pos;

typedef struct Player {
  Pos p;
  char anim;
  unsigned char state;  
  unsigned char action, action_cooldown, action_perf;
  unsigned char depth;
  unsigned char health;
  unsigned char stars_kills;
  unsigned short armour_weapon;
  
} Player;

typedef struct Enemy {
  Pos p;
  unsigned char state;
  unsigned char health;
} Enemy;


#define game_state_menu 1
#define game_state_playing 2
#define game_state_setup 3
#define game_state_ended 4

unsigned char game_state;
unsigned char main_menu_state;

#define MM_START 1
#define MM_LOAD 2
#define MM_SOUND 4
#define MM_SOUND_BIT 8

#define MAX_ENEMIES 18
#define CAVE_MIN 8
#define CAVE_MAX level_height - 4
#define ORE_MIN 6
#define ORE_MAX level_height - 1

#define armour_boots 0
#define armour_helm 1
#define armour_pants 2
#define armour_chest 3
#define armour_sword 4

Enemy enemy_buf[MAX_ENEMIES] = {{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0},{0}};
unsigned char used_enemies;

Arduboy2 arduboy;
Player player;
BeepPin1 toneplayer;

// Playdate
void resolveResultForRecipe(unsigned char offset);
void drawRecipeAtSlot(unsigned char offset, unsigned char index, unsigned char row_pos,unsigned char col_pos);
void printNameForItem(Item_Registry item);
void draw_recipe_block(unsigned char row_pos,unsigned char col_pos);
void handleInput();
void reset_main();
unsigned char readBlockIDat(unsigned char xpos, unsigned char ypos);
void setBlockAt(unsigned char xpos, unsigned char ypos, unsigned char type);
void draw_menu();
void addItemToInventory(Item_Registry item, unsigned char count);
unsigned char setLevelForArmour(unsigned char type, unsigned char lvl);
unsigned char inventory_satisfies_element(unsigned char element);
void applyGravity(Pos *ps);
unsigned char getLevelForArmour(unsigned char type);
void printNameForRecipe(unsigned char index);
unsigned char getBlockToPlace(unsigned char* xindex, unsigned char* yindex);
void draw_game_over();
void handleEnemiesForCurrentState();
void drawPlayerForCurrentState();
void drawActionForCurrentState();
unsigned char isInCraftMenu();
void draw_setup();
void draw_crafting();
void draw_playing();
unsigned char getBlockToMine(unsigned char* xindex, unsigned char* yindex);
unsigned char isBlockMined(unsigned char xpos, unsigned char ypos);
void idle_state();
void draw_info_for_inv(Item_Registry item, unsigned char count);

void setup() {
  // initiate arduboy instance
  arduboy.begin();
  i=j=k=l=0;
  arduboy.setFrameRate(25);
  arduboy.initRandomSeed();
  next_seed = millis();
  
  reset_main();
  player.armour_weapon = 0x0000;
  
  arduboy.audio.begin();
  toneplayer.begin();
  
}

void reset_main() {
  j = 4;
  game_state = game_state_menu;
  main_menu_state = MM_START;
  arduboy.setRGBled(0,0,0);  
  player.depth = 1;
  player.health = 200;
  inventory[0] = 0x00; inventory[1] = 0x00; inventory[2] = 0x00; inventory[3] = 0x00;
  inventory[4] = 0x00; inventory[5] = 0x00; inventory[6] = 0x00; inventory[7] = 0x00;
  inventory[8] = 0x00; inventory[9] = 0x00; inventory[10] = 0x00; inventory[11] = 0x00;
  inventory[12] = 0x00; inventory[13] = 0x00; inventory[14] = 0x00; inventory[15] = 0x00;
  player.armour_weapon = 0;

}

void cave_creeper(unsigned char cpx, unsigned char cpy, unsigned char lifetime) {
    if(lifetime==0) { return; }
    if(cpx < 2 || cpx > 14) { return; }
    if(cpy > CAVE_MAX) { return; }
    if(readBlockIDat(cpx,cpy) == block_air) { return; }
    setBlockAt(cpx,cpy,block_air); 

    l = rand() % 100;
    if(used_enemies < MAX_ENEMIES-3-(player.depth/8) && l > 80) {
        enemy_buf[used_enemies].p.xpos = cpx*8;
        enemy_buf[used_enemies].p.ypos = cpy*8;
        enemy_buf[used_enemies].p.facing = 0;
        enemy_buf[used_enemies].p.upvel = 3;
        enemy_buf[used_enemies].health = 10 + (player.depth/2);
        enemy_buf[used_enemies].state = l > 93 ? 1 : l > 86 ? 2 : 3;
        used_enemies++; 
    }
    
    if(lifetime==1) { 
      return; 
    }
    l = rand() % 12;
    if(l> 10) {
      cave_creeper(cpx-1,cpy,lifetime-1);
      cave_creeper(cpx,cpy+1,lifetime-1);
      cave_creeper(cpx+1,cpy,lifetime-1);
    } else if(l > 7) {
      cave_creeper(cpx,cpy+1,lifetime-1);
      cave_creeper(cpx+1,cpy,lifetime-1);
    } else {
      if(l>5){
        cave_creeper(cpx,cpy+1,lifetime-1);
      } else {  
        cave_creeper(cpx+1,cpy,lifetime-1);
      }
    }
}

void ore_creeper(unsigned char cpx, unsigned char cpy, unsigned char lifetime, unsigned char block_id) {
    if(lifetime==0) { return; }
    if(cpx < 2 || cpx > 14) { return; }
    if(cpy > CAVE_MAX) { return; }
    if(readBlockIDat(cpx,cpy) == block_air) { return; }
    setBlockAt(cpx,cpy,block_id); 
    
    if(lifetime==1) { 
      return; 
    }
    l = rand() % 12;
    if(l> 10) {
      ore_creeper(cpx-1,cpy,lifetime-1,block_id);
      ore_creeper(cpx,cpy+1,lifetime-1,block_id);
      ore_creeper(cpx+1,cpy,lifetime-1,block_id);
    } else if(l > 7) {
      ore_creeper(cpx,cpy+1,lifetime-1,block_id);
      ore_creeper(cpx+1,cpy,lifetime-1,block_id);
    } else {
      if(l>5){
        ore_creeper(cpx,cpy+1,lifetime-1,block_id);
      } else {  
        ore_creeper(cpx+1,cpy,lifetime-1,block_id);
      }
    }
}

void generate_dungeon(unsigned char x, unsigned char y) {

    for(unsigned char iter=0; iter<5; iter++){
      setBlockAt(x-2+iter,y,block_air); 
      setBlockAt(x-2+iter,y+1,block_air); 
      setBlockAt(x-2+iter,y+2,block_air); 
      setBlockAt(x-2,y-1+iter,block_bedrock); 
      setBlockAt(x-2+iter,y-1,block_bedrock); 
      setBlockAt(x-2+iter,y+3,block_bedrock); 
    }
    setBlockAt(x+2,y-1,block_bedrock); 
    setBlockAt(x+2,y,block_bedrock); 
    setBlockAt(x+2,y+1,block_bedrock); 
    setBlockAt(x+2,y+2,block_bedrock); 
    setBlockAt(x+2,y+3,block_bedrock); 
    setBlockAt(x,y+2,block_spawn); 
    setBlockAt(x - (x<8?-2:2),y+2,block_air); 
}

void generate_challenge(unsigned char y) {
  for(unsigned char iter=0; iter<16; iter++){
    setBlockAt(iter,y,block_bedrock); 
    setBlockAt(iter,y+1,block_air); 
    setBlockAt(iter,y+2,block_air); 
    setBlockAt(iter,y+3,block_air); 
    setBlockAt(iter,y+4,block_air); 
    setBlockAt(iter,y+5,block_bedrock); 
    
  }

  setBlockAt(0,y,block_air); 
  setBlockAt(0,y+5,block_air); 
  setBlockAt(1,y+5,block_air); 

  setBlockAt(2,y+1,block_bedrock); 
  setBlockAt(2,y+2,block_bedrock); 
  setBlockAt(2,y+3,block_bedrock); 
  setBlockAt(2,y+4,block_stone); 

  setBlockAt(6,y+2,block_spawn_no_loot); 

  setBlockAt(7,y+1,block_stone); 
  setBlockAt(7,y+2,block_bedrock); 
  setBlockAt(7,y+3,block_bedrock); 
  setBlockAt(7,y+4,block_bedrock);

  setBlockAt(12,y+1,block_stone); 
  setBlockAt(12,y+2,block_bedrock); 
  setBlockAt(12,y+3,block_bedrock); 
  setBlockAt(12,y+4,block_bedrock);
  setBlockAt(10,y+5,block_air); 
  setBlockAt(11,y+5,block_air); 
  
  setBlockAt(11,y+3,block_spawn_no_loot); 
  setBlockAt(15,y+2,block_spawn_no_loot); 
  
  setBlockAt(15,y+4,block_star);
  setBlockAt(15,y+5,block_air); 
  setBlockAt(14,y+5,block_air); 
}

void seed_level_init() {
  srand(next_seed);
  blocksInInventory = 0;
  
  used_enemies = 0;
  player.state = state_idle;
  player.p.xpos = 8;
  player.p.ypos = 0;
  player.p.facing = 0;
  player.anim = 0;
  player.p.upvel = 0;
  player.action = action_mine;
  player.action_cooldown = 0;
  player.action_perf = 0;
  
  for(i=0; i<level_height; i++){
    mined[i]=0;
    for(l=0; l<16; l=l+2){
      
      if(i<2){
        setBlockAt(l,i,block_air); 
        setBlockAt(l+1,i,block_air);  
      } else if(i==2){
        setBlockAt(l,i,rand()%10<5? block_air : block_grass); 
        setBlockAt(l+1,i,block_grass);  
      } else if(i==3){
        setBlockAt(l,i,block_grass); 
        setBlockAt(l+1,i,block_grass); 
      } else if(i==4){
        setBlockAt(l,i,block_dirt); 
        setBlockAt(l+1,i,block_dirt); 
      } else if(i==5){
        setBlockAt(l,i,rand()%10<5? block_dirt : block_stone);
        setBlockAt(l+1,i,rand()%10<5? block_dirt : block_stone);
      } else if(i==level_height-2){
        setBlockAt(l,i,block_air); 
        setBlockAt(l+1,i,block_air);  
      } else if(i==level_height-1){
        setBlockAt(l,i,block_bedrock); 
        setBlockAt(l+1,i,block_bedrock);  
      } else {
        setBlockAt(l,i,block_stone);
        setBlockAt(l+1,i,block_stone);
      }
    }  
  }

  /*
  * Make some caves
  */
  i = rand() % 4;
  do {
    cave_creeper(2 + (rand() % 12), CAVE_MIN + (rand() % (CAVE_MAX - CAVE_MIN)),2 + (rand() % 10));
    i++;
  } while(i<8);

  i = rand() % 4;
  
  do {
    j = (rand() % 100) + ((player.depth)/2);
    ore_creeper(2 + (rand() % 12), ORE_MIN + (rand() % (ORE_MAX - ORE_MIN)),1 + (rand() % 2), j < 40 ? block_iron: j < 70 ? block_coal : j < 95 ? block_gold : j < 115 ? block_chrome : block_nickel);
    i++;
  } while(i<24);

  if((rand() % 100) < 50) {
    generate_dungeon(3 + rand() % 11,ORE_MIN + (rand() % (CAVE_MAX - ORE_MIN)));  
  }

  if(player.depth % 8 == 0 && player.depth > 0) {
    generate_challenge(8 + (rand() % 16));  
  }
  
  setBlockAt(14,level_height-3,block_bedrock); 
  setBlockAt(15,level_height-3,block_bedrock); 
  setBlockAt(15,level_height-2,block_door);
  
}

#define EEPROM_START 0xFF

unsigned char load_saved_game() {
    if(EEPROM.read(EEPROM_START+0) != 0xBE && EEPROM.read(EEPROM_START+1) != 0x79 && EEPROM.read(EEPROM_START+2) != 0x01){ 
      return 0; 
      arduboy.setRGBled(255,0,0);  
    }
    player.depth = EEPROM.read(EEPROM_START+3);
    player.health = EEPROM.read(EEPROM_START+4);

    player.armour_weapon = 0;
    player.armour_weapon = EEPROM.read(EEPROM_START+5);
    player.armour_weapon = player.armour_weapon << 8;
    player.armour_weapon |= EEPROM.read(EEPROM_START+6);
    player.stars_kills = EEPROM.read(EEPROM_START+7);

    for(unsigned char iter = 0; iter<16; iter++) {
      inventory[iter] = EEPROM.read(EEPROM_START+8+iter);  
    }
    
    arduboy.setRGBled(0,255,0);  
    return 1;
}

//Updated to update :) 
unsigned char save_game() {
  EEPROM.update(EEPROM_START+0, 0xBE);  
  EEPROM.update(EEPROM_START+1, 0x79);  
  EEPROM.update(EEPROM_START+2, 0x01);
  EEPROM.update(EEPROM_START+3, player.depth);
  EEPROM.update(EEPROM_START+4, player.health);
  EEPROM.update(EEPROM_START+5, (player.armour_weapon >> 8)&0xFF);
  EEPROM.update(EEPROM_START+6, player.armour_weapon&0xFF);
  EEPROM.update(EEPROM_START+7, player.stars_kills);

  for(unsigned char iter = 0; iter<16; iter++) {
    EEPROM.update(EEPROM_START+8+iter, inventory[iter]);  
  }

  arduboy.setRGBled(0,255,0);  

  return 1;
}

 
// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  // first we clear our screen to black
  arduboy.clear();
  toneplayer.timer();
  if(toneplayer.duration <= 0) {
    toneplayer.noTone();  
  }

  if(game_state == game_state_menu) { draw_menu(); }
  if(game_state == game_state_ended) { draw_game_over(); }
  if(game_state == game_state_playing) {
    if(isInCraftMenu()) {
      draw_crafting(); 
    } else {
      draw_playing(); 
      
    }
  }
  if(game_state == game_state_setup) { draw_setup(); }

  //Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  arduboy.display();
  //toneplayer.tone(toneplayer.freq(256),40);
}

void draw_game_over() {
  if(arduboy.pressed(A_BUTTON)){
    i = 121;
  }
  else if(i==121 && arduboy.notPressed(A_BUTTON)){
    reset_main();
    i=0;  
  }
  
  arduboy.setCursor(0,0);
  arduboy.print(F("Game Over"));
    
  arduboy.setCursor(0,12);
  l = player.depth;
  arduboy.print(F("Depth: "));  
  
  arduboy.setCursor(36,12); arduboy.print(l);

  arduboy.setCursor(0,24);
  arduboy.print(F("Kills: "));  
  l = (player.stars_kills >> 3) & 0x1F;
  arduboy.setCursor(36,24); arduboy.print(l);

  arduboy.setCursor(0,36);
  arduboy.print(F("Stars: "));  
  l = (player.stars_kills & 0x07);
  arduboy.setCursor(36,36); arduboy.print(l);

  arduboy.setCursor(0,48);
  arduboy.print(F("Press A for menu"));  
  
}

void draw_setup() {

  if(main_menu_state==0){
    seed_level_init(); 
    main_menu_state = 1;
  }
  k++;
  k = k > 8 ? 8 : k;
  if(k == 8){
    if(arduboy.pressed(A_BUTTON)){
      arduboy.setRGBled(0,0,0);  
      game_state = game_state_playing;
      main_menu_state = 0;
    }
    if(arduboy.pressed(B_BUTTON)){
      //load game.
      player.anim++;
      if(player.anim > 40) {
        player.anim = 0;
        save_game();
      }
    } else {
      player.anim = 0;
    }
  }

  if(arduboy.everyXFrames(8)){arduboy.setRGBled(0,0,0);  }

  arduboy.setCursor(0,0);
  l = player.depth;
  arduboy.print(F("Depth: "));  
  arduboy.setCursor(36,0); arduboy.print(l);
  arduboy.setCursor(0,16);
  arduboy.print(F("Press A To begin"));  
  arduboy.setCursor(0,32);
  arduboy.print(F("Hold B To save"));  
  arduboy.drawRect(0,42,80,4,WHITE);
  arduboy.fillRect(0,43,player.anim*2,2,WHITE);
}

 int dogenonce = 0;
void draw_menu() {
  next_seed += blocks[next_seed % 20000];

  j++;
  j = j >= 4 ? 4 : j;
  
  if(arduboy.pressed(A_BUTTON)){
    
    if(main_menu_state & MM_START) { 
      main_menu_state = 0;
      game_state = game_state_setup;  
      k = 0;
      return;
    }
    if(main_menu_state & MM_LOAD) { 
      if(load_saved_game()){
        game_state = game_state_setup;  
        main_menu_state = 0;
      } else {
        k = 0;  
      }
    }
    if(main_menu_state & MM_SOUND && j==4) { 
      j=0;
      if(main_menu_state & MM_SOUND_BIT) {
        main_menu_state = main_menu_state & 7;
        player.armour_weapon |= 0x8000;
      } else {
        main_menu_state = main_menu_state | MM_SOUND_BIT;
        player.armour_weapon &= 0x7FFF;
      }
      
    }
    
  }

  
  arduboy.setCursor(8,2);
  arduboy.print(F("Start"));
  arduboy.setCursor(8,10);
  arduboy.print(F("Load"));
  arduboy.setCursor(8,18);
  arduboy.print(F("Sound"));

  
  arduboy.setCursor(62,2);
  arduboy.print(F("ARDUMINER"));
  arduboy.setCursor(52,11);
  arduboy.print(F("By @Bergasms"));
  
  if(arduboy.pressed(DOWN_BUTTON) && j==4){
    l = main_menu_state; j=0;
    main_menu_state = (main_menu_state << 1) & 7;
    main_menu_state = main_menu_state == 0 ? MM_START : main_menu_state;
    main_menu_state = main_menu_state | (l & MM_SOUND_BIT);
  }
  
  if(arduboy.pressed(UP_BUTTON) && j==4){
    l = main_menu_state; j=0;
    main_menu_state = (main_menu_state >> 1) & 7;
    main_menu_state = main_menu_state == 0 ? MM_SOUND : main_menu_state;
    main_menu_state = main_menu_state | (l & MM_SOUND_BIT);
  }

   arduboy.drawFastHLine(1,4,4,main_menu_state & MM_START ? WHITE : BLACK); 
   arduboy.drawFastHLine(1,12,4,main_menu_state & MM_LOAD  ? WHITE : BLACK); 
   arduboy.drawFastHLine(1,20,4,main_menu_state & MM_SOUND ? WHITE : BLACK); 
  
  if(main_menu_state & MM_SOUND_BIT) {
    arduboy.drawCircle(42,22,2,WHITE);
    //toneplayer.off();
  } else {
    arduboy.fillCircle(42,22,2,WHITE);
   // toneplayer.on();
  }

  if(dogenonce == 0) {
    dogenonce = 1;
  for(i=0; i<level_height; i++){
    mined[i]=0;
    for(l=0; l<16; l=l+2){
      
      if(i<2){
        setBlockAt(l,i,block_air); 
        setBlockAt(l+1,i,block_air);  
      } else if(i==2){
        setBlockAt(l,i,rand()%10<5? block_air : block_grass); 
        setBlockAt(l+1,i,block_grass);  
      } else if(i==3){
        setBlockAt(l,i,block_grass); 
        setBlockAt(l+1,i,block_grass); 
      } else if(i==4){
        setBlockAt(l,i,block_dirt); 
        setBlockAt(l+1,i,block_dirt); 
      } else if(i==5){
        setBlockAt(l,i,rand()%10<5? block_dirt : block_stone);
        setBlockAt(l+1,i,rand()%10<5? block_dirt : block_stone);
      }
    }
  } 
  }
 
 for(int xx=1; xx<8; xx++) {
    for(i=0; i<16; i++){
      
      unsigned char blockID = readBlockIDat(i,xx);
      arduboy.drawBitmap(i*8,32 + (xx*8),blocks + (blockID*8),8,8,1); 
    } 
 }
}

void draw_playing() {
  next_seed++;

  handleInput();
  applyGravity(&(player.p));
  
  //works over the backing buffer, taking each byte, splitting it, and figuring out what is up.
  k=(player.p.ypos/8);
  k = k < 4 ? 0 : k - 4;
  k = k+9 >= level_height ? level_height-9 : k;
  
  if(player.p.ypos < 32){ k=0; }
  else if(player.p.ypos < (level_height-4)*8){ k=((player.p.ypos)/8)-3; }
  else {k=level_height-8;}
  
  for(j=1;j<9;j++){
    for(i=0; i<16; i++){
      
      unsigned char blockID = readBlockIDat(i,k);
      arduboy.drawBitmap(i*8,j*8 - (player.p.ypos % 8),blocks + (blockID*8),8,8,1);

      if((blockID == block_spawn || blockID == block_spawn_no_loot) && arduboy.everyXFrames(120) && (abs(player.p.ypos/8 + 1 - k) < 5) && (abs(player.p.xpos/8 - i)) < 8) {
        char iter;
        for(iter = 0; iter < used_enemies; iter++){
          if(enemy_buf[iter].health == 0) { break; }
        }

        if(iter == used_enemies) {
          if(used_enemies < MAX_ENEMIES-1) {
            used_enemies++;
          } else {
            iter = -1;  
          }
        }

        if(iter >= 0 && iter < MAX_ENEMIES-1){
          enemy_buf[iter].p.xpos = i*8;
          enemy_buf[iter].p.ypos = (k-(blockID == block_spawn_no_loot?-1:1))*8;
          enemy_buf[iter].p.facing = 0;
          enemy_buf[iter].p.upvel = 3;
          enemy_buf[iter].health = 10 + (player.depth/2);
          enemy_buf[iter].state = (k%3) + 1;
        }
      }
      
      if(player.action == action_mine){
        //outline the selected box
        unsigned char xp,yp;
        l = getBlockToMine(&xp, &yp);
        if(l && xp==i && yp==k){
          arduboy.drawRect(i*8,j*8 - (player.p.ypos % 8),8,8,BLACK);
        }
      }

      if(player.action == action_place && blocksInInventory){
        //outline the selected box
        
        unsigned char xp,yp;
        l = getBlockToPlace(&xp, &yp);
        if(l && xp==i && yp==k && millis()%2==0){
          
          arduboy.drawBitmap(i*8 + 1,j*8 - (player.p.ypos % 8) + 1,blocks + (block_bedrock*8),6,6,1);
        }
      }
    }
    k++;
  }
  
  //arduboy.drawBitmap(16,8,person+64+(m*8),8,8,1);  

  handleEnemiesForCurrentState();
  drawPlayerForCurrentState();
  drawActionForCurrentState();
  
  if(player.health == 0) {
    game_state = game_state_ended;
  }
  k=0;
  j=0;
}

/**
*
* Inv and crafting
*
*/

unsigned char playSound() { 
  return ((player.armour_weapon & 0x8000)>0?1:0); 
}

void add_star() {
  player.stars_kills++;
}

void add_kill() {
  unsigned char kills = player.stars_kills >> 3;
  kills++;
  player.stars_kills = player.stars_kills & 0x7;
  player.stars_kills |= (kills << 3);
}

void addBlockToInventory(unsigned char blockID) {

  if(playSound()){
    toneplayer.tone(toneplayer.freq(512),40);  
  }
  
  switch(blockID){
    case block_stone:
      if(playSound()){ toneplayer.tone(toneplayer.freq(256),40); }
      blocksInInventory++;  
      blocksInInventory = blocksInInventory >= 63 ? 63 : blocksInInventory;
    break;
    case block_coal:
      addItemToInventory(I_COAL,1);
    break;
    case block_iron:
      addItemToInventory(I_IRON_ORE,1);
    break;
    case block_gold:
      addItemToInventory(I_GOLD_ORE,1);
    break;
    case block_nickel:
      addItemToInventory(I_NICKEL_ORE,1);
    break;
    case block_chrome:
      addItemToInventory(I_CHROME_ORE,1);
    break;
    case block_spawn: {
      addItemToInventory(I_GAPPLE,1);
      unsigned char rnum = rand()%100;
      if(rnum < 50) {
        addItemToInventory(I_IRON_ORE,3);
        addItemToInventory(I_NICKEL_ORE,1);
        addItemToInventory(I_APPLE,1);
      } else if(rnum < 80) {
        addItemToInventory(I_STEEL,2);
        addItemToInventory(I_CHROME_ORE,2);
        addItemToInventory(I_NICKEL_ORE,2);
      } else {
        addItemToInventory(I_GAPPLE,1);
        addItemToInventory(I_STAINLESS,2);
        addItemToInventory(I_INCONEL,2);
      }
      if(playSound()){ toneplayer.tone(toneplayer.freq(10000),40); }
    }
    break;
    case block_spawn_no_loot: { 
      if(playSound()){ toneplayer.tone(toneplayer.freq(12000),40); }
      addItemToInventory(I_STEEL,1); 
    } break;
    case block_star: {
      add_star();  
    }
    default: if(playSound()){ toneplayer.tone(toneplayer.freq(256),40); } break;
  }  
}

void set_item_in_inventory(unsigned char index, unsigned char count, Item_Registry item) {
    inventory[index] = 0;
    inventory[index] = (item << 3);
    inventory[index] = ((inventory[index] & 0xF8) | (count & 0x7));
}

Item_Registry item_in_inventory(unsigned char index, unsigned char *count) {
  *count = (inventory[index] & 0x7);
  return (Item_Registry)((inventory[index] >> 3) & 0x1F);  
}

void addItemToInventory(Item_Registry item, unsigned char count) {
    
    for(unsigned char iter = 0; iter < 16; iter++) {
      unsigned char cval;
      Item_Registry check = item_in_inventory(iter,&cval);
      if(check == item || check == I_EMPTY) {
        if(cval < 7) {
          //we have space in this slot for this item.
          if(cval + count <= 7) {
             set_item_in_inventory(iter,cval+count,item);
             return;
          } else {
            //we have overflow
            set_item_in_inventory(iter,7,item);
            addItemToInventory(item,(cval+count)-7);
            return;
          }
        }  
      }  
    }

    //if we get here, we could not add the item. inv full? maybe inform user with light flash
}

void removeItemFromInventory(Item_Registry item, unsigned char count) {
    
    for(unsigned char iter = 0; iter < 16; iter++) {
      unsigned char cval;
      Item_Registry check = item_in_inventory(iter,&cval);
      if(check == item) {
        if(cval > 0) {
          //we have space in this slot for this item.
          if(count < cval) {
             set_item_in_inventory(iter,cval-count,item);
             return;
          } else {
            //we have to remove
            set_item_in_inventory(iter,0,I_EMPTY);
            for(unsigned char iterplus = iter; iterplus < 15; iterplus++) {
              inventory[iterplus] = inventory[iterplus+1];
            }
            set_item_in_inventory(15,0,I_EMPTY);
            removeItemFromInventory(item,(count - cval));
            return;
          }
        }  
      }  
    }

}

/**
*
* Game controls
*
*/
unsigned char readBlockIDat(unsigned char xpos, unsigned char ypos) {
  if(isBlockMined(xpos,ypos)){ return block_air; }
    if(xpos%2==0){
      return ((gamebuf[ypos*8+(xpos/2)] >> 4) & 0x0F);
    } else {
      return (gamebuf[ypos*8+(xpos/2)] & 0x0F);
    }
}

void setBlockAt(unsigned char xpos, unsigned char ypos, unsigned char type) {
  
  if(xpos%2==0){
    gamebuf[ypos*8+(xpos/2)] = ((gamebuf[ypos*8+(xpos/2)] & 0x0F) | (type << 4));    
  } else {
    gamebuf[ypos*8+(xpos/2)] = ((gamebuf[ypos*8+(xpos/2)] & 0xF0) | type);
  }
}

void placeBlockat(unsigned char xpos, unsigned char ypos, unsigned char type) {
    setBlockAt(xpos,ypos,type);
    mined[ypos] &= ~(1 << xpos);
}

void mineBlockat(unsigned char xpos, unsigned char ypos) {
    mined[ypos] |= (0x1 << xpos);
}

unsigned char isBlockMined(unsigned char xpos, unsigned char ypos) {
  return (mined[ypos] & (0x1 << xpos)) == 0 ? 0 : 1; 
}

unsigned char leftBlockID(unsigned char xpos, unsigned char ypos) {
    if(xpos < 1) { return block_air; }
    xpos--;
    if(isBlockMined(xpos,ypos)){ return block_air; }
    return readBlockIDat(xpos,ypos);
}


unsigned char rightBlockID(unsigned char xpos, unsigned char ypos) {
    if(xpos > 14) { return block_air; }
    xpos++;
    if(isBlockMined(xpos,ypos)){ return block_air; }
    return readBlockIDat(xpos,ypos);
}

unsigned char isWalking() { return (player.state & state_walk) != 0 ? 1 : 0; }
unsigned char isAttacking() { return (player.state & state_slash) != 0 ? 1 : 0; }
unsigned char isIdle() { return (player.state & state_idle) != 0 ? 1 : 0; }
unsigned char isMining() { return (player.state & state_action) && (player.action == action_mine) ? 1 : 0; }
unsigned char isEating() { return (player.state & state_action) && (player.action == action_eat) ? 1 : 0; }
unsigned char isCrafting() { return (player.state & state_action) && (player.action == action_craft) ? 1 : 0; }
unsigned char isPlacing() { return (player.state & state_action) && (player.action == action_place) ? 1 : 0; }
unsigned char isInCraftMenu() { return (player.state & state_crafting) != 0 ? 1 : 0; }


unsigned char onGround(unsigned char xpos, unsigned char ypos, unsigned char upvel, unsigned char facing) { 

  if(upvel > 3) { return 0; }
  
  unsigned char next = ypos;
  next = next < 1 ? 1 : next;
  
  i = (xpos+(facing==FACING_RIGHT?1:3))/8;
  l = (next+9)/8;
  return readBlockIDat(i,l);
}


void set_walk() {
    player.state = state_walk;
}

void set_jump() {
  if(onGround(player.p.xpos,player.p.ypos,player.p.upvel,player.p.facing)) {
    player.p.upvel = 8;
  }
}

void set_attack() {
  player.anim = 0;
  player.state = state_slash;

  //check for nearby enemies and hit them
  for(unsigned char iter=0; iter<used_enemies; iter++) {
    //enemy can be considered for updates
    if(enemy_buf[iter].health==0) { continue; }
    
    //update ypos info
    Enemy *a = &(enemy_buf[iter]);
    char kk = a->p.ypos - player.p.ypos;
    if(abs(kk) > 4) { continue; }
    kk = a->p.xpos - player.p.xpos;
    unsigned char damg = 1 + getLevelForArmour(armour_sword)*2;
    if((player.p.facing == FACING_RIGHT && kk >= -2 && kk < 6)) {
        enemy_buf[iter].health = enemy_buf[iter].health - damg > 1 ? enemy_buf[iter].health - damg : 0;
        enemy_buf[iter].p.xpos += 4;
    } else if((player.p.facing == FACING_LEFT && kk <= 2 && kk > -6)) {
        enemy_buf[iter].health = enemy_buf[iter].health - damg > 1 ? enemy_buf[iter].health - damg : 0;
        enemy_buf[iter].p.xpos -= 4;
    } 

    if(enemy_buf[iter].health == 0) {
      //enemy was killed this step, we could reward with an Apple.
      if((rand()%100) < 30) {
        addItemToInventory(I_APPLE,1);
      }  
      add_kill();
    }
  }
  
}

void set_action() {
  player.anim = 0;
  if(player.state != state_action && player.state != state_idle){
    player.action_perf = 0;
  }
  player.state = state_action;
  
}

void set_idle() {
  player.state = state_idle;
  idle_state();
}

void toggle_action() {
  if(player.action_cooldown>0){ return; }
  player.action_cooldown = 3;
  player.action += 1;
  
  if(player.action == action_sentinel) {
    player.action = action_mine;  
  }
     
}

void idle_state(){
  if(arduboy.pressed(RIGHT_BUTTON)){  set_walk(); } 
  else if(arduboy.pressed(LEFT_BUTTON)){  set_walk(); } 
  else if(arduboy.pressed(UP_BUTTON)){  set_jump(); }
  else if(arduboy.pressed(DOWN_BUTTON)){ toggle_action();  } //change actions
  else if(arduboy.pressed(B_BUTTON)){ set_attack(); } 
  else if(arduboy.pressed(A_BUTTON)){ set_action(); } 
  else {
    player.anim=0;  
  }
  
}

void walk_state() {
  player.anim += 1; player.anim %= 3;  
  if(arduboy.pressed(UP_BUTTON)){ set_jump(); }
  
  if(arduboy.pressed(RIGHT_BUTTON)){ 
    player.p.facing = FACING_RIGHT;
    if(arduboy.pressed(B_BUTTON)){ 
      set_attack(); 
    } else { 
      l = rightBlockID((player.p.xpos-4)/8,(player.p.ypos+4)/8);
      if(l==0 || l==block_door){
        player.p.xpos += 1;     
        if(player.p.xpos % 8 > 4 && l==block_door){
          main_menu_state = 0;
          game_state = game_state_setup;  
          player.depth++;
        }
      }
      if(player.p.xpos > 124){ player.p.xpos = 124; } 
    }
  } 
  else if(arduboy.pressed(LEFT_BUTTON)){ 
    player.p.facing = FACING_LEFT;
    if(arduboy.pressed(B_BUTTON)){ 
      set_attack(); 
    } else { 
      if(leftBlockID((player.p.xpos+7)/8,(player.p.ypos+4)/8)==0){
        player.p.xpos -= 1;     
      }
      if(player.p.xpos < 0){ player.p.xpos = 0; } 
    }
  }
  else {
    if(player.anim==0) {set_idle();}  
  }

}

unsigned char resolveNext(unsigned char next, Pos *ps) {
  i = (ps->xpos+(ps->facing==FACING_RIGHT?1:3))/8;
  l = (next+8)/8;
  
  if(readBlockIDat(i,l) == 0) {
    ps->ypos = next;  
    return 0;
  } else {
    return 1;
  }
}

void applyGravity(Pos *ps) {
  unsigned char next = ps->ypos - (ps->upvel - 3);
  next = next < 8 ? 8 : next;
  ps->upvel = ps->upvel > 0 ? ps->upvel - 1 : 0;
  if(ps->upvel > 3) {
    if(resolveNext(next, ps)==1){
      ps->upvel = 3;  
    }
    
    return;  
  }

  if(resolveNext(next, ps)==1){
    if(resolveNext(next-1, ps)==1){
      if(resolveNext(next-2, ps)==0){
        
      }
    }
  }
}

void attack_state() {
  player.anim += 1; player.anim %= 3;  
  if(player.anim==0) {set_idle();}  
  if(arduboy.pressed(UP_BUTTON)){ set_jump(); }
}

void action_state() {
  player.anim += 1; player.anim %= 3;  
  if(player.anim==0) {set_idle(); }  
}

unsigned char common_action(unsigned char holdfor) {
  player.anim += 1; player.anim %= 3;  
  if(player.anim==0) {set_idle(); }  
  if(arduboy.pressed(A_BUTTON)){ 
    player.action_perf++;
    if(player.action_perf >= holdfor) {
      player.action_perf = 0;
      return 1;
    }
  }
  if(arduboy.notPressed(A_BUTTON)){ 
    player.action_perf = 0;
  }
  return 0;
}



unsigned char getBlockToPlace(unsigned char* xindex, unsigned char* yindex) {
  unsigned char can_place;
    if(player.p.facing==1) {
      if((player.p.xpos)/8 == 0){ return 0; }
      *xindex = (player.p.xpos)/8 - 1;
      *yindex = (player.p.ypos+12)/8;
      can_place = leftBlockID((player.p.xpos+2)/8,*yindex); if(can_place == block_air) { return 1; }
      *yindex = (player.p.ypos+4)/8;
      can_place = leftBlockID((player.p.xpos+2)/8,*yindex); if(can_place == block_air) { return 1; }
      *yindex = (player.p.ypos-4)/8;
      can_place = leftBlockID((player.p.xpos+2)/8,*yindex); if(can_place == block_air) { return 1; }
       
    } else {
      if((player.p.xpos)/8 == 15){ return 0; }
      *xindex = (player.p.xpos)/8 + 1;
      *yindex = (player.p.ypos+12)/8;
      can_place = rightBlockID((player.p.xpos+2)/8,*yindex); if(can_place == block_air) { return 1; }
      *yindex = (player.p.ypos+4)/8;
      can_place = rightBlockID((player.p.xpos+2)/8,*yindex); if(can_place == block_air) { return 1; }
      *yindex = (player.p.ypos-4)/8;
      can_place = rightBlockID((player.p.xpos+2)/8,*yindex); if(can_place == block_air) { return 1; }
    }
    
    return 0;
}

unsigned char getBlockToMine(unsigned char* xindex, unsigned char* yindex) {
  unsigned char mined_block;
    if(player.p.facing==1) {
      *xindex = (player.p.xpos)/8 - 1;
      *yindex = (player.p.ypos+4)/8;
      mined_block = leftBlockID((player.p.xpos+2)/8,*yindex);
      if(mined_block && mined_block != block_bedrock){
        return mined_block;
      }
      *yindex = (player.p.ypos+12)/8;
      mined_block = leftBlockID((player.p.xpos+2)/8,*yindex);
      if(mined_block && mined_block != block_bedrock){
        return mined_block;
      } 
    } else {
      *xindex = (player.p.xpos)/8 + 1;
      *yindex = (player.p.ypos+4)/8;
      mined_block = rightBlockID((player.p.xpos+2)/8, *yindex);
      if(mined_block && mined_block != block_bedrock){
        return mined_block;
      }
      *yindex = (player.p.ypos+12)/8;
      mined_block = rightBlockID((player.p.xpos+2)/8, *yindex);
      if(mined_block && mined_block != block_bedrock){
        return mined_block;
      }
    }
    return block_air;
}

void mine_state() {
  if(common_action(10)) {
    
    unsigned char mined_block, x_ind, y_ind;
    mined_block = getBlockToMine(&x_ind, &y_ind);
    if(mined_block){
      player.action_perf = 0;
      mineBlockat(x_ind,y_ind);
      addBlockToInventory(mined_block);
    }
  } else {
    if(player.action_perf) {
      if(playSound()){ toneplayer.tone(toneplayer.freq(28000),120); } 
    }  
  }
}

void eat_state() {
  if(common_action(10)) {
    unsigned char has = 0;
    has |= inventory_satisfies_element(0x71) == 1 ? 1 : 0;
    has |= inventory_satisfies_element(0x69) == 1 ? 2 : 0;

    if(player.health <= 160) {
      if(has & 1) {
        removeItemFromInventory((Item_Registry)14,1);
        player.health = player.health + 40 > 200 ? 200 : player.health + 40;  
      }
      else if(has & 2) {
        removeItemFromInventory((Item_Registry)13,1);
        player.health = player.health + 10 > 200 ? 200 : player.health + 10;
      }
    }
    
    else {
      if(has & 2) {
        removeItemFromInventory((Item_Registry)13,1);
        player.health = player.health + 10 > 200 ? 200 : player.health + 10;
      } 
      else if(has & 1) {
        removeItemFromInventory((Item_Registry)14,1);
        player.health = player.health + 40 > 200 ? 200 : player.health + 40;  
      }
    } 
    
  }
}

void placing_state() {
  
  if(common_action(8)) {
    if(blocksInInventory == 0){
      return;
    }
    unsigned char xp,yp;
    l = getBlockToPlace(&xp, &yp);
    if(l) {
      placeBlockat(xp,yp,block_stone);
      player.action_perf = 0;
      blocksInInventory--;
    }
  }
}

void craft_state() {
  if(common_action(10)) {
    player.state = state_crafting;
    player.action_cooldown = 0;
    j = 0;
    recipe_evaluation = 0xFFFF;
  }
}

void handleInput() {
  if(isIdle()) { idle_state(); }
  else if(isWalking()) { walk_state(); }
  else if(isAttacking()) { attack_state(); }
  else if(isMining()) { mine_state(); }
  else if(isEating()) { eat_state(); }
  else if(isCrafting()) { craft_state(); }
  else if(isPlacing()) { placing_state(); }
}

unsigned char offset_for_state() {
  if(isIdle()) { return 0; }
  if(isWalking()) { return 0; }
  if(isAttacking()) { return 6; }
  if(isMining()) { return 3; }
  if(isEating()) { return 9; }
  if(isCrafting()) { return 0; }
  if(isPlacing()) { return 0; }
}

void drawPlayerForCurrentState() {
  i=offset_for_state() + player.anim;
  j=0;
  if(player.p.facing==1){ i+= 12; j=4;}
  if(player.p.ypos >= (level_height-4)*8) {l = player.p.ypos - (level_height-4)*8 + 4*8;}
  else if(player.p.ypos >= 4*8) {l = 4*8;}
  else {l = player.p.ypos; }
  
  arduboy.drawBitmap(player.p.xpos-j,l,person + (i*8),8,8,1);  
}

void drawActionForCurrentState() {
  arduboy.fillRect(120,1,8,8,BLACK);
  arduboy.drawBitmap(120,1,action + (player.action*8),8,8,1);
  if(player.action_cooldown>0) { player.action_cooldown--; }

  arduboy.fillRect(49,1,71,8,BLACK);
  unsigned char nextx = 50;
  for(unsigned char iter = 0; iter < (player.health/10); iter++) {
    arduboy.drawBitmap(nextx,1,heart + ((iter%2)==0 ? 0 : 3),3,8,1);
    nextx += 3 + ((iter%2)==0 ? 0 : 1);
  }
  
}

void stepAnimation(Enemy *a) {
  unsigned char cval = (a->state >> 2) & 3;
  cval++;
  cval = cval > 3 ? 1 : cval;
  a->state &= 0xF3;
  a->state |= ((cval << 2) & 12);
}

void handleEnemiesForCurrentState() {
  unsigned char logical_next;
  if(player.p.ypos >= (level_height-4)*8) {logical_next = player.p.ypos - (level_height-4)*8 + 4*8;}
  else if(player.p.ypos >= 4*8) {logical_next = 4*8;}
  else {logical_next = player.p.ypos; }

  for(unsigned char iter=0; iter<used_enemies; iter++) {
    //enemy can be considered for updates
    if(enemy_buf[iter].health==0) { continue; }
    
    //update ypos info
    Enemy *a = &(enemy_buf[iter]);
    char kk = a->p.ypos - player.p.ypos + logical_next;
    if(kk > 64) { continue; }
    
    //update gravity
    applyGravity(&(a->p));

    

    //move if required
    //change the 2 into a per mob speed val
    if(arduboy.everyXFrames(2)) {
      char vdist = a->p.xpos - player.p.xpos;
      if(vdist > 0) {
        a->p.facing = FACING_LEFT;
        //left move
        if(leftBlockID((a->p.xpos+7)/8,(a->p.ypos+4)/8)==0){
          a->p.xpos -= 1;    
          stepAnimation(a); 
        } else {
          if(leftBlockID((a->p.xpos+7)/8,(a->p.ypos-4)/8)==0 && readBlockIDat((a->p.xpos)/8,(a->p.ypos-4)/8)==0){
            a->p.xpos -= 1;       
            a->p.upvel = 8;
            stepAnimation(a);
          }

        }
      } else if (vdist < 0) {
        //right move
        a->p.facing = FACING_RIGHT;
        if(rightBlockID((a->p.xpos-3)/8,(a->p.ypos+4)/8)==0){
          a->p.xpos += 1;   
          stepAnimation(a);  
        } else {
          if(rightBlockID((a->p.xpos-3)/8,(a->p.ypos-4)/8)==0 && readBlockIDat((a->p.xpos)/8,(a->p.ypos-4)/8)==0){
            a->p.xpos += 1;         
            a->p.upvel = 8;
            stepAnimation(a);  
          }  
        }
      }

      
    }

    //hit player?
    
    
    //handle drawing
    j = a->state & 3;
    j--;
    
    
    arduboy.drawBitmap((a->p.xpos)+2,kk ,enemies + (j*24) + (((a->state >> 2) & 3)-1)*8 + (a->p.facing==FACING_LEFT?4:0),4,8,WHITE);  
    
    if(arduboy.everyXFrames(player.depth < 10 ? 4 : player.depth < 20 ? 3 : player.depth < 40 ? 2 : 1)){
      kk = a->p.ypos - player.p.ypos;
      if(abs(kk) > 4) { continue; }
      kk = a->p.xpos - player.p.xpos;

      char dmg = (j+1) * (player.depth < 10 ? 4 : player.depth < 20 ? 8 : player.depth < 40 ? 12 : 16);

      if((rand() % 100) < getLevelForArmour(armour_boots)*10) { dmg -= getLevelForArmour(armour_boots)*3; }
      if((rand() % 100) < getLevelForArmour(armour_helm)*15) { dmg -= getLevelForArmour(armour_helm)*3; }
      if((rand() % 100) < getLevelForArmour(armour_pants)*20) { dmg -= getLevelForArmour(armour_pants)*3; }
      if((rand() % 100) < getLevelForArmour(armour_chest)*30) { dmg -= getLevelForArmour(armour_chest)*3; }
      
      if(abs(kk) < 3 && dmg > 0) {
        player.health = player.health - dmg > 0 ? player.health - dmg : 0;
        if(playSound()){ toneplayer.tone(toneplayer.freq(750),20); }
        arduboy.setRGBled(255,0,0);
      }
    }

    if(arduboy.everyXFrames(2)) {
      arduboy.setRGBled(0,0,0);  
    }
  }  
 
}


/*
*
* Crafting
*/
unsigned char drop_item_counter;
void draw_crafting() {

  //action cooldown is co-opted for button state.
  if(arduboy.pressed(UP_BUTTON) && (player.action_cooldown & 1)==0)    { j = j <= 1 ? 0 : j - 1; player.action_cooldown |= 1;}
  if(arduboy.pressed(DOWN_BUTTON) && (player.action_cooldown & 2)==0)  { j++;  player.action_cooldown |= 2;}
  if(arduboy.pressed(LEFT_BUTTON) && (player.action_cooldown & 4)==0)  { k = k <= 1 ? 0 : k - 1;  player.action_cooldown |= 4;}
  if(arduboy.pressed(RIGHT_BUTTON) && (player.action_cooldown & 8)==0) { k = k < 9 ? k + 1 : k;   player.action_cooldown |= 8;}

  if(arduboy.notPressed(UP_BUTTON) )    { player.action_cooldown &= 0xE;}
  if(arduboy.notPressed(DOWN_BUTTON) )  { player.action_cooldown &= 0xD;}
  if(arduboy.notPressed(LEFT_BUTTON) )  { player.action_cooldown &= 0xB;}
  if(arduboy.notPressed(RIGHT_BUTTON) ) { player.action_cooldown &= 7;}

  //border ticker
  l++; l = l > 15 ? 0 : l;

  //draw the crafting interface. it might be quicker to do this from a bitmap, but we are not time limited here.
  //top area
  
  arduboy.drawFastHLine(0,8,128,WHITE);

  unsigned char row_pos = j;
  unsigned char col_pos = k;

  
  //item row and columns

  unsigned char cval;
  Item_Registry item;
  for(i=0; i<8; i++) {
    arduboy.drawRect(1 + (i*11),10,10,10,WHITE);
    arduboy.drawRect(1 + (i*11),21,10,10,WHITE);

    item = item_in_inventory(i,&cval);
    if(cval > 0) {
      
      arduboy.drawBitmap(2+(i*11),11,item_icons + ((item-1)*8),8,8,1);  
    }

    item = item_in_inventory(i+8,&cval);
    if(cval > 0) {
      arduboy.drawBitmap(2+(i*11),22,item_icons + ((item-1)*8),8,8,1);  
    }
  }
  

  //armour blocks
  if(getLevelForArmour(armour_boots) > 0 ) { 
    arduboy.drawBitmap(94,12,item_icons + 14*8,8,8,1); 
    arduboy.drawFastHLine(93,10,2*getLevelForArmour(armour_boots),WHITE);  
  } else { arduboy.drawRect(93,10,10,10,WHITE); }
  
  if(getLevelForArmour(armour_helm) > 0 ) { 
    arduboy.drawBitmap(105,12,item_icons + 15*8,8,8,1); 
    arduboy.drawFastHLine(104,10,2*getLevelForArmour(armour_helm),WHITE);  
  } else { arduboy.drawRect(105,10,10,10,WHITE); }
  
  if(getLevelForArmour(armour_pants) > 0 ) { 
    arduboy.drawBitmap(117,12,item_icons + 16*8,8,8,1); 
    arduboy.drawFastHLine(116,10,2*getLevelForArmour(armour_pants),WHITE);  
  } else { arduboy.drawRect(117,10,10,10,WHITE); }
  
  if(getLevelForArmour(armour_chest) > 0 ) { 
    arduboy.drawBitmap(94,23,item_icons + 17*8,8,8,1); 
    arduboy.drawFastHLine(93,21,2*getLevelForArmour(armour_chest),WHITE);  
  } else { arduboy.drawRect(93,21,10,10,WHITE); }

  //sword block
  arduboy.drawRect(121,32,6,32,WHITE);
  unsigned char len = getLevelForArmour(armour_sword) + 1;
  arduboy.fillRect(123,60,2,2,WHITE);  
  arduboy.drawFastHLine(122,59,4,WHITE);
  arduboy.drawFastVLine(123,59 - len * 5,len * 5,WHITE);
  arduboy.drawFastVLine(124,60 - len * 5,len * 5,WHITE);

  //blocks
  arduboy.fillRect(105,22,8,8,WHITE);
  arduboy.setCursor(115,23); arduboy.print(blocksInInventory);

  arduboy.drawFastVLine(90,8,24,WHITE);

  
    if(row_pos < 2) {
      if(col_pos > 7){
        if(l > 8) {
          arduboy.drawRect(90 + ((col_pos - 8)*18),9 + (row_pos*18),18,18,WHITE);
        }
      } else {
        if(l > 8) {
          arduboy.drawRect(0 + (col_pos*11),9 + (row_pos*11),12,12,WHITE);
        }
        item = item_in_inventory(row_pos*8 + col_pos,&cval);
        if(cval > 0) {
          draw_info_for_inv(item,cval);  
          if(arduboy.pressed(A_BUTTON)){
            drop_item_counter++;
            if(drop_item_counter > 30) {
                removeItemFromInventory(item,1);
            drop_item_counter = 0;
            }
          } else {
            drop_item_counter = 0;
          }
          
        } else {
          arduboy.setCursor(0,0);
          arduboy.print(F("CRAFTING"));  
        }
      }
    } else {
      if(row_pos==2 && col_pos > 7) {
        if(l > 8) {
          arduboy.drawRect(90,45,36,10,WHITE);
        }  
      }
    }

    draw_recipe_block(row_pos,col_pos);     
  
  
  
  if(arduboy.pressed(B_BUTTON)){    
    set_idle(); 
  }
}

void draw_info_for_inv(Item_Registry item, unsigned char count) {
  arduboy.setCursor(0,0);
  arduboy.print(count);
  arduboy.setCursor(8,0);
  arduboy.print(F("x"));
  arduboy.setCursor(16,0);
  printNameForItem(item);
  arduboy.setCursor(110,0);
  arduboy.print(F("A "));
  if(drop_item_counter < 10) {
    arduboy.print(F("/"));
  } else if(drop_item_counter < 20) {
    arduboy.print(F("|"));
  } else  {
    arduboy.print(F("\\"));
  }
}

void printNameForItem(Item_Registry item) {
  switch(item) {
    case I_EMPTY: arduboy.print(F("Empty")); break;
    case I_IRON_ORE: arduboy.print(F("Iron Ore")); break;
    case I_GOLD_ORE: arduboy.print(F("Gold Ore")); break;
    case I_CHROME_ORE: arduboy.print(F("Chrome Ore")); break;
    case I_COAL: arduboy.print(F("Coal")); break;  
    case I_NICKEL_ORE: arduboy.print(F("Nickel Ore")); break;
    case I_IRON_INGOT: arduboy.print(F("Iron Ingot")); break;
    case I_GOLD_INGOT: arduboy.print(F("Gold Ingot")); break;  
    case I_NICKEL_INGOT: arduboy.print(F("Nickel Ingot")); break;
    case I_CHROME_INGOT: arduboy.print(F("Chrome Ingot")); break;
    case I_STEEL: arduboy.print(F("Steel")); break;
    case I_INCONEL: arduboy.print(F("Inconel")); break;
    case I_STAINLESS: arduboy.print(F("Stainless")); break;
    case I_APPLE: arduboy.print(F("Apple")); break;
    case I_GAPPLE: arduboy.print(F("gApple")); break;
  }  
}

unsigned char inventory_satisfies_element(unsigned char element) {

  
  unsigned char recipe = ((element>>3)&0x1F);
  
  
  if(recipe>=I_RECIPE_CHECK_NONE && recipe <= I_RECIPE_CHECK_III) {
  
    if(getLevelForArmour((element & 0x07))==recipe - I_RECIPE_CHECK_NONE) { return 1; }
  }
  
  unsigned char cntr = 0;
  Item_Registry itr;
  itr = item_in_inventory(0,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(1,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(2,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(3,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(4,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(5,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(6,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(7,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(8,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(9,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(10,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(11,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(12,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(13,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(14,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }
  itr = item_in_inventory(15,&cntr); if(recipe==itr && (element & 0x07) <= cntr) { return 1; }


  return 0;
}

void evaluate_recipes() {

  i = 0;                            //i is recipe pointer. it always points to the header
  unsigned char val = 0;
  if( recipe_evaluation == 0xFFFFL ) {
    
    unsigned char stepper = 0;
    recipe_evaluation = 0;
    
    recipe_skip: 
    val = *(recipes+i);
    while(val != 0xFF) {

      
      for(unsigned char iter = 1; iter <= (val & 0x7); iter++) {
        //recipes[i + iter] is the next element
        if(inventory_satisfies_element(*(recipes + (i + iter))) == 0) {
          i += ((val & 0x7) + 1);  //skip length of elements + recipe header
          stepper++;
          goto recipe_skip;  
        }
 
        
      }
    
      i += ((val & 0x7) + 1);
      recipe_evaluation |= (0x0001L << stepper);
      stepper++;
      val = *(recipes+i);
      //this recipe satisfies.
      //printNameForRecipe((recipes[i] >> 3) & 0x1F);
    }
  
  }
}
 
void draw_recipe_block(unsigned char row_pos,unsigned char col_pos) {

  //player.action_perf is the scroll position.
  //player.anim is the number of available recipes.
  unsigned char visited = 0;
  //we evaluate the recipes, as we go through we determine the starting point in the array for the three recipes.
  evaluate_recipes();
  
  //recipe block
  arduboy.drawRect(1,32,119,32,WHITE);  

  if(col_pos < 8 && row_pos > 1) {
    arduboy.drawFastVLine(2,44,6,WHITE);
    arduboy.drawFastVLine(3,46,3,WHITE);
  }
  arduboy.drawFastHLine(1,41,119,WHITE);  
  
  //if cursor is on recipe block
  i = 0;
  unsigned char stepper = 0;
  unsigned char val = 0;
  val = *(recipes+i);

  unsigned char crpos = row_pos>1?row_pos-2:0;
  
  while(val != 0xFF) {

    

    if(recipe_evaluation & (0x0001L << stepper)) {
       //this is an active recipe.  
       if(visited == crpos) {
          //render at slot 1
          drawRecipeAtSlot(i,0,row_pos,col_pos);

          if(row_pos > 1 && col_pos < 8) {
            if(arduboy.pressed(A_BUTTON)){    
                
              arduboy.fillRect(2,1,player.anim*3,6,WHITE);
              if(player.anim < 40) {
                player.anim++;  
              } else if(player.anim == 40) {
                player.anim = 0; 
                arduboy.setCursor(2,0);
                arduboy.print("Hold A To Craft");
                resolveResultForRecipe(i);
                recipe_evaluation = 0xFFFF;
                k = 1;
                j = 0;
                return;
              }
            }
            if(arduboy.notPressed(A_BUTTON) )    { 
              player.anim = 0; 
              arduboy.setCursor(2,0);
                arduboy.print("Hold A To Craft");
            }
          }
           
       }
       else if(visited == (crpos+1)) {
          //render at slot 2
          drawRecipeAtSlot(i,1,row_pos,col_pos);
           
       }
       visited++;
    }
    
    i += ((val & 0x7) + 1);
    val = *(recipes+i);
    stepper++;
  }
}

void drawRecipeAtSlot(unsigned char offset, unsigned char index, unsigned char row_pos,unsigned char col_pos) {
    arduboy.drawRect(1,42 + (index * 10)+index,119,10,WHITE);  
    Recipe_Registry r = (Recipe_Registry)((*(recipes+offset) >> 3) & 0x1F);
    
    //show required in top bar
    unsigned char val = 0;
    val = *(recipes+offset);
    for(unsigned char elements=0; elements < (val & 0x7); elements++) {
        unsigned char element = *(recipes + offset+1 + elements);
        //print icon.
        
        arduboy.setCursor(6,42+ (index * 10)+index+1);
        printNameForRecipe(r);

        if(col_pos < 8 && row_pos > 1 && index == 0) {
          if(((element>>3) & 0x1F) >= I_RECIPE_CHECK_NONE && ((element>>3) & 0x1F) <= I_RECIPE_CHECK_III){ continue; }
          arduboy.drawBitmap(3 + (28*elements),33,  item_icons + ((((element>>3) & 0x1F)-1)*8),8,8,1);  
          arduboy.setCursor(11 + (index==0?3:0) + (28*elements),33);
          arduboy.print("x");
          arduboy.print((element&0x7));
          //print number
        }
        
    }

    
}

void printNameForRecipe(unsigned char index) {
  switch (index) {
    case R_IRON_INGOT: arduboy.print(F("Iron Ingot")); break;
    case R_NICKEL_INGOT: arduboy.print(F("Nickel Ingot")); break;
    case R_GOLD_INGOT: arduboy.print(F("Gold Ingot")); break;
    case R_CHROME_INGOT: arduboy.print(F("Chrome Ingot")); break;
    case R_STEEL: arduboy.print(F("Steel")); break;
    case R_INCONEL: arduboy.print(F("Inconel")); break;
    case R_CHROME: arduboy.print(F("Chrome")); break;
    case R_GAPPLE: arduboy.print(F("Gold Apple")); break;
    case R_IRON_BOOTS: arduboy.print(F("Iron Boots")); break;
    case R_STEEL_BOOTS: arduboy.print(F("Steel Boots")); break;
    case R_CHROME_BOOTS: arduboy.print(F("Stainless Boots")); break;
    case R_INCONEL_BOOTS: arduboy.print(F("Inconel Boots")); break;
    case R_IRON_HELM: arduboy.print(F("Iron Helm")); break;
    case R_STEEL_HELM: arduboy.print(F("Steel Helm")); break;
    case R_CHROME_HELM: arduboy.print(F("Stainless Helm")); break;
    case R_INCONEL_HELM: arduboy.print(F("Inconel Helm")); break;
    case R_IRON_PANTS: arduboy.print(F("Iron Pants")); break;
    case R_STEEL_PANTS: arduboy.print(F("Steel Pants")); break;
    case R_CHROME_PANTS: arduboy.print(F("Stainless Pants")); break;
    case R_INCONEL_PANTS: arduboy.print(F("Inconel Pants")); break;
    case R_IRON_CHEST: arduboy.print(F("Iron Chest")); break;
    case R_STEEL_CHEST: arduboy.print(F("Steel Chest")); break;
    case R_CHROME_CHEST: arduboy.print(F("Stainless Chest")); break;
    case R_INCONEL_CHEST: arduboy.print(F("Inconcel Chest")); break;
    case R_IRON_SWORD: arduboy.print(F("Iron Sword")); break;
    case R_STEEL_SWORD: arduboy.print(F("Steel Sword")); break;
    case R_CHROME_SWORD: arduboy.print(F("Katana")); break;
    case R_INCONEL_SWORD: arduboy.print(F("Claymore")); break;   
    case R_STAINLESS: arduboy.print(F("Stainless")); break;   
  }
}

void resolveResultForRecipe(unsigned char offset) {
  Recipe_Registry r = (Recipe_Registry)((*(recipes+offset) >> 3) & 0x1F);
    
  //show required in top bar
  unsigned char val = 0;
  val = *(recipes+offset);
  for(unsigned char elements=0; elements < (val & 0x7); elements++) {
      unsigned char element = *(recipes + offset+1 + elements);
      removeItemFromInventory((Item_Registry)((element>>3) & 0x1F),(element&0x7));
  }


  //now add the result
  switch(r) {
    case R_IRON_INGOT:    { addItemToInventory(I_IRON_INGOT,1);   } break;
    case R_NICKEL_INGOT:  { addItemToInventory(I_NICKEL_INGOT,1); } break;
    case R_GOLD_INGOT:    { addItemToInventory(I_GOLD_INGOT,1);   } break;
    case R_CHROME_INGOT:  { addItemToInventory(I_CHROME_INGOT,1); } break;
    case R_STEEL:         { addItemToInventory(I_STEEL,1);     } break;
    case R_INCONEL:       { addItemToInventory(I_INCONEL,1);   } break;
    case R_CHROME:        { addItemToInventory(I_STAINLESS,1); } break;
    case R_GAPPLE:        { addItemToInventory(I_GAPPLE,1);    } break;
    case R_IRON_BOOTS:    { setLevelForArmour(armour_boots,1); } break;
    case R_STEEL_BOOTS:   { setLevelForArmour(armour_boots,2); } break;
    case R_CHROME_BOOTS:  { setLevelForArmour(armour_boots,3); } break;
    case R_INCONEL_BOOTS: { setLevelForArmour(armour_boots,4); } break;
    case R_IRON_HELM:     { setLevelForArmour(armour_helm,1);  } break;
    case R_STEEL_HELM:    { setLevelForArmour(armour_helm,2);  } break;
    case R_CHROME_HELM:   { setLevelForArmour(armour_helm,3);  } break;
    case R_INCONEL_HELM:  { setLevelForArmour(armour_helm,4);  } break;
    case R_IRON_PANTS:    { setLevelForArmour(armour_pants,1); } break;
    case R_STEEL_PANTS:   { setLevelForArmour(armour_pants,2); } break;
    case R_CHROME_PANTS:  { setLevelForArmour(armour_pants,3); } break;
    case R_INCONEL_PANTS: { setLevelForArmour(armour_pants,4); } break;
    case R_IRON_CHEST:    { setLevelForArmour(armour_chest,1); } break;
    case R_STEEL_CHEST:   { setLevelForArmour(armour_chest,2); } break;
    case R_CHROME_CHEST:  { setLevelForArmour(armour_chest,3); } break;
    case R_INCONEL_CHEST: { setLevelForArmour(armour_chest,4); } break;
    case R_IRON_SWORD:    { setLevelForArmour(armour_sword,1); } break;
    case R_STEEL_SWORD:   { setLevelForArmour(armour_sword,2); } break;
    case R_CHROME_SWORD:  { setLevelForArmour(armour_sword,3); } break;
    case R_INCONEL_SWORD: { setLevelForArmour(armour_sword,4); } break;
    case R_STAINLESS:     { addItemToInventory(I_STAINLESS,1); } break;
  }
}

unsigned char getLevelForArmour(unsigned char type) {
  return (unsigned char)((player.armour_weapon >> (type * 3)) & 0x7);
}

unsigned char setLevelForArmour(unsigned char type, unsigned char lvl) {
  short val = lvl;
  val = val << (type * 3);
  player.armour_weapon = player.armour_weapon & (0xFFFF ^ (0x7 << (type * 3)));
  player.armour_weapon |= val;
}






