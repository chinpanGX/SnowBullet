/*-----------------------------
	player.h
------------------------------*/

#pragma once

#include "collision.h"


void Player_Initialize(void);
void Player_Finalize(void);
void Player_Update(void);
void Player_Draw(void);
const CIRCLE* Player_GetCollision();

void Player_AddDamage(int damage);
int Player_GetHitPoint();
