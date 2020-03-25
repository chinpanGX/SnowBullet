/*-----------------------------
	enemylaser.h
------------------------------*/

#pragma once

#include "collision.h"
#include <d3dx9.h>

//íeÇÃç≈ëÂï\é¶êî
#define ENEMYLASER_MAX (10)


void EnemyLaser_Initialize(void);
void EnemyLaser_Update(void);
void EnemyLaser_Draw(void);

void EnemyLaser_Create(float x, float y, D3DXVECTOR2 dir);

void EnemyLaser_Destroy(int index);
bool EnemyLaser_IsEnable(int index);
const CUPSULE* EnemyLaser_GetCollision(int index);

