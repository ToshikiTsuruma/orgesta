//=============================================================================
//
// ���͏��� [input.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// �C���v�b�g�N���X
//*****************************************************************************
class CInput
{
public:
	enum class CODE {
		//�V�X�e��
		SELECT = 0,
		BACK,
		PAUSE,
		//�ړ�
		MOVE_UP,	//��
		MOVE_DOWN,	//��
		MOVE_LEFT,	//��
		MOVE_RIGHT,	//�E
		MOVE_RISE,	//�㏸
		MOVE_FALL,	//���~
		//��]
		ROTATE_UP,		//��
		ROTATE_DOWN,	//��
		ROTATE_LEFT,	//��
		ROTATE_RIGHT,	//�E
		//�I��
		SELECT_UP,
		SELECT_DOWN,
		SELECT_LFET,
		SELECT_RIGHT,
		//�A�N�V����
		ATTACK_1,
		ATTACK_2,
		DASH,
		INTERACT,

#ifdef _DEBUG
		//�f�o�b�O�p
		DEBUG_0,
		DEBUG_1,
		DEBUG_2,
		DEBUG_3,
#endif

		CODE_MAX
	};

	CInput();			//�f�t�H���g�R���X�g���N�^
	virtual ~CInput();	//�f�X�g���N�^
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual bool GetPress(CODE code);	//���͔���i�v���X�j
	virtual bool GetTrigger(CODE code);	//���͔���i�g���K�[�j

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;
};

//*****************************************************************************
// �L�[�{�[�h�N���X
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetPress(CODE code);	//���͔���i�v���X�j
	bool GetTrigger(CODE code);	//���͔���i�g���K�[�j

private:
	BYTE m_aKeyState[256];
	BYTE m_aKeyStateTrigger[256];
};

//*****************************************************************************
// �Q�[���p�b�h�N���X(Xinput)
//*****************************************************************************
class CInputGamepadX : public CInput
{
public:
	//�g���K�[�̍��E
	enum class TRIGGER_TYPE {
		RIGHT = 0,
		LEFT,
		ENUM_MAX
	};

	//�X�e�B�b�N�̏㉺���E
	enum class STICK_TYPE {
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		ENUM_MAX
	};

	CInputGamepadX();	//�f�t�H���g�R���X�g���N�^
	~CInputGamepadX();	//�f�X�g���N�^
	void Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	bool GetConnectGamepad(void);	//�Q�[���p�b�h�ڑ��̔���
	bool GetButtonPress(int nButton);		//�{�^���̃v���X����
	bool GetButtonTrigger(int nButton);		//�{�^���̃g���K�[����
	bool GetTriggerPress(TRIGGER_TYPE type);		//�g���K�[�̃v���X����
	bool GetTriggerTrigger(TRIGGER_TYPE type);	//�g���K�[�̃g���K�[����
	bool GetLeftStick(STICK_TYPE type);			//���X�e�B�b�N�̔���
	bool GetRightStick(STICK_TYPE type);			//�E�X�e�B�b�N�̔���
	bool GetPress(CODE code);	//���͔���i�v���X�j
	bool GetTrigger(CODE code);	//���͔���i�g���K�[�j
	void SetVibration(int nLeftValue, int nRightValue, int nCntVibration);	//�o�C�u���[�V�����̐ݒ�

private:
	XINPUT_STATE m_state;		//�R���g���[���[�̏��
	XINPUT_STATE m_stateLast;	//�R���g���[���[�̂ЂƂO�̏�ԁi�g���K�[����p�j
	XINPUT_VIBRATION m_vibration;	//�o�C�u���[�V����
	bool m_bConnect;		//�ڑ��m�F
	int m_nCntVibration;	//�o�C�u���[�V�����p�̃J�E���^�[
	bool m_bVibration;		//�o�C�u���[�V���������s�����ǂ���
};

#endif // !_INPUT_H_