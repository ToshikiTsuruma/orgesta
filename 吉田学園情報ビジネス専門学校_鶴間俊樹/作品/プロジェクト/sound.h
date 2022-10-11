//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�N���X
//*****************************************************************************
class CSound
{
public:
	enum class SOUND_LABEL {
		NONE = 0,
		BGM_TITLE,		//�^�C�g��BGM
		BGM_GAME,		//�Q�[��BGM

		TITLE_START,	//�^�C�g����ʂ̃Q�[���X�^�[�g��
		TITLE_OPEN,		//�^�C�g����ʂ̃E�B���h�E���J������
		TITLE_CLOSE,	//�^�C�g����ʂ̃E�B���h�E�������

		SELECT_SWITCH,	//�^�C�g����ʂ̑I��ؑ֎�
		CANSEL,			//�L�����Z����
		FOOTSTEP,		//����
		SWISH_PUNCH,	//����U�鉹
		DAMAGE_PUNCH,	//���ɂ��_���[�W��
		SHOCK_PUNCH,	//�p���`�̏Ռ���
		SHOCK_STAMP,	//���݂���

		HEAL,			//�񕜉�
		GETEXP,			//�o���l�擾��
		DRAIN,			//�̗͋z����
		SHOOT_BULLET,	//�e�̔��ˉ�
		DAMAGE_BULLET,	//�e�̃_���[�W��
		DAMAGE_FIRE,	//���̃_���[�W��
		STUN,			//�C�≹
		EXPLOSION,		//������
		REPULSIVE_WAVE,	//�˗͔g������
		THUNDERBOLT,	//������
		DAMAGE_THUNDER,	//���_���[�W��
		SPAWN_ENEMY,	//�G�̃X�|�[��
		GROW_UP,		//����
		CREATE_APPLE,	//�ь�̐�����
		DEAD_TREE,		//�؂̎��S��
		ENDGAME,		//�Q�[���N���A���̏Ռ��g���N����Ƃ��̉�
		GAMEOVER,		//�Q�[���I�[�o�[
		GAMECLEAR,		//�Q�[���N���A

		ENUM_MAX
	};

	CSound();	//�f�t�H���g�R���X�g���N�^
	~CSound();	//�f�X�g���N�^
	HRESULT Init(HWND hWnd);	//����������
	void Uninit(void);			//�I������
	HRESULT PlaySound(SOUND_LABEL label);	//���̍Đ�
	void StopSound(SOUND_LABEL label);		//���̒�~
	void StopSound(void);					//���̒�~
	void PauseSound(SOUND_LABEL label);		//���̈ꎞ��~
	void StartSound(SOUND_LABEL label);		//���̍ĊJ

	void SetBGM(SOUND_LABEL label);			//�Đ���BGM�̐ݒ�
	SOUND_LABEL GetBGM(void);				//�Đ���BGM�̎擾
	void ResetBeginPlay(void);	//�Đ��J�n���̃t���O�����ׂă��Z�b�g

private:
	// �p�����[�^�\���̒�`
	typedef struct
	{
		char *filename;		// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g
	} PARAM;

	//�����o�֐�
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	
	//�����o�ϐ�
	IXAudio2 *m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice *m_apSourceVoice[(int)SOUND_LABEL::ENUM_MAX] = {};	// �\�[�X�{�C�X
	BYTE *m_apDataAudio[(int)SOUND_LABEL::ENUM_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[(int)SOUND_LABEL::ENUM_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
	static PARAM m_aParam[(int)SOUND_LABEL::ENUM_MAX];						//�t�@�C�����ƃ��[�v

	SOUND_LABEL m_playBGM;	//�Đ�����BGM
	bool m_abBeginPlay[(int)SOUND_LABEL::ENUM_MAX];	//�P�t���[�����Ƃ̍Đ��J�n�����ǂ����̔���
};

#endif // !_SOUND_H_
