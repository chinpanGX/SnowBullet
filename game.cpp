/*-----------------------------
	game.cpp
------------------------------*/

#include "main.h"
#include "texture.h"
#include "player.h"
#include "enemy.h"
#include "fade.h"
#include "scene.h"
#include "collision.h"
#include "bullet.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "explosion.h"
#include "score_draw.h"
#include "tileMap.h"
#include "enemygenerater.h"
#include "sound.h"

//PHASEの構造体
typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_MAX
};

//グローバル変数
static PHASE_INDEX g_Phase;
static int g_GameFrameCount;	//ゲームフレームカウンター

//プロトタイプ宣言
static bool Game_EndCheck(void);


//初期化
void Game_Initialize(void)
{
	Player_Initialize();
	Enemy_Initialize();
	Bullet_Initialize();
	EnemyBullet_Initialize();
	EnemyLaser_Initialize();
	Explosion_Initialize();

	g_Phase = PHASE_INDEX_FADE;
	g_GameFrameCount = 0;
	Fade_Start(false, 30, D3DCOLOR_RGBA(0,0,0,0));

	PlaySound(SOUND_LABEL_BGM001);	//ゲームBGM再生
}

//終了処理
void Game_Finalize(void)
{
	Player_Finalize();
	Enemy_Finalize();
	StopSound(SOUND_LABEL_BGM001);	//ゲームBGM停止
}

//更新
void Game_Update(void)
{
	switch(g_Phase)
	{
	case PHASE_INDEX_FADE:
		if( !Fade_IsFade() ) 
		{
			g_Phase = PHASE_INDEX_PLAYER_NORMAL;
		}
		break;
	case PHASE_INDEX_PLAYER_NORMAL:
		Player_Update();
		Enemy_Update();
		Bullet_Update();
		EnemyBullet_Update();
		EnemyLaser_Update();
		Explosion_Update();
		EnemyGenerater_Update();
		 
		//当たり判定は必ずすべてのアップデート処理が終わった後に行う
		Collision_Update();

		//ゲームの終了チェック
		if (Game_EndCheck())
		{
			Fade_Start(true, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_Phase = PHASE_INDEX_STAGE_CLEAR;
		}
		break;
	case PHASE_INDEX_STAGE_CLEAR:
		if (!Fade_IsFade()) {
			Scene_Change(SCENE_INDEX_RESULT);
		}
		break;
	}
}

void Game_Draw(void)
{
	TileMap_Draw();
	Player_Draw();
	Enemy_Draw();
	Bullet_Draw();
	EnemyBullet_Draw();
	EnemyLaser_Draw();
	Explosion_Draw();
	Score_Draw(0, 0, Player_GetHitPoint(), 4, true);	//プレイヤーのヒットポイントの描画
	Score_Draw(SCREEN_WIDTH - 100, 0, Enemy_GetKillCount(), 3, true);	//エネミーのキル数の描画
}

//ゲームの終了チェック
bool Game_EndCheck(void)
{
	//プレイヤーのヒットポイントが０
	if (Player_GetHitPoint() <= 0)
	{
		return true;
	}
	return false;
}

