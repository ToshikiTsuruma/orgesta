//=============================================================================
//
// ���f���I�u�W�F�N�g���� [objectModel.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "objectModel.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjectModel::CObjectModel()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CObjectModel::CObjectModel(CModel::MODELTYPE typeModel, bool bOutLine)
{
	m_pModel = CModel::Create(typeModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nullptr, bOutLine);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectModel::~CObjectModel()
{

}

//=============================================================================
// ���f���I�u�W�F�N�g�̐�������
//=============================================================================
CObjectModel* CObjectModel::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) {
	CObjectModel* pObjectModel;
	pObjectModel = new CObjectModel(type, bOutLine);
	if (pObjectModel == nullptr) return nullptr;

	pObjectModel->Init();
	pObjectModel->SetPos(pos);
	pObjectModel->SetRot(rot);

	return pObjectModel;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CObjectModel::Init(void) {

	return S_OK;
}

//=============================================================================
// ���f���I�u�W�F�N�g�̏I������
//=============================================================================
void CObjectModel::Uninit(void) {
	if (m_pModel != nullptr) {
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// ���f���I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObjectModel::Update(void) {
	//���f���̍X�V
	if (m_pModel != nullptr) {
		m_pModel->Update();
	}

	//�ړ�����
	Move();
	//��]����
	Rotate();
}

//=============================================================================
// ���f���I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObjectModel::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	if (pManager == nullptr) return;
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-----------------------------------
	//���f���̕`��
	//-----------------------------------
	if (m_pModel != nullptr) m_pModel->Draw();
}

//=============================================================================
// �ړ�
//=============================================================================
void CObjectModel::Move(void) {
	//�ʒu�̈ړ�
	m_pos += m_move;
}

//=============================================================================
// ��]
//=============================================================================
void CObjectModel::Rotate(void) {
	m_rot += m_rotSpeed;

	if (m_rot.x > D3DX_PI) {
		m_rot.x += -D3DX_PI * 2;
	}
	if (m_rot.x < -D3DX_PI) {
		m_rot.x += D3DX_PI * 2;
	}
	if (m_rot.y < -D3DX_PI) {
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI) {
		m_rot.y += -D3DX_PI * 2;
	}
	if (m_rot.z < -D3DX_PI) {
		m_rot.z += D3DX_PI * 2;
	}
	if (m_rot.z > D3DX_PI) {
		m_rot.z += -D3DX_PI * 2;
	}
}

//=============================================================================
// ���f���̎�ނ̎擾
//=============================================================================
CModel::MODELTYPE CObjectModel::GetModelType(void) {
	if (m_pModel != nullptr) return m_pModel->GetModelType();
	return (CModel::MODELTYPE) NULL;
}

//=============================================================================
// ���f���̐F�̐ݒ�
//=============================================================================
void CObjectModel::SetModelColor(D3DXCOLOR col, int nIdx) {
	if (m_pModel == nullptr) return;
	m_pModel->SetMaterialDiffuse(col, nIdx);
}

//=============================================================================
// ���f���̔����F�̐ݒ�
//=============================================================================
void CObjectModel::SetModelGlowColor(D3DXCOLOR col) {
	if (m_pModel == nullptr) return;
	m_pModel->SetColorGlow(col);
}