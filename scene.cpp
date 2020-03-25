/*-----------------------------
	scene.cpp
------------------------------*/

#include "scene.h"
#include "title.h"
#include "sousa.h"
#include "rule.h"
#include "game.h"
#include "result.h"

//スタティック変数
static SCENE_INDEX g_SceneIndex;
static SCENE_INDEX g_SceneNextIndex;

typedef void (*SceneFunc)(void);

static const SceneFunc Initialize[] = 
{
	Title_Initialize,
	Sousa_Init,
	Rule_Init,
	Game_Initialize,
	Result_Initialize,
};

//シーンの初期化
void Scene_Initialize(SCENE_INDEX index)
{
	g_SceneNextIndex = g_SceneIndex = index;
	
	Initialize[g_SceneIndex]();
}

//シーンの終了処理
void Scene_Finalize(void)
{
	switch( g_SceneIndex ) 
	{
		case SCENE_INDEX_TITLE:		//タイトル
			Title_Finalize();
			break;

		case SCENE_INDEX_SOUSA:		//操作説明
			Sousa_Uninit();
			break;
		
		case SCENE_INDEX_RULE:
			Rule_Uninit();
			break;

		case SCENE_INDEX_GAME:		//ゲーム
			Game_Finalize();
			break;

		case SCENE_INDEX_RESULT:	//リザルト
			Result_Finalize();
			break;
	}
}

//シーンの更新処理
void Scene_Update(void)
{
	switch( g_SceneIndex ) 
	{
		case SCENE_INDEX_TITLE:		//タイトル
			Title_Update();
			break;	

		case SCENE_INDEX_SOUSA:		//操作説明
			Sousa_Update();
			break;
		
		case SCENE_INDEX_RULE:
			Rule_Update();
			break;

		case SCENE_INDEX_GAME:		//ゲーム
			Game_Update();
			break;
		
		case SCENE_INDEX_RESULT:	//リザルト
			Result_Update();
			break;
	}
}

//シーンの描画
void Scene_Draw(void)
{
	switch( g_SceneIndex ) 
	{
		case SCENE_INDEX_TITLE:
			Title_Draw();
			break;
	
		case SCENE_INDEX_SOUSA:		
			Sousa_Draw();
			break;
	
		case SCENE_INDEX_RULE:
			Rule_Draw();
			break;
	
		case SCENE_INDEX_GAME:
			Game_Draw();
			break;

		case SCENE_INDEX_RESULT:
			Result_Draw();
			break;
	}
}

//現在のシーンをチェック
void Scene_Check(void)
{
	if( g_SceneIndex != g_SceneNextIndex ) {
		Scene_Finalize();
		Scene_Initialize(g_SceneNextIndex);
	}
}

//次のシーンに変更
void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}

