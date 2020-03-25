/*-----------------------------
	bullet.h
------------------------------*/

#pragma once

#include "collision.h"
#include <d3dx9.h>

//íeÇÃç≈ëÂï\é¶êî
#define BULLET_MAX (128)


void Bullet_Initialize(void);
void Bullet_Update(void);
void Bullet_Draw(void);

void Bullet_Create(float x, float y, D3DXVECTOR2 dir);

void Bullet_Destroy(int index);
bool Bullet_IsEnable(int index);
const CIRCLE* Bullet_GetCollision(int index);

