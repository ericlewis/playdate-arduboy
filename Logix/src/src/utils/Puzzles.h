#pragma once

#include "Enums.h"

const uint8_t PROGMEM puzzle_AND[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x00, 0x02, 0x22,   
    0x00, 0x12, 0x22,
    0x11, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_NAND[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x01, 0x02, 0x22,   
    0x01, 0x12, 0x22,
    0x10, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_OR[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x00, 0x02, 0x22,   
    0x01, 0x12, 0x22,
    0x11, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_NOR[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x01, 0x02, 0x22,   
    0x00, 0x12, 0x22,
    0x10, 0x12, 0x22,   

};



const uint8_t PROGMEM puzzle_XOR[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x00, 0x02, 0x22,  
    0x01, 0x12, 0x22,
    0x10, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_XNOR[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x01, 0x02, 0x22,   
    0x00, 0x12, 0x22,
    0x11, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_NOT[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x02,               // Outcomes

    0x01, 0x22, 0x22,   
    0x10, 0x22, 0x22,

};


const uint8_t PROGMEM puzzle_00[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x11, 0x10, 0x22,   
    0x00, 0x01, 0x22,
    0x11, 0x00, 0x22,

};


const uint8_t PROGMEM puzzle_02[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), 

    0x03,               // Outcomes

    0x01, 0x00, 0x01,   
    0x10, 0x11, 0x10,
    0x11, 0x00, 0x11,

};


const uint8_t PROGMEM puzzle_01[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), 

    0x03,               // Outcomes

    0x11, 0x11, 0x10,   
    0x00, 0x01, 0x01,
    0x11, 0x11, 0x01,

};


const uint8_t PROGMEM puzzle_03[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NOT), static_cast<uint8_t>(ItemType::NOT), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x10, 0x12, 0x22,   
    0x01, 0x02, 0x22,
    0x10, 0x02, 0x22,

};


const uint8_t PROGMEM puzzle_04[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::NOR), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x11, 0x10, 0x22,   
    0x00, 0x01, 0x22,
    0x10, 0x00, 0x22,

};


const uint8_t PROGMEM puzzle_05[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::NOT), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x10, 0x10, 0x00,   
    0x00, 0x01, 0x22,
    0x11, 0x01, 0x22,

};


const uint8_t PROGMEM puzzle_06[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NOR), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x10, 0x10, 0x22,   
    0x01, 0x00, 0x22,
    0x01, 0x11, 0x22,

};


const uint8_t PROGMEM puzzle_07[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x02,               // Outcomes

    0x11, 0x20, 0x22,   
    0x00, 0x21, 0x22,

};


const uint8_t PROGMEM puzzle_08[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::XOR), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x10, 0x10, 0x22,   
    0x00, 0x01, 0x22,
    0x01, 0x10, 0x22,

};


const uint8_t PROGMEM puzzle_09[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x11, 0x12, 0x12,   
    0x00, 0x02, 0x02,
    0x00, 0x12, 0x02,

};


const uint8_t PROGMEM puzzle_10[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::AND), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::NOR), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x02,               // Outcomes

    0x11, 0x20, 0x22,   
    0x00, 0x21, 0x22,

};


const uint8_t PROGMEM puzzle_11[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NAND), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x01, 0x01, 0x22,   
    0x11, 0x01, 0x22,
    0x10, 0x11, 0x22,

};


const uint8_t PROGMEM puzzle_12[] = {

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NAND), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x01, 0x01, 0x22,   
    0x11, 0x01, 0x22,
    0x10, 0x10, 0x22,

};


const uint8_t PROGMEM puzzle_13[] = { // OR Gate from NANDs

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NAND), static_cast<uint8_t>(ItemType::NAND), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x00, 0x02, 0x22,   
    0x01, 0x12, 0x22,
    0x11, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_14[] = { // AND gate from NORs

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NOR), static_cast<uint8_t>(ItemType::NOR), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x00, 0x02, 0x22,   
    0x00, 0x12, 0x22,
    0x11, 0x12, 0x22,   

};


const uint8_t PROGMEM puzzle_15[] = { // NOT from NAND gate

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::NAND), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x02,               // Outcomes

    0x01, 0x22, 0x22,   
    0x10, 0x22, 0x22,

};


const uint8_t PROGMEM puzzle_16[] = { 

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), 

    0x03,               // Outcomes

    0x01, 0x01, 0x00,
    0x10, 0x00, 0x10,
    0x10, 0x10, 0x11,   

};


const uint8_t PROGMEM puzzle_17[] = { //NEGOR

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 

    0x03,               // Outcomes

    0x01, 0x02, 0x22,   
    0x01, 0x12, 0x22,
    0x10, 0x12, 0x22,   

};

const uint8_t PROGMEM test_bed[] = { 

    static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), static_cast<uint8_t>(ItemType::SWITCH_UP), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), static_cast<uint8_t>(ItemType::BLANK), 
    static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), static_cast<uint8_t>(ItemType::LED), 

    0x03,               // Outcomes

    0x01, 0x01, 0x01,   
    0x01, 0x01, 0x01,
    0x01, 0x01, 0x01,   

};

#define NUMBER_OF_PUZZLES 27
#define TEST_BED NUMBER_OF_PUZZLES

const uint8_t *puzzles[] =    { puzzle_NOT, puzzle_OR, puzzle_NAND, puzzle_AND, puzzle_XOR, 
                                puzzle_NOR, puzzle_XNOR, puzzle_15, puzzle_07, puzzle_10, 
                                puzzle_09, puzzle_00, puzzle_17, puzzle_03, puzzle_04, 
                                puzzle_05, puzzle_06, puzzle_12, puzzle_02, puzzle_08, 
                                puzzle_11, puzzle_12, puzzle_01, puzzle_13, puzzle_14, 
                                puzzle_15, puzzle_16,/* 15 */
                                test_bed };
