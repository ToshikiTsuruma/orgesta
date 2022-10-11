//=============================================================================
//
// 2D�I�u�W�F�N�g���� [object2D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObject2D::CObject2D()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fRatioWidth = 1.0f;
	m_fHeight = 0.0f;
	m_fRatioHeight = 1.0f;
	m_fAngle = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_bUseZBuffTexture = false;

	SetDrawPriority(DRAW_PRIORITY::UI);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject2D::~CObject2D()
{

}

//=============================================================================
// 2D�I�u�W�F�N�g�̏����ݒ�Ȃ��̐�������
//=============================================================================
CObject2D* CObject2D::Create(void) {
	CObject2D* pObject2D;
	pObject2D = new CObject2D;
	if (pObject2D == nullptr) return nullptr;

	pObject2D->Init();

	return pObject2D;
}

//=============================================================================
// 2D�I�u�W�F�N�g�̐�������
//=============================================================================
CObject2D* CObject2D::Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight) {
	CObject2D* pObject2D;
	pObject2D = new CObject2D;
	if (pObject2D == nullptr) return nullptr;

	pObject2D->m_pos = pos;
	pObject2D->m_fWidth = fWidth;
	pObject2D->m_fHeight = fHeight;
	pObject2D->SetTexType(type);
	pObject2D->Init();

	return pObject2D;
}

//=============================================================================
// 2D�I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CObject2D::Init(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�𐶐�����O�ɔj��
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//���_�o�b�t�@�̐���
	if (pDevice != nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	VERTEX_2D *pVtx;	//���_�o�b�t�@
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 2D�I�u�W�F�N�g�̏I������
//=============================================================================
void CObject2D::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
   	Release();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObject2D::Update(void) {

	//���_�o�b�t�@�̍X�V
	//SetVtxPos();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObject2D::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;	//null�̏ꍇ�I��
	//�����_���[�̎擾
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;	//null�̏ꍇ�I��
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;		//null�̏ꍇ�I��

	//���ɉe���͂Ȃ����������O�̂���Z�l�e�N�X�`���ɂ͏������܂Ȃ�
	if (pRenderer->GetDrawZTex()) return;

	//���_�o�b�t�@��null�̏ꍇ�I��
	if (m_pVtxBuff == nullptr) return;

	//�e�N�X�`���̎擾
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());
	//Z�o�b�t�@�̃e�N�X�`�����g�p����ꍇ�擾
	if (m_bUseZBuffTexture) pTexture = pRenderer->GetZBuffTex();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�	

	//�A���t�@�e�X�g�@�L���^����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//�A���t�@�l�̎Q�ƒl
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//Z�o�b�t�@�̍X�V�𖳌��@�`�揇�����3D�I�u�W�F�N�g��2D�I�u�W�F�N�g�̑O�ʂɏo����悤�ɂ��邽��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//���_��`��ݒ�
	pRenderer->SetVtxDecl2D();
	//�e�N�X�`���̐ݒ�
	pRenderer->SetEffectTexture(pTexture);

	DWORD dwPassFlag = PASS_2D;	//�J�n����p�X�̃t���O
	//�e�N�X�`��������ꍇ�t���O��ǉ�
	if (pTexture != nullptr) dwPassFlag |= PASS_TEXTURE;

	pRenderer->BeginPassEffect(dwPassFlag);	//�p�X�̊J�n
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0,
		2);

	pRenderer->EndPassEffect();	//�p�X�̏I��

	//Z�o�b�t�@�̍X�V��L��
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//�A���t�@�e�X�g�@�L���^����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//�A���t�@�e�X�g
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//�A���t�@�l�̎Q�ƒl
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//=============================================================================
// 2D�I�u�W�F�N�g�̈ʒu���̐ݒ�
//=============================================================================
void CObject2D::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
	//���_�o�b�t�@�̍X�V
	SetVtxPos();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̈ʒu���̎擾
//=============================================================================
D3DXVECTOR3 CObject2D::GetPos(void) { return m_pos; }

//=============================================================================
// 2D�I�u�W�F�N�g�̈ړ��ʂ̐ݒ�
//=============================================================================
void CObject2D::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// 2D�I�u�W�F�N�g�̈ړ��ʂ̎擾
//=============================================================================
D3DXVECTOR3 CObject2D::GetMove(void) { return m_move; }

//=============================================================================
// 2D�I�u�W�F�N�g�̃T�C�Y�̐ݒ�
//=============================================================================
void CObject2D::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;
	//���_�o�b�t�@�̍X�V
	SetVtxPos();
}

//=============================================================================
// 2D�I�u�W�F�N�g�̃T�C�Y�̎擾
//=============================================================================
D3DXVECTOR3 CObject2D::GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f); }

//=============================================================================
// 2D�I�u�W�F�N�g�̕��̊����̐ݒ�
//=============================================================================
void CObject2D::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 2D�I�u�W�F�N�g�̕��̊����̐ݒ�
//=============================================================================
void CObject2D::SetRatioHeight(float fRatio) {
	m_fRatioHeight = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �p�x�̐ݒ�
//=============================================================================
void CObject2D::SetAngle(float fAngle) {
	m_fAngle = fAngle;
	//���_�o�b�t�@�̍X�V
	SetVtxPos();
}

//=============================================================================
// �p�x�̎擾
//=============================================================================
float CObject2D::GetAngle(void) { return m_fAngle; }

//=============================================================================
// �e���_�̈ʒu���X�V
//=============================================================================
void CObject2D::SetVtxPos(void) {
	if (m_pVtxBuff != nullptr) {
		float fRadius = sqrtf(powf(m_fWidth / 2.0f, 2.0f) + powf(m_fHeight / 2.0f, 2.0f));	//�|���S���̑Ίp���̔���
		float fAngleDelta = atan2f(m_fWidth, m_fHeight);	//��̊p�x����̍����i���W�A���j

		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �J���[�̐ݒ�
//=============================================================================
void CObject2D::SetColor(D3DXCOLOR col) {
	m_col = col;

	//���_�o�b�t�@�̍X�V
	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//���_�o�b�t�@�̃��b�N
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// ���_����ݒ�
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �J���[�̎擾
//=============================================================================
D3DXCOLOR CObject2D::GetColor(void) { return m_col; }

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void CObject2D::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2(startU, startV);
	pVtx[1].tex = D3DXVECTOR2(endU, startV);
	pVtx[2].tex = D3DXVECTOR2(startU, endV);
	pVtx[3].tex = D3DXVECTOR2(endU, endV);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �����ɑΉ������e�N�X�`�����W�̐ݒ�
//=============================================================================
void CObject2D::SetTexNumber(int nNumber) {
	VERTEX_2D *pVtx;
	//���_�o�b�t�@�̃��b�N
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2(nNumber / 10.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nNumber + 1) / 10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber / 10.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nNumber + 1) / 10.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}