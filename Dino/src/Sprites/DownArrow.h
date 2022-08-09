#pragma once

#include <stdint.h>

constexpr uint8_t DownArrowWidth = 6;
constexpr uint8_t DownArrowHeight = 9;

constexpr uint8_t DownArrow[] PROGMEM =
{
	// Dimensions
	DownArrowWidth, DownArrowHeight,

	// Frame 0 - Down_Arrow
	0x40, 0x80, 0x00, 0x80, 0x40, 0x00,
	0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
};