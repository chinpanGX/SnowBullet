/*-----------------------------
	scene.cpp
------------------------------*/

#include "scene.h"
#include "title.h"
#include "sousa.h"
#include "rule.h"
#include "game.h"
#include "result.h"

//�X�^�e�B�b�N�ϐ�
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

//�V�[���̏�����
void Scene_Initialize(SCENE_INDEX index)
{
	g_SceneNextIndex = g_SceneIndex = index;
	
	Initialize[g_SceneIndex]();
}

//�V�[���̏I������
void Scene_Finalize(void)
{
	switch( g_SceneIndex ) 
	{
		case SCENE_INDEX_TITLE:		//�^�C�g��
			Title_Finalize();
			break;

		case SCENE_INDEX_SOUSA:		//�������
			Sousa_Uninit();
			break;
		
		case SCENE_INDEX_RULE:
			Rule_Uninit();
			break;

		case SCENE_INDEX_GAME:		//�Q�[��
			Game_Finalize();
			break;

		case SCENE_INDEX_RESULT:	//���U���g
			Result_Finalize();
			break;
	}
}

//�V�[���̍X�V����
void Scene_Update(void)
{
	switch( g_SceneIndex ) 
	{
		case SCENE_INDEX_TITLE:		//�^�C�g��
			Title_Update();
			break;	

		case SCENE_INDEX_SOUSA:		//�������
			Sousa_Update();
			break;
		
		case SCENE_INDEX_RULE:
			Rule_Update();
			break;

		case SCENE_INDEX_GAME:		//�Q�[��
			Game_Update();
			break;
		
		case SCENE_INDEX_RESULT:	//���U���g
			Result_Update();
			break;
	}
}

//�V�[���̕`��
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

//���݂̃V�[�����`�F�b�N
void Scene_Check(void)
{
	if( g_SceneIndex != g_SceneNextIndex ) {
		Scene_Finalize();
		Scene_Initialize(g_SceneNextIndex);
	}
}

//���̃V�[���ɕύX
void Scene_Change(SCENE_INDEX index)
{
	g_SceneNextIndex = index;
}

