//=============================================================================
//
// �I�u�W�F�N�g���� [object.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "object.h"
#include "objectList.h"

#include "manager.h"
#include "renderer.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CObject::m_nUpdatePauseLevel = 0;
CObject* CObject::m_pTopAll = nullptr;
CObject* CObject::m_pCurAll = nullptr;
CObject* CObject::m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};
CObject* CObject::m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObject::CObject()
{
	m_bDeath = false;
	m_objType = OBJTYPE_NONE;
	m_updatePriority = UPDATE_PRIORITY::DEFAULT;
	m_drawPriority = DRAW_PRIORITY::DEFAULT;
	m_texType = CTexture::TEXTURE_TYPE::NONE;
	m_bDraw = true;
	m_nPauseLevel = 0;

	//�S�I�u�W�F�N�g�̃��X�g�̏����ݒ�
	m_pPrevAll = m_pCurAll;	//�O�̃|�C���^�̐ݒ�
	m_pNextAll = nullptr;		//���̃|�C���^�̐ݒ�
	if (m_pTopAll == nullptr) m_pTopAll = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_pCurAll != nullptr) m_pCurAll->m_pNextAll = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_pCurAll = this;		//�Ō���̃|�C���^�̐ݒ�

	//�X�V���̃��X�g�̏����ݒ�
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextUpdate = nullptr;								//���̃|�C���^�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == nullptr) m_apTopUpdate[(int)m_updatePriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] != nullptr) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurUpdate[(int)m_updatePriority] = this;			//�Ō���̃|�C���^�̐ݒ�

	//�`�揇�̃��X�g�̏����ݒ�
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextDraw = nullptr;							//���̃|�C���^�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == nullptr) m_apTopDraw[(int)m_drawPriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] != nullptr) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurDraw[(int)m_drawPriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{
	//�S���X�g�̍X�V
	//�S�I�u�W�F�N�g���X�g
	if (m_pTopAll == this) m_pTopAll = m_pNextAll;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_pCurAll == this) m_pCurAll = m_pPrevAll;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevAll != nullptr) m_pPrevAll->m_pNextAll = m_pNextAll;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextAll != nullptr) m_pNextAll->m_pPrevAll = m_pPrevAll;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�X�V�����X�g
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevUpdate != nullptr) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextUpdate != nullptr) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�`�揇���X�g
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevDraw != nullptr) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextDraw != nullptr) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����
}

//=============================================================================
// �S�I�u�W�F�N�g�̃��X�g��Top���擾
//=============================================================================
CObject* CObject::GetObjectTopAll(void) {
	return m_pTopAll;
}

//=============================================================================
// �S�I�u�W�F�N�g�̃��X�g��Next���擾
//=============================================================================
CObject* CObject::GetObjectNextAll(CObject* pObject) {
	if (pObject != nullptr) {
		return pObject->m_pNextAll;
	}
	return nullptr;
}

//=============================================================================
// ���ׂẴI�u�W�F�N�g��ReleasePtr���s��
//=============================================================================
void CObject::ReleasePtrAll(CObject* pReleaseObj) {
	CObject* pObject = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObject != nullptr)
	{
		CObject* pObjectNext = pObject->m_pNextAll;

		//ReleasePtr���s��
		pObject->ReleasePtr(pReleaseObj);

		//���̃I�u�W�F�N�g����
		pObject = pObjectNext;
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̉������
//=============================================================================
//���j�������I�u�W�F�N�g��ʂ̃N���X�Ń|�C���^��ێ����Ă����ꍇ�G���[���N���邽�ߒ���
void CObject::ReleaseAll(void) {
	CObject* pObjectAll = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObjectAll != nullptr)
	{
		CObject* pObjectNext = pObjectAll->m_pNextAll;
		//�I���������s��
		if (!pObjectAll->m_bDeath) pObjectAll->Uninit();
		//���̃I�u�W�F�N�g����
		pObjectAll = pObjectNext;
	}
}

//=============================================================================
// �w�肵���^�C�v�̃I�u�W�F�N�g�̉������
//=============================================================================
//���j�������I�u�W�F�N�g��ʂ̃N���X�Ń|�C���^��ێ����Ă����ꍇ�G���[���N���邽�ߒ���
void CObject::ReleaseObjtype(int nObjtype) {
	CObject* pObject = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObject != nullptr)
	{
		//���̃I�u�W�F�N�g���擾
		CObject* pObjectNext = pObject->m_pNextAll;

		//�I�u�W�F�N�g�^�C�v����v���Ȃ��ꍇ
		if (!(pObject->m_objType & nObjtype)) {
			//���̃I�u�W�F�N�g����
			pObject = pObjectNext;
			//���[�v���΂�
			continue;
		}

		//�I���������s��
		pObject->Uninit();
		//���̃I�u�W�F�N�g����
		pObject = pObjectNext;
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObject::UpdateAll(void) {
	//�I�u�W�F�N�g�̍X�V
	for (int nCnt = 0; nCnt < (int)UPDATE_PRIORITY::ENUM_MAX; nCnt++) {
		CObject* pObjectUpdate = m_apTopUpdate[nCnt];	//�X�V����I�u�W�F�N�g

		while (pObjectUpdate != nullptr) {
			CObject* pObjectNext = pObjectUpdate->m_pNextUpdate;	//���̃I�u�W�F�N�g���擾

			//���݂̃|�[�Y���x�����Ⴂ�ꍇ�A�X�V�����Ɏ��̃I�u�W�F�N�g
			if (pObjectUpdate->m_nPauseLevel < m_nUpdatePauseLevel) {
				//���̃I�u�W�F�N�g����
				pObjectUpdate = pObjectNext;
				continue;
			}

			//���S�t���O�������Ă��Ȃ��ꍇ�X�V
			if (!pObjectUpdate->m_bDeath) pObjectUpdate->Update();	

			//���̃I�u�W�F�N�g����
			pObjectUpdate = pObjectNext;
		}
	}

	//���S�t���O���������I�u�W�F�N�g��j��
	CObject* pObjectDeath = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObjectDeath != nullptr)
	{
		CObject* pObjectNext = pObjectDeath->m_pNextAll;
		//���S���Ă����ꍇ�j��
		if (pObjectDeath->m_bDeath) {
			//�I�u�W�F�N�g���X�g�N���X�̃��X�g�̃m�[�h�ɔj�������I�u�W�F�N�g���܂܂�Ă���ꍇ�A���̃m�[�h��j������
			CObjectList::ReleaseDeleteNode(pObjectDeath);
			//���ׂẴI�u�W�F�N�g��ReleasePtr���s��
			ReleasePtrAll(pObjectDeath);
			//�I�u�W�F�N�g�̔j��
			delete pObjectDeath;	
		}

		pObjectDeath = pObjectNext;
	}
}

//=============================================================================
// �S�I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObject::DrawAll(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer= pManager->GetRenderer();
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	for (int nCnt = 0; nCnt < (int)DRAW_PRIORITY::ENUM_MAX; nCnt++) {
		//UI�̕`��ɐ؂�ւ�����ꍇ�AZ�o�b�t�@��1.0f�ŃN���A����
		if(nCnt == (int)DRAW_PRIORITY::UI_BG && pDevice != nullptr) pDevice->Clear(0, nullptr, (D3DCLEAR_ZBUFFER), D3DXCOLOR(), 1.0f, 0);

		CObject* pObject = m_apTopDraw[nCnt];
		while (pObject != nullptr) {
			CObject* pObjectNext = pObject->m_pNextDraw;	//���̃I�u�W�F�N�g�擾
			//�`��\���̏ꍇ�`��
			if(pObject->m_bDraw && !pObject->m_bDeath) pObject->Draw();
			//���̃I�u�W�F�N�g����
			pObject = pObjectNext;
		}
	}
}

//=============================================================================
// �w�肵���^�C�v�̎��S����
//=============================================================================
void CObject::DeadObjtype(int nObjtype) {
	CObject* pObject = m_pTopAll;	//�S�I�u�W�F�N�g�̃|�C���^��擪���珇�ɑ��
	while (pObject != nullptr)
	{
		CObject* pObjectNext = pObject->m_pNextAll;

		//�I�u�W�F�N�g�^�C�v����v���Ȃ��ꍇ
		if (!(pObject->m_objType & nObjtype)) {
			//���̃I�u�W�F�N�g����
			pObject = pObjectNext;
			//���[�v���΂�
			continue;
		}

		//���S�������s��
		if (!pObject->m_bDeath) pObject->Dead();
		//���̃I�u�W�F�N�g����
		pObject = pObjectNext;
	}
}

//=============================================================================
// ����ʒu����I�u�W�F�N�g�ւ̍ł��߂����������߂�
//=============================================================================
CObject* CObject::GetNearObject(const D3DXVECTOR3 pos, int nObjType, int nObjFlag, D3DXVECTOR3* pPosNearObj, float* pfDistNearObj, D3DXVECTOR3* pVecNearObj) {
	bool bSuccessGetPos = false;	//�ł��߂��I�u�W�F�N�g�����߂�ꂽ���ǂ���
	CObject* pObjNear = nullptr;	//�ł��߂��I�u�W�F�N�g

	//�ł��߂��I�u�W�F�N�g�̏��
	D3DXVECTOR3 posNearObj;	//�ł��߂��I�u�W�F�N�g�̈ʒu
	float fDistNearObj;		//�ł��߂��I�u�W�F�N�g�ւ̋���
	D3DXVECTOR3 vecNearObj;	//�ł��߂��I�u�W�F�N�g�ւ̐��K���x�N�g��

	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾

		//�I�u�W�F�N�g�^�C�v�̊m�F
		bool bMatchType = false;
		if (pObject->GetObjType() & nObjType) bMatchType = true;

		bool bDeath = pObject->GetDeath();

		//���̃��[�v�ɔ�΂�
		if (!bMatchType || bDeath) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;
		}

		//�����蔻�肪�L�����ǂ����̔���
		if (nObjFlag & OBJFLAG_ENABLECOLLISION) {
			//�L���łȂ��ꍇ���̃��[�v�ɔ�΂�
			if (!pObject->GetEnableCollision()) {
				pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
				continue;
			}
		}

		//---------------------------
		//�ʒu�擾���Ă��Ȃ��Ƃ��A�^�C�v�̍ŏ��̃I�u�W�F�N�g���ł��߂��I�u�W�F�N�g�Ƃ��ď���ݒ�
		//---------------------------
		if (!bSuccessGetPos) {
			bSuccessGetPos = true;	//�擾����
			//���̐ݒ�
			pObjNear = pObject;
			posNearObj = pObject->GetPos();	//�ʒu
			fDistNearObj = D3DXVec3Length(&D3DXVECTOR3(posNearObj - pos));	//����
			D3DXVec3Normalize(&vecNearObj, &D3DXVECTOR3(posNearObj - pos));	//���K���x�N�g��
		}

		//---------------------------
		//�ʒu�̎擾
		//---------------------------
		D3DXVECTOR3 posObject = pObject->GetPos();	//�I�u�W�F�N�g�̈ʒu

		D3DXVECTOR3 vecObject = D3DXVECTOR3(posObject - pos);	//����ʒu����I�u�W�F�N�g�ւ̃x�N�g��
		float fDistObj = D3DXVec3Length(&vecObject);	//����ʒu�ƃI�u�W�F�N�g�̋���
		//�����̍X�V
		if (fDistNearObj > fDistObj) {
			//�ł��߂��I�u�W�F�N�g�̏��
			pObjNear = pObject;
			posNearObj = posObject;		//�ʒu
			fDistNearObj = fDistObj;	//����
			D3DXVec3Normalize(&vecNearObj, &vecObject);	//���K���x�N�g��
		}

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}

	//�擾���s��
	if (!bSuccessGetPos) return nullptr;

	//�擾�������A���̐ݒ�
	if (pPosNearObj != nullptr) *pPosNearObj = posNearObj;
	if (pfDistNearObj != nullptr) *pfDistNearObj = fDistNearObj;
	if (pVecNearObj != nullptr) *pVecNearObj = vecNearObj;

	return pObjNear;
}

//=============================================================================
// �I�u�W�F�N�g�̉������
//=============================================================================
void CObject::Release(void) {
	m_bDeath = true;	//���S�t���O�𗧂Ă�
}

//=============================================================================
// ���S�t���O�̎擾
//=============================================================================
bool CObject::GetDeath(void) {
	return m_bDeath;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̐ݒ�
//=============================================================================
void CObject::SetObjType(OBJTYPE objType) {
	//�I�u�W�F�N�g�^�C�v�̃��X�g��ύX
	m_objType = objType;
}

//=============================================================================
// �I�u�W�F�N�g�̎�ނ̎擾
//=============================================================================
CObject::OBJTYPE CObject::GetObjType(void) {
	return m_objType;
}

//=============================================================================
// �X�V�̗D�揇�ʂ̐ݒ�
//=============================================================================
void CObject::SetUpdatePriority(UPDATE_PRIORITY priority) {
	//�ύX�O�̍X�V�D�揇�̃I�u�W�F�N�g�̃��X�g�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == this) m_apTopUpdate[(int)m_updatePriority] = m_pNextUpdate;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] == this) m_apCurUpdate[(int)m_updatePriority] = m_pPrevUpdate;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevUpdate != nullptr) m_pPrevUpdate->m_pNextUpdate = m_pNextUpdate;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextUpdate != nullptr) m_pNextUpdate->m_pPrevUpdate = m_pPrevUpdate;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�X�V�D�揇�̃��X�g��ύX
	m_updatePriority = priority;
	m_pPrevUpdate = m_apCurUpdate[(int)m_updatePriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextUpdate = nullptr;								//���̃|�C���^�̐ݒ�
	if (m_apTopUpdate[(int)m_updatePriority] == nullptr) m_apTopUpdate[(int)m_updatePriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurUpdate[(int)m_updatePriority] != nullptr) m_apCurUpdate[(int)m_updatePriority]->m_pNextUpdate = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurUpdate[(int)m_updatePriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �`��̗D�揇�ʂ̐ݒ�
//=============================================================================
void CObject::SetDrawPriority(DRAW_PRIORITY priority) {
	//�ύX�O�̕`��D�揇�̃I�u�W�F�N�g�̃��X�g�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == this) m_apTopDraw[(int)m_drawPriority] = m_pNextDraw;	//���̃I�u�W�F�N�g��top�������ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] == this) m_apCurDraw[(int)m_drawPriority] = m_pPrevDraw;	//���̃I�u�W�F�N�g��cur�������ꍇ�A�O�̃I�u�W�F�N�g��cur�ɂ���
	if (m_pPrevDraw != nullptr) m_pPrevDraw->m_pNextDraw = m_pNextDraw;	//�O�̃I�u�W�F�N�g��pNext�ɁA���̃I�u�W�F�N�g��pNext����
	if (m_pNextDraw != nullptr) m_pNextDraw->m_pPrevDraw = m_pPrevDraw;	//���̃I�u�W�F�N�g��pPrev�ɁA���̃I�u�W�F�N�g��pPrev����

	//�`��D�揇�̃��X�g��ύX
	m_drawPriority = priority;	//�`��D�揇�̐ݒ�
	m_pPrevDraw = m_apCurDraw[(int)m_drawPriority];	//�O�̃|�C���^�̐ݒ�
	m_pNextDraw = nullptr;							//���̃|�C���^�̐ݒ�
	if (m_apTopDraw[(int)m_drawPriority] == nullptr) m_apTopDraw[(int)m_drawPriority] = this;				//top�����݂��Ȃ��ꍇ�A���̃I�u�W�F�N�g��top�ɂ���
	if (m_apCurDraw[(int)m_drawPriority] != nullptr) m_apCurDraw[(int)m_drawPriority]->m_pNextDraw = this;	//cur�����݂���ꍇ�Acur��pNext�����̃I�u�W�F�N�g�̃|�C���^�ɂ���
	m_apCurDraw[(int)m_drawPriority] = this;			//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CObject::SetTexType(CTexture::TEXTURE_TYPE type) {
	m_texType = type;
}

//=============================================================================
// �e�N�X�`���̎�ނ̎擾
//=============================================================================
CTexture::TEXTURE_TYPE CObject::GetTexType(void) {
	return m_texType;
}

//=============================================================================
// �����蔻��̗L����Ԃ�ݒ�
//=============================================================================
void CObject::SetEnableCollision(bool bEnable) {
	m_bEnableCollision = bEnable;
}

//=============================================================================
// �����蔻��̗L����Ԃ��擾
//=============================================================================
bool CObject::GetEnableCollision(void) {
	return m_bEnableCollision;
}