/*-----------------------------
	enemybullet.h
------------------------------*/

#pragma once

#include "collision.h"
#include <d3dx9.h>

//íeÇÃç≈ëÂï\é¶êî
#define ENEMYBULLET_MAX (1400)


void EnemyBullet_Initialize(void);
void EnemyBullet_Update(void);
void EnemyBullet_Draw(void);

void EnemyBullet_Create(float x, float y, D3DXVECTOR2 dir);

void EnemyBullet_Destroy(int index);
bool EnemyBullet_IsEnable(int index);
const CIRCLE* EnemyBullet_GetCollision(int index);

