/*-----------------------------
	result.cpp
------------------------------*/

#include "main.h"
#include "game.h"
#include "enemy.h"
#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "score_draw.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

#define RANKING_MAX 5

double ranking[RANKING_MAX];

void  ranking_sort(int score);

//スタティック変数
static bool g_bEnd = false;		//trueで画面遷移
static double count[5];
//int i = 0;

//リザルトの初期化
void Result_Initialize(void)
{
	ranking_sort(Enemy_GetKillCount());	//	値の代入
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	g_bEnd = false;
	PlaySound(SOUND_LABEL_BGM002);	//リザルトBGM再生
}

//リザルトの終了処理
void Result_Finalize(void)
{

}

//リザルトの更新
void Result_Update(void)
{	
	if (!g_bEnd)
	{
		//リザルトからタイトルへ遷移
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(0, BUTTON_Y))
		{
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
			StopSound(SOUND_LABEL_BGM002);	//リザルトBGM停止
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			Scene_Change(SCENE_INDEX_TITLE);
		}
	}
}

//リザルトの描画
void Result_Draw(void)
{
	//killcount = Enemy_GetKillCount();//最終キル数をゲッターを使って代入
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);
	
	/*---------------------------------------------------------------------------------
memo	
		順番は関係なく、数字が高いほうが描画される
		
		配列になっているが、描画が一番上の番号しか描画されない？
		そもそも配列になっていないのか？

	--------------------------------------------------------------------------------*/
	//ranking_sort(Enemy_GetKillCount());	
	
	//ranking_sort(100);
	//count = ranking_sort(15.0);

	for (int i = 0; i < RANKING_MAX; i++)	//	描画を五回繰り返す
	{
		Score_Draw(SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT / 3 + i * 32, ranking[i], 3, true);	
		//Score_Draw(SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT / 3 + 64, ranking[i+1], 3, true);
	}	
}


void ranking_sort(int score)
{
	for (int i = 0; i < RANKING_MAX; i++) 
	{//1位から比較
		//今の順位よりも高ければそれが今回の順位となる
		//(1位より高ければ1位、1位より低く2位より高ければ2位、…)
		if (score > ranking[i])
		{
			for (int j = RANKING_MAX - 1; j > i; j--) //今回の順位以降のスコアをずらす
			{
				ranking[j] = ranking[j - 1]; 
			}
			ranking[i] = score;//今回のスコアを記憶
			break;//以降の順位は調べない
		}
	}
}
