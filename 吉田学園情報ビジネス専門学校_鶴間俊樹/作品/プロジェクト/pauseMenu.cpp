//=============================================================================
//
// �|�[�Y���j���[���� [pauseMenu.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "pauseMenu.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "gameScene.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_SELECT (2)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CPauseMenu::CPauseMenu() : CSelectMenu2D(NUM_SELECT, true)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CPauseMenu::~CPauseMenu()
{

}

//=============================================================================
// �ь�I�����j���[�̐�������
//=============================================================================
CPauseMenu* CPauseMenu::Create(void) {
	CPauseMenu* pPauseMenu;
	pPauseMenu = new CPauseMenu();
	if (pPauseMenu == nullptr) return nullptr;

	//�|�[�Y�̐ݒ�
	pPauseMenu->SetPauseLevel(AddUpdatePauseLevel());	//�|�[�Y�̃��x�����グ�ĕԂ�l�Ń|�[�Y�X�V�\���x�����擾���A�I�u�W�F�N�g�̃|�[�Y���x���Ƃ��Đݒ�

	pPauseMenu->Init();

	return pPauseMenu;
}

//=============================================================================
// �ь�I�����j���[�̏���������
//=============================================================================
HRESULT CPauseMenu::Init(void) {
	//�w�i�̐ݒ�
	SetMenuBG(CTexture::TEXTURE_TYPE::PAUSE_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	//�c�I��
	SetSelectType(CSelectMenu::SELECT_TYPE::VERTICAL);
	//�I����UI�̏ڍאݒ�
	SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 400.0f - 60.0f, 0.0f), 280.0f, 60.0f, CTexture::TEXTURE_TYPE::TEXT_CONTINUE);
	SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 400.0f + 60.0f, 0.0f), 280.0f, 60.0f, CTexture::TEXTURE_TYPE::TEXT_TITLE);
	//�I�����A�C�R���̐���
	CreateSelectIcon(D3DXVECTOR3(-180.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
	SetIconPosOffset(1, D3DXVECTOR3(-120.0f, 0.0f, 0.0f));

	CSelectMenu2D::Init();

	return S_OK;
}

//=============================================================================
// �ь�I�����j���[�̏I������
//=============================================================================
void CPauseMenu::Uninit(void) {
	CSelectMenu2D::Uninit();

	//�|�[�Y�̃��x����������
	//SubUpdatePauseLevel();	//�|�[�Y���j���[�̑I���ɂ���ĕ��򂳂���悤�ɂ��邽�߃R�����g�A�E�g
}

//=============================================================================
// �ь�I�����j���[�̍X�V����
//=============================================================================
void CPauseMenu::Update(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	//���݂̓��̓f�o�C�X�̎擾
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//�t�F�[�h�̎擾
	CFade* pFade = pManager->GetFade();		//�t�F�[�h�ւ̃|�C���^
	if (pFade == nullptr) return;
	//�Q�[���V�[���̎擾
	CGameScene* pGameScene = pManager->GetGameScene();		//�t�F�[�h�ւ̃|�C���^
	if (pGameScene == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = pManager->GetSound();	//�T�E���h�ւ̃|�C���^
	if (pSound == nullptr) return;

	if (pInput != nullptr) {
		//�I�������
		if (pInput->GetTrigger(CInput::CODE::SELECT) && !GetLockChangeSelect()) {
			switch (GetIdxCurSelect())
			{
				//�Q�[���ĊJ
			case 0:
				//�Q�[���V�[���̃|�[�Y���j���[��j������
				pGameScene->DeletePauseMenu();
				//�|�[�Y�̃��x����������
				SubUpdatePauseLevel();
				//�T�E���h���Đ�
				pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
				return;
				break;

				//�^�C�g���ɖ߂�
			case 1:
				//�Q�[���V�[���̃|�[�Y���j���[��j������
				pGameScene->DeletePauseMenu();
				//�^�C�g���փV�[���J��
				pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.02f, 60);
				//�T�E���h���Đ�
				pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
				break;
			}
		}
	}

	CSelectMenu2D::Update();
}