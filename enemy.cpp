/*-----------------------------
	enemy.cpp
------------------------------*/

#include "enemy.h"
#include "main.h"
#include "sprite.h"
#include "table.h"
#include "player.h"
#include "enemybullet.h"
#include "enemylaser.h"
#include "explosion.h"
#include "collision.h"

#define _USE_MATH_DEFINES
#include <math.h>

//�}�N����`
#define ENEMY_WIDTH				(32)
#define ENEMY_HEIGHT			(32)
#define ANIME_PATTERN_MAX		(3)
#define ANIME_PATTERN_SKIPFRAME (8)

#define ENEMY_SEARCH_RADIUS		(200.0f)
#define ENEMY_SEARCH_ANGLE		(0.5f)
#define ENEMY_CHASE_SPEED		(1.0f)
#define ENEMY_RETURN_SPEED		(1.0f)


//�\����
typedef struct
{
	int			enable;		//�����t���O

	D3DXVECTOR2 pos;		//�ʒu
	D3DXVECTOR2	velosity;	//���x

	float		rot;		//��]
	int			color;		//�F
	int			muki;		//�����i�ړ������j

	int			animePattern;	//�A�j���p�^�[��
	int			animeFrame;		//�p�^�[���؂�ւ��E�G�C�g

	CIRCLE		collision;

	int			frame;
	int			frame_alpha;
	int			state;
	D3DXVECTOR2 dir_shot;
	D3DXVECTOR2 pos_return;
	
	int killcounter;		//�L����

}ENEMY_DATA;

ENEMY_DATA enemy[ENEMY_COUNT];


//�񋓌^�̒�`
typedef enum
{
	ENEMY_STATE_INIT,		//������
	ENEMY_STATE_SEARCH,		//���͂��T�[�`
	ENEMY_STATE_FIND,		//�G�𔭌�
	ENEMY_STATE_CHASE,		//�ǐ�
	ENEMY_STATE_SHOT,		//�V���b�g����
	ENEMY_STATE_LASER,		//���[�U�[����
	ENEMY_STATE_COOLDOWN,	//�V���b�g�I��
	ENEMY_STATE_RETURN,		//��ʒu�ɖ߂�
	ENEMY_STATE_COUNT,		//�G�l�~�[�̐�
	ENEMY_STATE_DEAD,		//���S���
	ENEMY_STATE_CREATE,		//�G�̐���

	ENEMY_STATE_MAX

}ENEMY_STATE;

//�O���[�o���ϐ�
int Enemy_CalcMuki(D3DXVECTOR2 dir);

//�X�^�e�B�b�N�ϐ�
static int KillCount;

//�v���g�^�C�v�錾
void Enemy_StateInit(int index);
void Enemy_StateSearch(int index);
void Enemy_StateFind(int index);
void Enemy_StateChase(int index);
void Enemy_StateShot(int index);
void Enemy_StateLaser(int index);
void Enemy_StateCooldown(int index);
void Enemy_StateReturn(int index);

//������
void Enemy_Initialize(void)
{
	KillCount = 0;
	//�G�̏�����
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		Enemy_StateInit(i);
		enemy[i].state = ENEMY_STATE_SEARCH;
	}
}

//�I������
void Enemy_Finalize(void)
{

}

//�X�V
void Enemy_Update(void)
{
	D3DXVECTOR2 dir = D3DXVECTOR2(0.0f, 0.0f);

	for (int i = 0; i<ENEMY_COUNT; i++)
	{
		//�����蔻��p���W�̍X�V
		enemy[i].collision.cx = enemy[i].pos.x;
		enemy[i].collision.cy = enemy[i].pos.y;

		//�X�L�b�v����t���[���l�𒴂�����
		if (++enemy[i].animeFrame > ANIME_PATTERN_SKIPFRAME)
		{
			//�A�j���p�^�[����i�߂�(�ő�l�𒴂����烊�Z�b�g)
			if (++enemy[i].animePattern >= ANIME_PATTERN_MAX)
				enemy[i].animePattern = 0;
			//�t���[���͌��ɖ߂�
			enemy[i].animeFrame = 0;
		}

//------------------------------------------------------------
		/*-----------------------------------
		|		�X�e�[�g�̍X�V				|
		------------------------------------*/
		switch (enemy[i].state)
		{
			//�X�e�[�g�̏�����
		case ENEMY_STATE_INIT:
			Enemy_StateInit(i);
			break;

			//�ړ������͂̃T�[�`
		case ENEMY_STATE_SEARCH:	
			Enemy_StateSearch(i);
			break;

			//�v���C���[�̂𔭌�
		case ENEMY_STATE_FIND:
			Enemy_StateFind(i);
			break;
			
			//�ǐ�
		case ENEMY_STATE_CHASE:
			Enemy_StateChase(i);
			break;
			
			//�V���b�g����
		case ENEMY_STATE_SHOT:
			Enemy_StateShot(i);
			break;
			
			//���[�U�[����
		case ENEMY_STATE_LASER:
			Enemy_StateLaser(i);
			break;
		
			//�V���b�g�I��
		case ENEMY_STATE_COOLDOWN:
			Enemy_StateCooldown(i);
			break;
		
			//��ʒu�ɖ߂�
		case ENEMY_STATE_RETURN:
			Enemy_StateReturn(i);
			break;
		default:
			break;
		}
	}
}

//�`��
void Enemy_Draw(void)
{
	for (int i = 0; i < ENEMY_COUNT; i++){

		//�����t���O��TRUE�̓G�̂ݏ������s��
		if (!enemy[i].enable) {
			continue;
		}

		Sprite_Draw(TEXTURE_INDEX_YUKIDARUMA,
			enemy[i].pos.x,
			enemy[i].pos.y,
			GetAnimTbl(enemy[i].color, enemy[i].muki, enemy[i].animePattern).x * 256,
			GetAnimTbl(enemy[i].color, enemy[i].muki, enemy[i].animePattern).y * 256,
			32,
			32,
			16,
			16,
			2.0f,
			2.0f,
			enemy[i].rot);
	}
}

//�G�l�~�[�̎�������
void Enemy_Create()
{
	for (int i = 0; i < ENEMY_COUNT; i++)
	{
		//�G�l�~�[��enable��true�Ȃ�
		if (enemy[i].enable)
		{
			continue;	//�������Ȃ�
		}

		/*�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
		�b			�G�̈ʒu					�b
		�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[*/
		//�o���ʒu�̓����_��
		enemy[i].pos.x = frand() * SCREEN_WIDTH;
		enemy[i].pos.y = frand() * SCREEN_HEIGHT;

		/*-------------------------------------------
		|	��ʂ̃u���b�N�ɏ���Ă����炸�炷		|
		-------------------------------------------*/
		//X��
		enemy[i].pos.x = frand() * SCREEN_WIDTH;
		if (enemy[i].pos.x < 0 + 64)
		{
			enemy[i].pos.x = 64;
		}
		if (enemy[i].pos.x > SCREEN_WIDTH - 64)
		{
			enemy[i].pos.x = SCREEN_WIDTH - 64;
		}
		
		//Y��
		enemy[i].pos.y = frand() * SCREEN_HEIGHT;
		if (enemy[i].pos.y < 0 + 64)
		{
			enemy[i].pos.y = 64;
		}
		if (enemy[i].pos.y > SCREEN_HEIGHT - 64)
		{
			enemy[i].pos.y = SCREEN_HEIGHT - 64;
		}
		
		enemy[i].enable = true;		//�G�t���O��true�ɂ���
		
		enemy[i].state = ENEMY_STATE_SEARCH;	//�X�e�[�g���T�[�`��Ԃֈڍs
		break;
	}	
}

//�G�̎��S����
void Enemy_Destroy(int index)
{
	enemy[index].state = ENEMY_STATE_DEAD;
	enemy[index].enable = false;

}

//�G�l�~�[�̐����t���O
bool Enemy_IsEnable(int index)
{
	return enemy[index].enable;
}

//�R���W�����̔���
const CIRCLE* Enemy_GetCollision(int index)
{
	return &enemy[index].collision;
}

//���������
int Enemy_CalcMuki(D3DXVECTOR2 dir)
{
	int muki = 0;

	//�ړ��������L�����`�F�b�N
	if (D3DXVec2Length(&dir) > 0.01f)
	{
		//X������Y�����̑傫�����ׂāA�ǂ���̕����傫���̂��𒲂ׂ�
		//X�����̕����傫���ꍇ
		if (fabs(dir.x) > fabs(dir.y))
		{
			//X�������v���X�̒l�Ȃ̂��}�C�i�X�̒l�Ȃ̂��𒲂ׂ�
			//�}�C�i�X�����̏ꍇ�͍�����
			if (dir.x < 0.0f)
				muki = 1;
			//��������Ȃ��i�v���X�����j�ꍇ�͉E����
			else
				muki = 2;
		}
		//Y�����̕����傫���ꍇ
		else
		{
			//Y�������v���X�̒l�Ȃ̂��}�C�i�X�̒l�Ȃ̂��𒲂ׂ�
			//�}�C�i�X�����̏ꍇ�͏����
			if (dir.y < 0.0f)
				muki = 3;
			//��������Ȃ��i�v���X�����j�ꍇ�͏����
			else
				muki = 0;
		}
	}

	return muki;
}

//�T�[�`��Ԃ̏�����
void Enemy_StateInit(int index)
{
//---------------------------�@-------------------------------------------------
	/*�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
	�b			�G�̈ʒu					�b
	�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[*/
	//�o���ʒu�̓����_��
	enemy[index].pos.x = frand() * SCREEN_WIDTH;
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;

	/*-------------------------------------------
	|	��ʂ̃u���b�N�ɏ���Ă����炸�炷		|
	-------------------------------------------*/
	//X��
	enemy[index].pos.x = frand() * SCREEN_WIDTH;
	if (enemy[index].pos.x < 0 + 64)
	{
		enemy[index].pos.x = 64;
	}
	if(enemy[index].pos.x > SCREEN_WIDTH - 64)
	{
		enemy[index].pos.x = SCREEN_WIDTH - 64;
	}
	//Y��
	enemy[index].pos.y = frand() * SCREEN_HEIGHT;
	if (enemy[index].pos.y < 0 + 64 )
	{
		enemy[index].pos.y = 64;
	}
	if(enemy[index].pos.y > SCREEN_HEIGHT - 64)
	{
		enemy[index].pos.y = SCREEN_HEIGHT - 64;
	}
//------------------------------------------------------------------------------

	enemy[index].velosity.x = 1.0f;	//x�����̈ړ����x
	enemy[index].velosity.y = 1.0f;	//y�����̈ړ����x

	enemy[index].rot = 0;		//��]
	enemy[index].color = 1;		//�F

	enemy[index].muki = rand() % 4;	//����
	enemy[index].enable = TRUE;		//�����t���O

	enemy[index].collision.cx = enemy[index].pos.x;	//�R���W����X
	enemy[index].collision.cy = enemy[index].pos.y;	//�R���W����Y
	enemy[index].collision.r = ENEMY_WIDTH * 0.8f;	//�R���W�������a

	enemy[index].frame = 0;

	//�X�e�[�g���T�[�`��Ԃֈڍs
	enemy[index].state = ENEMY_STATE_SEARCH;

}

//���͂̃T�[�`
void Enemy_StateSearch(int index)
{
	//�t���[����i�߂�
	enemy[index].frame++;

	//�G�̈ړ�
	enemy[index].pos.x += enemy[index].velosity.x;
	enemy[index].pos.y += enemy[index].velosity.y;
	
	D3DXVECTOR2 dir;
//-----------------------------------------------------------
	/*---------------------------------------
	|		�}�b�v�Ƃ̃q�b�g�`�F�b�N		|
	----------------------------------------*/
	if (enemy[index].pos.x < 0 + 64)
	{
		enemy[index].pos.x = 64;
		enemy[index].velosity.x *= -1.0f;
	}
	if (enemy[index].pos.x > SCREEN_WIDTH - 64)
	{
		enemy[index].pos.x = SCREEN_WIDTH - 64;
		enemy[index].velosity.x *= -1.0f;
	}
	if (enemy[index].pos.y < 0 + 64)
	{
		enemy[index].pos.y = 64;
		enemy[index].velosity.y *= -1.0f;
	}
	if (enemy[index].pos.y > SCREEN_HEIGHT - 64)
	{
		enemy[index].pos.y = SCREEN_HEIGHT - 64;
		enemy[index].velosity.y *= -1.0f;
	}
//---------------------------------------------------------------

	//�����Ă�������̃x�N�g�������
	switch (enemy[index].muki)
	{
	case 0://������
		dir = D3DXVECTOR2( 0.0f,  1.0f);
		break;
	case 1://������
		dir = D3DXVECTOR2(-1.0f,  0.0f);
		break;
	case 2://�E����
		dir = D3DXVECTOR2( 1.0f,  0.0f);
		break;
	case 3://�����
		dir = D3DXVECTOR2( 0.0f, -1.0f);
		break;
	}

//-----------------------------------------------------------------------------
	/*-----------------------------------
	|	  �v���C���[���T�[�`	    	|
	------------------------------------*/
	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->cx, Player_GetCollision()->cy);

	//�G�ƃv���C���[�̋������v�Z����
	D3DXVECTOR2 vLen = enemy[index].pos - pl_pos;
	float length = D3DXVec2Length(&vLen);
#if 0
	//���肷�鋗���̒��ɓ����Ă��Ȃ��ꍇ
	if (length > ENEMY_SEARCH_RADIUS)
	{
		enemy[index].state = ENEMY_STATE_COOLDOWN;
	}
#endif
	//���肷�鋗���̒��ɓ����Ă��邩���`�F�b�N����
	if (length < ENEMY_SEARCH_RADIUS)
	{
		//�G���猩�ăv���C���[�̍��W�ւ̕����x�N�g�������
		D3DXVECTOR2 pl_dir = pl_pos - enemy[index].pos;
		D3DXVec2Normalize(&pl_dir, &pl_dir);

		//�G�̌����ƃv���C�����W�ւ̕����x�N�g���œ��ς����
		float dot = D3DXVec2Dot(&dir, &pl_dir);

		//���肷��p�x���ɓ����Ă��邩���`�F�b�N����
		if (dot > ENEMY_SEARCH_ANGLE)
		{
			Explosion_Create(enemy[index].pos.x, enemy[index].pos.y);

			//����ɒʂ�΃v���C���[�����X�e�[�g�Ɉڍs����
			enemy[index].state = ENEMY_STATE_FIND;
			enemy[index].frame = 0;

			//�߂��ė�����W��ۑ�����
			enemy[index].pos_return = enemy[index].pos;
		}
	}
//--------------------------------------------------------------------------------------

	//������ω�������
	if (enemy[index].frame > 60)
	{
		enemy[index].muki = (enemy[index].muki + 1) % 4;
		enemy[index].frame = 0;
	}
}

//�v���C���[�̔���
void Enemy_StateFind(int index)
{
	//�t���[����i�߂�
	enemy[index].frame++;

	//��莞�Ԍo������ǐՃX�e�[�g�ֈڍs
	if (enemy[index].frame > 20){

		enemy[index].frame = 0;

		//�X�e�[�g���T�[�`��Ԃֈڍs
		enemy[index].state = ENEMY_STATE_CHASE;
	}
}

//�v���C���[��ǐ�
void Enemy_StateChase(int index)
{
	//�t���[����i�߂�
	enemy[index].frame++;

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->cx, Player_GetCollision()->cy);
	
	D3DXVECTOR2 dir = pl_pos - enemy[index].pos;
	D3DXVec2Normalize(&dir, &dir);
	dir *= ENEMY_CHASE_SPEED;

	enemy[index].pos += dir;

	//�ړ�����������������
	enemy[index].muki = Enemy_CalcMuki(dir);

	//��莞�Ԍo������e�ˏo�X�e�[�g�ֈڍs
	if (enemy[index].frame > 120){

		enemy[index].frame = 0;

		//�ړ�������ۑ�
		enemy[index].dir_shot = dir;

		//�����_���Łu�΂�T���e�v�������́u���[�U�[�v���ˏo
		if (rand() % 2)
		{
			//�X�e�[�g���T�[�`��Ԃֈڍs
			enemy[index].state = ENEMY_STATE_SHOT;
		}
		else
		{
			//�X�e�[�g���T�[�`��Ԃֈڍs
			enemy[index].state = ENEMY_STATE_LASER;
		}
	}
}

//----------------------------------------------------------------------------------------------------
	/*--------------------------------------
	|		�G�l�~�[�̍U���A�N�V����       |
	--------------------------------------*/
//�V���b�g�𔭎�
void Enemy_StateShot(int index)
{
	//�t���[����i�߂�
	enemy[index].frame++;

	float shot_rad = atan2(enemy[index].dir_shot.y, enemy[index].dir_shot.x);
	shot_rad = (float)(M_PI * 2 / 20)*enemy[index].frame;

	//�ˏo�p�x�N�g�����t���[���l�ŉ�]������
	D3DXVECTOR2 shot_dir;
	shot_dir.x = enemy[index].dir_shot.x*cosf(shot_rad) - enemy[index].dir_shot.y*sinf(shot_rad);
	shot_dir.y = enemy[index].dir_shot.x*sinf(shot_rad) + enemy[index].dir_shot.y*cosf(shot_rad);

	EnemyBullet_Create(enemy[index].pos.x, enemy[index].pos.y, shot_dir);

	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
	if (enemy[index].frame > 90){

		enemy[index].frame = 0;

		//�X�e�[�g���A�ҏ�Ԃֈڍs
		enemy[index].state = ENEMY_STATE_COOLDOWN;
	}
}
//���[�U�[�𔭎�
void Enemy_StateLaser(int index)
{
	if (enemy[index].frame == 0)
	{
		EnemyLaser_Create(enemy[index].pos.x, enemy[index].pos.y, enemy[index].dir_shot);
	}

	//�t���[����i�߂�
	enemy[index].frame++;


	//��莞�Ԍo������N�[���_�E���X�e�[�g�ֈڍs
	if (enemy[index].frame > 90)
	{

		enemy[index].frame = 0;

		//�X�e�[�g���A�ҏ�Ԃֈڍs
		enemy[index].state = ENEMY_STATE_COOLDOWN;
	}
}
//-----------------------------------------------------------------------------------------------------

//�U���A�N�V������̃N�[���_�E��
void Enemy_StateCooldown(int index)
{
	//�t���[����i�߂�
	enemy[index].frame++;

	//��莞�Ԍo������A�҃X�e�[�g�ֈڍs
	if (enemy[index].frame > 30)
	{

		enemy[index].frame = 0;

		//�X�e�[�g���A�ҏ�Ԃֈڍs
		enemy[index].state = ENEMY_STATE_RETURN;
	}
}

//��ʒu�ɖ߂�
void Enemy_StateReturn(int index)
{
	//�t���[����i�߂�
	enemy[index].frame++;

	//�v���C���[�̍��W���擾����
	D3DXVECTOR2 pl_pos = D3DXVECTOR2(Player_GetCollision()->cx, Player_GetCollision()->cy);

	//�A�҂�����W�ւ̌����x�N�g�����v�Z����
	D3DXVECTOR2 dir = enemy[index].pos_return - enemy[index].pos;

	//�x�N�g���̒������v�Z����
	float length = D3DXVec2Length(&dir);

	D3DXVec2Normalize(&dir, &dir);
	dir *= ENEMY_CHASE_SPEED;

	enemy[index].pos += dir;

	//�ړ�����������������
	enemy[index].muki = Enemy_CalcMuki(dir);

	//�A�҂�����W�ɓ���������
	if (length <= ENEMY_CHASE_SPEED){

		enemy[index].frame = 0;

		//�X�e�[�g���T�[�`��Ԃֈڍs
		enemy[index].state = ENEMY_STATE_SEARCH;
	}
}


//�G�l�~�[�̃L����
void Enemy_KillCounter(int killcount)
{
	KillCount += killcount;
}

//�G�l�~�[�̍ŏI�L�����̃Q�b�^�[
int Enemy_GetKillCount()
{
	return KillCount;
}

