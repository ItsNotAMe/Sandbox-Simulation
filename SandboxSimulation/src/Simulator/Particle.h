#pragma once

#include <cstdint>

#include "../Util.h"

// from https://github.com/GameEngineering/EP01_SandSim/blob/master/source/main.c
struct Particle
{
	bool HasBeenUpdatedThisFrame = false;
	bool IsUsingVelocity = false;
	uint32_t Color = 0x000000;
	uint8_t Id = 0;
	float LifeTime = 0;
	vec2 Velocity = { 0, 0 };
};

// For now, all particle information will simply be a value to determine its material id
#define MAT_ID_EMPTY     (uint8_t)0
#define MAT_ID_SAND      (uint8_t)1
#define MAT_ID_WATER     (uint8_t)2
#define MAT_ID_STEAM     (uint8_t)3
#define MAT_ID_WOOD      (uint8_t)4
#define MAT_ID_FIRE      (uint8_t)5
#define MAT_ID_SALT      (uint8_t)6
#define MAT_ID_SMOKE     (uint8_t)7
#define MAT_ID_EMBER     (uint8_t)8
#define MAT_ID_GUNPOWDER (uint8_t)9
#define MAT_ID_OIL       (uint8_t)10
#define MAT_ID_LAVA      (uint8_t)11
#define MAT_ID_STONE     (uint8_t)12
#define MAT_ID_ACID      (uint8_t)13

// Colors
//#define MAT_COLOR_EMPTY     (uint32_t)0x000000
#define MAT_COLOR_SAND      (uint32_t)0x966432
#define MAT_COLOR_WATER     (uint32_t)0x1464AA
#define MAT_COLOR_STEAM     (uint32_t)0xDCDCFA
#define MAT_COLOR_WOOD      (uint32_t)0x3C2814
#define MAT_COLOR_FIRE      (uint32_t)0x961400
#define MAT_COLOR_SALT      (uint32_t)0xC8B4BE
#define MAT_COLOR_SMOKE     (uint32_t)0x323232
#define MAT_COLOR_EMBER     (uint32_t)0xC87814
#define MAT_COLOR_GUNPOWDER (uint32_t)0x3C3C3C
#define MAT_COLOR_OIL       (uint32_t)0x50463C
#define MAT_COLOR_LAVA      (uint32_t)0xC83200
#define MAT_COLOR_STONE     (uint32_t)0x786E78
#define MAT_COLOR_ACID      (uint32_t)0x5AC83C