//=============================================================================
//
// 3D�I�u�W�F�N�g���� [object3D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "object3D.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObject3D::CObject3D()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_nNumVtx = 0;
	m_nNumIdx = 0;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject3D::~CObject3D()
{

}

//=============================================================================
// 3D�I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CObject3D::Init(void) {

	return S_OK;
}

//=============================================================================
// 3D�I�u�W�F�N�g�̏I������
//=============================================================================
void CObject3D::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != nullptr) {
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// 3D�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObject3D::Update(void) {

}

//=============================================================================
// 3D�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObject3D::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;	//null�̏ꍇ�I��
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;	//null�̏ꍇ�I��
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;		//null�̏ꍇ�I��

	//�e�N�X�`���̎擾
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());

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

	//�V�F�[�_�Ƀ}�g���b�N�X��ݒ�
	pRenderer->SetEffectMatrixWorld(m_mtxWorld);

	//�V�F�[�_�Ƀe�N�X�`����ݒ�
	pRenderer->SetEffectTexture(pTexture);

	//�A���t�@�e�X�g�@�L���^����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//�A���t�@�l�̎Q�ƒl
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);
	//���_��`��ݒ�
	pRenderer->SetVtxDecl3D();	

	//���C�g�̏�Ԏ擾
	DWORD dwEnableLight = 0;
	pDevice->GetRenderState(D3DRS_LIGHTING, &dwEnableLight);

	DWORD dwPassFlag = PASS_3D;

	//�e�N�X�`��������ꍇ�t���O��ǉ�
	if (pTexture != nullptr) dwPassFlag |= PASS_TEXTURE;	
	//���C�g������ꍇ�t���O��ǉ�
	if (dwEnableLight) dwPassFlag |= PASS_LIGHT;	

	//���f�����ݒ肵���}�e���A���̉e�����󂯂Ȃ��悤�Ƀ}�e���A���̐ݒ�
	pRenderer->SetEffectMaterialDiffuse(m_col);
	pRenderer->SetEffectMaterialEmissive(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	pRenderer->SetEffectMaterialSpecular(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pRenderer->SetEffectMaterialPower(2.0f);

	//�p�X�̊J�n
	pRenderer->BeginPassEffect(dwPassFlag);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumVtx,	//�g�p���钸�_��
		0,	//�����̒l���ŏ��̃C���f�b�N�X
		m_nNumIdx - 2);	//�O�p�`�̐�

	pRenderer->EndPassEffect();//�G�t�F�N�g�I��
	
	//�A���t�@�e�X�g�@�L���^����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//�A���t�@�l�̎Q�ƒl
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

}

//=============================================================================
// �ʒu���W�̐ݒ�
//=============================================================================
void CObject3D::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// �ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CObject3D::GetPos(void) {
	return m_pos;
}

//=============================================================================
// �p�x�̐ݒ�
//=============================================================================
void CObject3D::SetRot(D3DXVECTOR3 rot) { m_rot = rot; }

//=============================================================================
// �p�x�̎擾
//=============================================================================
D3DXVECTOR3 CObject3D::GetRot(void) { return m_rot; }

//=============================================================================
// ���_�o�b�t�@�̃|�C���^�̃|�C���^���擾
//=============================================================================
LPDIRECT3DVERTEXBUFFER9* CObject3D::GetPtrVtxBuff(void) {
	return &m_pVtxBuff;
}

//=============================================================================
// �C���f�b�N�X�o�b�t�@�̃|�C���^�̃|�C���^���擾
//=============================================================================
LPDIRECT3DINDEXBUFFER9* CObject3D::GetPtrIdxBuff(void) {
	return &m_pIdxBuff;
}

//=============================================================================
//���_���̐ݒ�
//=============================================================================
void CObject3D::SetNumVtx(int nNumVtx) {
	m_nNumVtx = nNumVtx;
}

//=============================================================================
//���_���̎擾
//=============================================================================
int CObject3D::GetNumVtx(void) {
	return m_nNumVtx;
}

//=============================================================================
//�C���f�b�N�X���̐ݒ�
//=============================================================================
void CObject3D::SetNumIdx(int nNumIdx) {
	m_nNumIdx = nNumIdx;
}

//=============================================================================
//�C���f�b�N�X���̎擾
//=============================================================================
int CObject3D::GetNumIdx(void) {
	return m_nNumIdx;
}