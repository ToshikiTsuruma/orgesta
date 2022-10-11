//=============================================================================
//
// �}�l�[�W���[���� [manager.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "manager.h"
#include "input.h"
#include "camera.h"
#include "sound.h"
#include "renderer.h"
#include "fade.h"
#include "scene.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "texture.h"
#include "model.h"

#include "gameScene.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CManager* CManager::m_pManager = nullptr;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CManager::CManager() {
	m_pRenderer = nullptr;
	m_pInputCur = nullptr;
	m_pInputKeyboard = nullptr;
	m_pInputGamepadX = nullptr;
	m_bEnableXInput = false;
	m_pCamera = nullptr;
	m_pSound = nullptr;
	m_pFade = nullptr;
	m_pScene = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager() {

}

//=============================================================================
// �}�l�[�W���[�̐���
//=============================================================================
void CManager::Create(void) {
	//��������Ă��Ȃ��ꍇ����
	if (m_pManager == nullptr) m_pManager = new CManager;
}
 
//=============================================================================
// �}�l�[�W���[�̔j��
//=============================================================================
void CManager::Destroy(void) {
	if (m_pManager != nullptr) {
		delete m_pManager;
		m_pManager = nullptr;
	}
}

//=============================================================================
// �}�l�[�W���[�̎擾 
//=============================================================================
CManager* CManager::GetManager(void) {
	return m_pManager;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow) {
	//�����̎�̏�����
	srand((unsigned)time(nullptr));

	//------------------------------
	//�C���X�^���X����
	//------------------------------
	//�L�[�{�[�h�̐���
	if (m_pInputKeyboard == nullptr) m_pInputKeyboard = new CInputKeyboard;
	if (m_pInputKeyboard != nullptr) m_pInputKeyboard->Init(hInstance, hWnd);	//������
	//�Q�[���p�b�h�̐���(Xinput)
	if (m_pInputGamepadX == nullptr) m_pInputGamepadX = new CInputGamepadX;
	if (m_pInputGamepadX != nullptr) {
		//������
		m_pInputGamepadX->Init();
		//���݂̓��̓f�o�C�X�̐ݒ�		
		if (m_pInputGamepadX->GetConnectGamepad()) {	//�Q�[���p�b�h���ڑ�����Ă���ꍇ
			m_pInputCur = m_pInputGamepadX;	//�Q�[���p�b�h����
			m_bEnableXInput = true;
		}
		else {	//�Q�[���p�b�h���ڑ�����Ă��Ȃ��ꍇ
			m_pInputCur = m_pInputKeyboard;	//�L�[�{�[�h����
			m_bEnableXInput = false;
		}
	}

	//�����_���[�̃C���X�^���X����
	if (m_pRenderer == nullptr) m_pRenderer = new CRenderer;
	if (m_pRenderer != nullptr) m_pRenderer->Init(hWnd, bWindow);

	//�T�E���h�̐���
	if (m_pSound == nullptr) m_pSound = new CSound;
	if (m_pSound != nullptr) m_pSound->Init(hWnd);

	//�t�F�[�h�̐���
	if (m_pFade == nullptr) m_pFade = new CFade;
	if (m_pFade != nullptr) m_pFade->Init();

	//------------------------------
	//���[�h
	//------------------------------
	//�e�N�X�`���̃��[�h
	CTexture::Load();
	//���f���̃��[�h
	CModel::Load();


	//�V�[���̐���
	CScene::ChangeScene(m_pScene, CScene::SCENE_TYPE::TITLE);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	//�t�F�[�h�̔j��
	if (m_pFade != nullptr) {
		m_pFade->Uninit();
		delete m_pFade;
		m_pFade = nullptr;
	}

	//�V�[���̔j��
	if (m_pScene != nullptr) {
		m_pScene->Uninit();
		delete m_pScene;
		m_pScene = nullptr;
	}

	//�J�����̔j��
	if (m_pCamera != nullptr) {
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = nullptr;
	}

	//�T�E���h�̔j��
	if (m_pSound != nullptr) {
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	//���f���̃A�����[�h
	CModel::Unload();
	//�e�N�X�`���̃A�����[�h
	CTexture::Unload();
	//�����_���[�̔j��
	if (m_pRenderer != nullptr) {
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//�L�[�{�[�h�̔j��
	if (m_pInputKeyboard != nullptr) {
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}
	//�Q�[���p�b�h�̔j��(Xinput)
	if (m_pInputGamepadX != nullptr) {
		m_pInputGamepadX->Uninit();
		delete m_pInputGamepadX;
		m_pInputGamepadX = nullptr;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void) {
	//�T�E���h�̍Đ��J�n���t���O�����Z�b�g
	if (m_pSound != nullptr) {
		m_pSound->ResetBeginPlay();
	}

	//�L�[�{�[�h�̍X�V����
	if (m_pInputKeyboard != nullptr) {
		m_pInputKeyboard->Update();
	}
	//�Q�[���p�b�h�̍X�V����(Xinput)
	if (m_pInputGamepadX != nullptr) {
		m_pInputGamepadX->Update();
	}
	//���݂̓��̓f�o�C�X�̐ݒ�
	if (m_pInputGamepadX != nullptr) {
		//�Q�[���p�b�h���ڑ�����Ă���ꍇ
		if (m_pInputGamepadX->GetConnectGamepad()) {
			m_pInputCur = m_pInputGamepadX;
			m_bEnableXInput = true;
		}
		else {
			m_pInputCur = m_pInputKeyboard;
			m_bEnableXInput = false;
		}
	}

	//�����_���[�̍X�V����
	if (m_pRenderer != nullptr) {
		m_pRenderer->Update();
	}

	//�V�[���̍X�V����
	if (m_pScene != nullptr) m_pScene->Update();

	//�t�F�[�h�̍X�V����
	if (m_pFade != nullptr) m_pFade->Update();

	//�J�����̍X�V
	if (m_pCamera != nullptr) m_pCamera->Update();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void) {
	//�����_���[�̕`��
	if (m_pRenderer != nullptr) m_pRenderer->Draw();
}

//=============================================================================
// �V�[���̕ύX
//=============================================================================
void CManager::ChangeScene(int nTypeScene) {
	//�V�[���̕ύX
	CScene::ChangeScene(m_pScene, (CScene::SCENE_TYPE)nTypeScene);
}

//=============================================================================
// �J�����̐ݒ�
//=============================================================================
void CManager::SetCamera(CCamera* pCamera) {
	if (pCamera == nullptr) return;
	//���݂̃J�����̔j��
	if (m_pCamera != nullptr) m_pCamera->Uninit();
	//�V�����J�����̐ݒ�
	m_pCamera = pCamera;
}

//=============================================================================
// �Q�[���V�[���̎擾
//=============================================================================
CGameScene* CManager::GetGameScene(void) {
	return dynamic_cast<CGameScene*>(m_pScene);
}