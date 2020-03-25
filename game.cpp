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

//PHASE�̍\����
typedef enum PHASE_INDEX
{
	PHASE_INDEX_FADE,
	PHASE_INDEX_PLAYER_NORMAL,
	PHASE_INDEX_STAGE_CLEAR,
	PHASE_INDEX_MAX
};

//�O���[�o���ϐ�
static PHASE_INDEX g_Phase;
static int g_GameFrameCount;	//�Q�[���t���[���J�E���^�[

//�v���g�^�C�v�錾
static bool Game_EndCheck(void);


//������
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

	PlaySound(SOUND_LABEL_BGM001);	//�Q�[��BGM�Đ�
}

//�I������
void Game_Finalize(void)
{
	Player_Finalize();
	Enemy_Finalize();
	StopSound(SOUND_LABEL_BGM001);	//�Q�[��BGM��~
}

//�X�V
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
		 
		//�����蔻��͕K�����ׂẴA�b�v�f�[�g�������I�������ɍs��
		Collision_Update();

		//�Q�[���̏I���`�F�b�N
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
	Score_Draw(0, 0, Player_GetHitPoint(), 4, true);	//�v���C���[�̃q�b�g�|�C���g�̕`��
	Score_Draw(SCREEN_WIDTH - 100, 0, Enemy_GetKillCount(), 3, true);	//�G�l�~�[�̃L�����̕`��
}

//�Q�[���̏I���`�F�b�N
bool Game_EndCheck(void)
{
	//�v���C���[�̃q�b�g�|�C���g���O
	if (Player_GetHitPoint() <= 0)
	{
		return true;
	}
	return false;
}

