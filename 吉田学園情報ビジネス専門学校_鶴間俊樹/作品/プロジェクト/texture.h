//=============================================================================
//
// �e�N�X�`������ [texture.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_TEXTURE_FILE_PATH (128)	//�e�N�X�`���̃t�@�C���̃p�X�̒����̍ő�

//*****************************************************************************
// �e�N�X�`���N���X
//*****************************************************************************
class CTexture
{
public:
	enum class TEXTURE_TYPE {
		NONE = 0,

		//UI�̃e�N�X�`��
		BG_TITLE,
		TEXT_TITLENAME,
		TEXT_GAMESTART,
		TEXT_TUTORIAL,
		TEXT_CREDIT,
		QUIT_GAME,
		TUTORIAL,
		CREDIT,
		SELECT_ICON,

		MENU_BG,
		RATE_BG,
		TIMER_FRAME,
		SCORE_FRAME,
		ICON_NONE,
		ICON_APPLE,
		ICON_HP,
		SILHOUETTE_TREE,
		MOON,
		ICON_INTERACT_KEYBOARD,
		ICON_INTERACT_GAMEPAD,
		ICON_NIX_SIGN,

		GAUGE_HORIZONTAL,
		GAUGE_VERTICAL,
		HP_GAUGE_FRAME,
		GROW_GAUGE_FRAME,
		TEXT_GROWUP,
		ARROW_LEFT,
		ARROW_RIGHT,
		PAUSE_BG,

		UPDATE_HISCORE,
		TEXT_GAMECLEAR,
		TEXT_GAMEOVER,
		TEXT_QUIT,
		TEXT_RETRY,
		TEXT_CONTINUE,
		TEXT_TITLE,
		TEXT_ENERGY,
		TEXT_COMBO,
		TEXT_X1,
		TEXT_X2,
		TEXT_X3,
		TEXT_X5,
		TEXT_SCORE,
		TEXT_HISCORE,

		//���f���̃e�N�X�`��

		//�G�t�F�N�g�̃e�N�X�`��
		EFFECT_EXPLOSION,		// �����G�t�F�N�g
		EFFECT_DAMAGE_PLAYER,	// �v���C���[�U���̃_���[�W
		EFFECT_DAMAGE_ENEMY,	// �G�̍U���̃_���[�W
		EFFECT_DAMAGE_BULLET,	// �e�̃_���[�W�G�t�F�N�g
		EFFECT_DEATH,			// ���S�G�t�F�N�g
		EFFECT_HEAL_APPLE,		// �ь�ɂ��񕜃G�t�F�N�g
		EFFECT_DRAIN_APPLE,		// �ь�ɂ��HP�z���G�t�F�N�g
		EFFECT_REPULSIVEWAVE_APPLE,	// �ь�ɂ��˗͔g�����G�t�F�N�g
		EFFECT_FIRE_BOLL,		// �΂̋�
		EFFECT_FIRE,			// ��
		EFFECT_THUNDERBOLT,		// ����
		EFFECT_THUNDER,			// ��
		EFFECT_WIND,			// ���̉Q
		EFFECT_PARTICLE,		// �p�[�e�B�N��
		EFFECT_SHOCKWAVE,		// �Ռ��g
		EFFECT_REPULSIVEWAVE,	// �˗͔g

		//�i���o�[�̃e�N�X�`��
		NUMBER_001,
		NUMBER_002,
		NUMBER_003,
		NUMBER_004,
		NUMBER_005,
		NUMBER_006,

		ENUM_MAX
	};

	CTexture();		//�f�t�H���g�R���X�g���N�^
	~CTexture();	//�f�X�g���N�^

	static HRESULT Load(void);	//�e�N�X�`���f�[�^�̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���f�[�^�̉��
	static LPDIRECT3DTEXTURE9 GetTexture(TEXTURE_TYPE type);	//�e�N�X�`���̎擾
	static char* GetPathName(TEXTURE_TYPE type);				//�e�N�X�`���̃p�X�̕�����̎擾

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[(int)TEXTURE_TYPE::ENUM_MAX];	//�e�N�X�`���ւ̃|�C���^
	static char m_asFilePath[(int)TEXTURE_TYPE::ENUM_MAX][MAX_TEXTURE_FILE_PATH];	// �t�@�C���̃p�X
};

#endif // !_TEXTURE_H_