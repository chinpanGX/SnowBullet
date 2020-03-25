/*------------------------------------
	enemygenerater.cpp
--------------------------------------*/

#include "enemygenerater.h"
#include "enemy.h"
#include "sprite.h"
#include "texture.h"

//スタティック変数
static int frameCounter = 0;	//初期化でも0代入忘れずに

//初期化
void EnemyGenerater_Init(void)
{

}

//終了処理
void EnemyGenerater_Uninit(void)
{

}

//更新
void EnemyGenerater_Update(void)
{
	if(frameCounter % 40 == 0)
	{
		Enemy_Create();
	}
	frameCounter++;
}