//=============================================================================
//
// UI���f���I�u�W�F�N�g���� [objectModelUI.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "objectModelUI.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "camera.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjectModelUI::CObjectModelUI()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CObjectModelUI::CObjectModelUI(CModel::MODELTYPE typeModel, bool bOutLine) : CObjectModel(typeModel, bOutLine)
{
	//�`�揇�̐ݒ�
	SetDrawPriority(DRAW_PRIORITY::UI);

	D3DXMatrixIdentity(&m_mtxView);
	m_rotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectModelUI::~CObjectModelUI()
{

}

//=============================================================================
// UI���f���I�u�W�F�N�g�̐�������
//=============================================================================
CObjectModelUI* CObjectModelUI::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) {
	CObjectModelUI* pObjectModelUI;
	pObjectModelUI = new CObjectModelUI(type, bOutLine);
	if (pObjectModelUI == nullptr) return nullptr;

	pObjectModelUI->SetPos(pos);
	pObjectModelUI->SetRot(rot);
	pObjectModelUI->Init();

	return pObjectModelUI;
}

//=============================================================================
// UI���f���I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CObjectModelUI::Init(void) {
	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// UI���f���I�u�W�F�N�g�̏I������
//=============================================================================
void CObjectModelUI::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// UI���f���I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObjectModelUI::Update(void) {
	//�p�x���擾
	D3DXVECTOR3 rot = GetRot();
	//��]������
	rot += m_rotSpeed;

	//�Β��ߎ��̒���
	if (rot.x > D3DX_PI) {
		rot.x -= D3DX_PI * 2;
	}
	if (rot.x < 0.0f) {
		rot.x += D3DX_PI * 2;
	}
	if (rot.y > D3DX_PI) {
		rot.y -= D3DX_PI * 2;
	}
	if (rot.y < 0.0f) {
		rot.y += D3DX_PI * 2;
	}
	if (rot.z > D3DX_PI) {
		rot.z -= D3DX_PI * 2;
	}
	if (rot.z < 0.0f) {
		rot.z += D3DX_PI * 2;
	}

	//�p�x��ݒ�
	SetRot(rot);

	CObjectModel::Update();
}

//=============================================================================
// UI���f���I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObjectModelUI::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;
	//�J�����̎擾
	CCamera* pCamera = pManager->GetCamera();
	if (pCamera == nullptr) return;

	//Z�e�N�X�`���ɏ������܂Ȃ�
	if (pRenderer->GetDrawZTex()) return;

	D3DXMATRIX mtxViewDef;	//�ʏ�̃r���[�}�g���b�N�X
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);	//�ʏ�̃r���[�}�g���b�N�X���擾
	D3DXVECTOR4 posVDef = pCamera->GetPosV();	//�ʏ�̃J�������_���擾

	//�t�H�O����
	pRenderer->SetEffectFogEnable(false);
	//�r���[�}�g���b�N�X�̕ύX
	pRenderer->SetEffectMatrixView(m_mtxView);
	//���_��ύX
	pRenderer->SetEffectPosView((D3DXVECTOR4)m_posV);

	//�`��
	CObjectModel::Draw();

	//�t�H�O�L��
	pRenderer->SetEffectFogEnable(true);
	//�r���[�}�g���b�N�X��߂�
	pRenderer->SetEffectMatrixView(mtxViewDef);
	//���_�̐ݒ��߂�
	pRenderer->SetEffectPosView(posVDef);
}