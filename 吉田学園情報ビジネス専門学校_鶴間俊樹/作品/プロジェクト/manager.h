//=============================================================================
//
// �}�l�[�W���[���� [manager.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_LIGHT (3)

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CRenderer;
class CInput;
class CInputKeyboard;
class CInputGamepadX;
class CCamera;
class CSound;
class CFade;
class CScene;
class CGameScene;

//*****************************************************************************
// �}�l�[�W���[�N���X
//*****************************************************************************
class CManager
{
public:
	static void Create(void);		//�}�l�[�W���[�̐���
	static void Destroy(void);		//�}�l�[�W���[�̔j��
	static CManager* GetManager(void);	//�}�l�[�W���[�̎擾

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��

	void ChangeScene(int nTypeScene);	//�V�[���̕ύX

	CRenderer* GetRenderer(void) { return m_pRenderer; }	//�����_���[�̎擾
	CInput* GetInputCur(void) { return m_pInputCur; }		//���݂̓��̓f�o�C�X�̎擾
	bool GetEnableXInput(void) { return m_bEnableXInput; }	//���݂̓��̓f�o�C�X��Xinput���ǂ����̎擾
	void SetCamera(CCamera* pCamera);	//�J�����̐ݒ�
	CCamera* GetCamera(void) { return m_pCamera; }				//�J�����̎擾
	CSound* GetSound(void) { return m_pSound; }	//�T�E���h�̎擾
	CFade* GetFade(void) { return m_pFade; }	//�t�F�[�h�̎擾
	CScene* GetScene(void) { return m_pScene; }	//�V�[���̎擾
	CGameScene* GetGameScene(void);	//�Q�[���V�[���̎擾

private:
	CManager();		//�f�t�H���g�R���X�g���N�^
	~CManager();	//�f�X�g���N�^
	static CManager* m_pManager;	//�}�l�[�W���[�̃C���X�^���X

	CRenderer* m_pRenderer;				//�����_���[�ւ̃|�C���^
	CInput* m_pInputCur;				//���݂̓��̓f�o�C�X�ւ̃|�C���^
	bool m_bEnableXInput;				//Xinput�L��
	CInputKeyboard* m_pInputKeyboard;	//�L�[�{�[�h�ւ̃|�C���^
	CInputGamepadX* m_pInputGamepadX;	//�Q�[���p�b�h�ւ̃|�C���^(Xinput)
	CCamera* m_pCamera;		//�J�����ւ̃|�C���^
	CSound* m_pSound;		//�T�E���h�ւ̃|�C���^
	CFade* m_pFade;			//�t�F�[�h�̃|�C���^
	CScene* m_pScene;		//�V�[���ւ̃|�C���^
};

#endif // !_MANAGER_H_
