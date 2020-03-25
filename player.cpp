/*-----------------------------
	player.cpp
------------------------------*/

#include "player.h"
#include "main.h"
#include "input.h"
#include "sprite.h"
#include "table.h"
#include "bullet.h"
#include "collision.h"
#include "sound.h"

//�}�N����`
#define PLAYER_WIDTH	(32)
#define PLAYER_HEIGHT	(32)

#define ANIME_PATTERN_MAX		3
#define ANIME_PATTERN_SKIPFRAME 8

//�\����
typedef struct
{
	D3DXVECTOR2 pos;			//�ʒu
	float		speed;			//�ړ����x
	int			muki;			//����
	int			color;			//�F

	int			animePattern;	//�A�j���p�^�[��
	int			animeFrame;		//�p�^�[���؂�ւ��E�G�C�g

	CIRCLE		collision;		//�~�̓����蔻��
	int			hitpoint;		//�q�b�g�|�C���g
//	int			limit;			//�u�Ԉړ��̉�
}PLAYER;

PLAYER player;

//�v���C���[�̏�����
void Player_Initialize(void)
{
	//�v���C���[�̏�����
	player.pos.x = SCREEN_WIDTH / 2;
	player.pos.y = SCREEN_HEIGHT / 2;

	player.speed = 0.5f;	//�ړ����x

	player.color = 0;		//�F
	player.muki = 0;		//����

	player.collision.cx = player.pos.x;			//�R���W����x
	player.collision.cy = player.pos.y;			//�R���W����y
	player.collision.r = PLAYER_WIDTH * 0.8f;	//�R���W�������a

	player.hitpoint = 5000;	//�q�b�g�|�C���g
	//player.limit = 5;			//�u�Ԉړ��̉�	
}

//�v���C���[�̏I������
void Player_Finalize(void)
{

}

//�v���C���[�̍X�V
void Player_Update(void)
{
	D3DXVECTOR2 dir = D3DXVECTOR2(0.0f, 0.0f);

//----------------------------------------------------------------------------------------------------	
	/*--------------------------------------
		�v���C���[�̃A�N�V����
	----------------------------------------*/
	/*-----------------------
	|	�ړ��̃C���v�b�g	|
	------------------------*/
	//��
	if (Keyboard_IsPress(DIK_UP) ||Keyboard_IsPress(DIK_W) || GamePad_IsPress(0, BUTTON_UP))
	{
		dir.y += -1;
		player.muki = 3;
		if (player.pos.y < 0 + 50)
		{
			player.pos.y = 0 + 50;
		}
	}
	//��
	if (Keyboard_IsPress(DIK_DOWN) || Keyboard_IsPress(DIK_S) || GamePad_IsPress(0, BUTTON_DOWN)){
		dir.y -= -1;
		player.muki = 0;
		if (player.pos.y > SCREEN_HEIGHT - 50)
		{
			player.pos.y = SCREEN_HEIGHT - 50;
		}
	}
	//��
	if (Keyboard_IsPress(DIK_LEFT) || Keyboard_IsPress(DIK_A) || GamePad_IsPress(0, BUTTON_LEFT)){
		dir.x += -1;
		player.muki = 1;
		if (player.pos.x > SCREEN_WIDTH - 50)
		{
			player.pos.x = SCREEN_WIDTH - 50;
		}
	}
	//�E
	if (Keyboard_IsPress(DIK_RIGHT) || Keyboard_IsPress(DIK_D) || GamePad_IsPress(0, BUTTON_RIGHT)){
		dir.x -= -1;
		player.muki = 2;
		if (player.pos.x < 0 + 50)
		{
			player.pos.x = 0 + 50;

		}
	}

	/*---------------------------
	|	�A�N�V�����C���v�b�g	|
	----------------------------*/
	
	//�X�y�[�X���Q�[���p�b�h��B�{�^���������ꂽ��e�𔭎�
	if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(0, BUTTON_B))
	{
		PlaySound(SOUND_LABEL_SE_SHOT);	//�V���b�gSE�̍Đ�
		
		//�J�[�\���L�[���͂�����Ă��Ȃ��ꍇ�A�������甭�˕������쐬����
		if (D3DXVec2Length(&dir) < 0.01f) 
		{
			switch (player.muki)
			{
			case 0://������
				dir.y = 1.0f;
				break;
			case 1://������
				dir.x = -1.0f;
				break;
			case 2://�E����
				dir.x = 1.0f;
				break;
			case 3://�����
				dir.y = -1.0f;
				break;
			}
		}
		Bullet_Create(player.pos.x, player.pos.y, dir);
		dir = D3DXVECTOR2(0.0f, 0.0f);
	}
#if 0
	//�v���C���[�̃��~�b�g���O�ȏ�
	if (player.limit > 0)
	{
		//Q�L�[���Q�[���p�b�h��L�{�^��
		if (Keyboard_IsPress(DIK_Q) || GamePad_IsPress(0, BUTTON_L))
		{
			dir.x += -1;
			player.muki = 1;
			if (player.pos.x > SCREEN_WIDTH - 50)
			{
				player.pos.x = SCREEN_WIDTH - 50;
			}
			player.limit--;
		}
		//E�L�[���Q�[���p�b�h��R�{�^��
		if (Keyboard_IsPress(DIK_E) || GamePad_IsPress(0, BUTTON_R))
		{
			dir.x -= -1;
			player.muki = 2;
			if (player.pos.x < 0 + 50)
			{
				player.pos.x = 0 + 50;

			}
			player.limit--;
		}
	}
#endif
//--------------------------------------------------------------------------------------------

	//�}�b�v�Ƃ̃q�b�g�`�F�b�N
	Collision_HitCheck_TileMap(player.pos, &dir);

	//�΂߂̓���
	D3DXVec2Normalize(&dir, &dir);
	D3DXVECTOR2 vecSpeed = dir * player.speed;
	player.pos += vecSpeed;
	

	// ���W�̍X�V����
	player.pos += dir;

	//�����蔻��p���W�̍X�V
	player.collision.cx = player.pos.x;
	player.collision.cy = player.pos.y;

	//�X�L�b�v����t���[���l�𒴂�����
	if (++player.animeFrame > ANIME_PATTERN_SKIPFRAME)
	{
		//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
		if (++player.animePattern >= ANIME_PATTERN_MAX)
			player.animePattern = 0;
		//�t���[���͌��ɖ߂�
		player.animeFrame = 0;
	}
}

//�v���C���[�̕`��
void Player_Draw(void)
{
	Sprite_SetColor(D3DCOLOR_RGBA(255, 255, 0, 255));
	Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
		player.pos.x,
		player.pos.y,
		GetAnimTbl(player.color, player.muki, player.animePattern).x * 256,
		GetAnimTbl(player.color, player.muki, player.animePattern).y * 256,
		32,
		32,
		16,
		16,
		1.0f,
		1.0f,
		0.0f);
}

//�v���C���[�̃R���W����
const CIRCLE* Player_GetCollision()
{
	return &player.collision;
}

//�v���C���[�̃_���[�W
void Player_AddDamage(int damage)
{
	PlaySound(SOUND_LABEL_SE_HIT);	//�q�b�gSE�̍Đ�
	player.hitpoint -= damage;
	if (player.hitpoint < 0)
		player.hitpoint = 0;
}

//�q�b�g�|�C���g�̃Q�b�^�[
int Player_GetHitPoint()
{
	return player.hitpoint;
}
