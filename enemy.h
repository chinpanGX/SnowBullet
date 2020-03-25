/*-----------------------------
	enemy.h
------------------------------*/

#pragma once


#include "collision.h"


#define ENEMY_COUNT 20	//エネミーの数

//プロトタイプ宣言
void Enemy_Initialize(void);
void Enemy_Finalize(void);
void Enemy_Update(void);
void Enemy_Draw(void);
void Enemy_Destroy(int index);
bool Enemy_IsEnable(int index);
const CIRCLE* Enemy_GetCollision(int index);
void Enemy_KillCounter(int killcount);
int Enemy_GetKillCount();
void Enemy_Create();

