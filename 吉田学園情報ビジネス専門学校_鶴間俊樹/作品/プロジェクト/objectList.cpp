//=============================================================================
//
// �I�u�W�F�N�g���X�g���� [objectList.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "objectList.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
CObjectList* CObjectList::m_pTopAllList = nullptr;
CObjectList* CObjectList::m_pCurAllList = nullptr;

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjectList::CObjectList()
{
	m_pTopNode = nullptr;
	m_pCurNode = nullptr;
	m_nNumNode = 0;

	//�S�I�u�W�F�N�g���X�g�̃��X�g�̏����ݒ�
	m_pPrevAllList = m_pCurAllList;	//�O�̃|�C���^�̐ݒ�
	m_pNextAllList = nullptr;		//���̃|�C���^�̐ݒ�
	if (m_pTopAllList == nullptr) m_pTopAllList = this;				//top�����݂��Ȃ��ꍇ�A���̃��X�g��top�ɂ���
	if (m_pCurAllList != nullptr) m_pCurAllList->m_pNextAllList = this;	//cur�����݂���ꍇ�Acur��pNext�����̃��X�g�̃|�C���^�ɂ���
	m_pCurAllList = this;		//�Ō���̃|�C���^�̐ݒ�
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectList::~CObjectList()
{
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	while (pListNode != nullptr)
	{
		//���̃m�[�h��ۑ�
		ListNode* pNextNode = pListNode->pNextNode;
		//�m�[�h�̔j��
		delete pListNode;
		//���̃m�[�h��ݒ�
		pListNode = pNextNode;
	}

	//�S�I�u�W�F�N�g���X�g
	if (m_pTopAllList == this) m_pTopAllList = m_pNextAllList;	//���̃��X�g��top�������ꍇ�A���̃��X�g��top�ɂ���
	if (m_pCurAllList == this) m_pCurAllList = m_pPrevAllList;	//����v��cur�������ꍇ�A�O�̃��X�g��cur�ɂ���
	if (m_pPrevAllList != nullptr) m_pPrevAllList->m_pNextAllList = m_pNextAllList;	//�O�̃��X�g��pNext�ɁA���̃��X�g��pNext����
	if (m_pNextAllList != nullptr) m_pNextAllList->m_pPrevAllList = m_pPrevAllList;	//���̃��X�g��pPrev�ɁA���̃��X�g��pPrev����
}

//=============================================================================
// �I�u�W�F�N�g���X�g�̏���������
//=============================================================================
void CObjectList::Init(void) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	while (pListNode != nullptr)
	{
		//���̃m�[�h��ۑ�
		ListNode* pNextNode = pListNode->pNextNode;
		//�m�[�h�̔j��
		delete pListNode;
		//���̃m�[�h��ݒ�
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
	m_pCurNode = nullptr;

	m_nNumNode = 0;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�Ƀm�[�h��ǉ�
//=============================================================================
void CObjectList::AppendNode(CObject* pObject) {
	ListNode* pNewNode = new ListNode;	//�V�����ǉ�����m�[�h
	//�m�[�h�𐶐��ł��Ă��Ȃ��ꍇ�I��
	if (pNewNode == nullptr) return;

	//���X�g�̐ݒ�
	pNewNode->pPrevNode = m_pCurNode;	//�O�̃m�[�h�̐ݒ�
	pNewNode->pNextNode = nullptr;		//���̃m�[�h�̐ݒ�
	if (m_pTopNode == nullptr) m_pTopNode = pNewNode;				//�擪�̃m�[�h�����݂��Ȃ��ꍇ�A���̃m�[�h��擪�ɂ���
	if (m_pCurNode != nullptr) m_pCurNode->pNextNode = pNewNode;	//�Ō���̃m�[�h�����݂���ꍇ�A�Ō���̎������̃m�[�h�ɂ���
	m_pCurNode = pNewNode;		//�Ō���̃m�[�h�̐ݒ�

	//�m�[�h�ɕۑ�����I�u�W�F�N�g��ݒ�
	pNewNode->pObject = pObject;	
	//�m�[�h�̐������Z
	m_nNumNode++;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�̃m�[�h�̍폜
//=============================================================================
bool CObjectList::DeleteNode(int nIdx, CObject** ppObject) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾
	int nCntLoop = 0;	//���[�v������

	//�m�[�h��null�ł͂Ȃ��ꍇ
	while (pListNode != nullptr)
	{
		//�폜����ԍ��ɒB�����ꍇ
		if (nIdx == nCntLoop) {
			//�����̃I�u�W�F�N�g�̃|�C���^�Ƀm�[�h�̃I�u�W�F�N�g�̃|�C���^��n��
			if (ppObject != nullptr) {
				*ppObject = pListNode->pObject;
			}

			//�m�[�h�̔j��
			ReleaseNode(pListNode);

			//�m�[�h�̐������Z
			m_nNumNode--;
			return true;
		}
		//���̃m�[�h�ɂ���
		pListNode = pListNode->pNextNode;

		nCntLoop++;
	}

	return false;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�Ɉ�v����I�u�W�F�N�g�����邩�ǂ���
//=============================================================================
bool CObjectList::MatchObject(CObject* pObject) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	//�m�[�h��null�ł͂Ȃ��ꍇ
	while (pListNode != nullptr)
	{
		//�I�u�W�F�N�g���X�g�Ɉ�v����I�u�W�F�N�g������ꍇ
		if (pListNode->pObject == pObject) {
			return true;
		}
		//���̃m�[�h�ɂ���
		pListNode = pListNode->pNextNode;
	}

	return false;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�̏ォ��n�Ԗڂ̃I�u�W�F�N�g���擾
//=============================================================================
CObject* CObjectList::GetObjectIndex(int nIdx) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾
	//�擪�m�[�h��null�̏ꍇ�I��
	if (pListNode == nullptr) return nullptr;

	for (int nCnt = 0; nCnt < nIdx; nCnt++)
	{
		//���̃m�[�h�ɂ���
		pListNode = pListNode->pNextNode;
		//�m�[�h��null�̏ꍇ�I��
		if (pListNode == nullptr) return nullptr;
	}

	return pListNode->pObject;
}

//=============================================================================
// ���X�g�ɂ���m�[�h�̐��̎擾
//=============================================================================
int CObjectList::GetNumNode(void) {
	return m_nNumNode;
}

//=============================================================================
// ���X�g�̂��ׂẴI�u�W�F�N�g���폜����
//=============================================================================
void CObjectList::DeleteAllObject(void) {
	ListNode* pListNode = m_pTopNode;	//�擪�̃m�[�h���擾

	while (pListNode != nullptr)
	{
		//���̃m�[�h��ۑ�
		ListNode* pNextNode = pListNode->pNextNode;

		//�I�u�W�F�N�g�̏I������
		if (pListNode->pObject != nullptr) {
			pListNode->pObject->Uninit();
			pListNode->pObject = nullptr;
		}

		//�m�[�h�̔j��
		delete pListNode;
		//���̃m�[�h��ݒ�
		pListNode = pNextNode;
	}

	m_pTopNode = nullptr;
	m_pCurNode = nullptr;

	m_nNumNode = 0;
}

//=============================================================================
// �S���X�g�̃m�[�h�̃I�u�W�F�N�g�̃|�C���^�ƈ����̃I�u�W�F�N�g�̃|�C���^����v����ꍇ�m�[�h��j��
//=============================================================================
//�����얢�`�F�b�N-----�����̓���m�F�ł͖��Ȃ�---------------------------------------------------------------------------------------��
void CObjectList::ReleaseDeleteNode(CObject* pDeleteObj) {
	if (m_pTopAllList == nullptr || pDeleteObj == nullptr) return;

	CObjectList* pObjList = m_pTopAllList;	//�I�u�W�F�N�g���X�g�̃|�C���^

	while (pObjList != nullptr)
	{
		ListNode* pNode = pObjList->m_pTopNode;	//�m�[�h�̃|�C���^
		while (pNode != nullptr)
		{
			ListNode* pNextNode = pNode->pNextNode;	//���̃m�[�h�̃|�C���^
			CObject* pObject = pNode->pObject;	//�I�u�W�F�N�g�ւ̃|�C���^

			//�m�[�h���ێ����Ă���I�u�W�F�N�g���폜����I�u�W�F�N�g�������ꍇ
			if (pObject == pDeleteObj) {
				pObjList->ReleaseNode(pNode);
			}

			//���̃m�[�h����
			pNode = pNextNode;
		}

		//���̃��X�g����
		pObjList = pObjList->m_pNextAllList;
	}
}

//=============================================================================
// �m�[�h�̔j��
//=============================================================================
void CObjectList::ReleaseNode(ListNode* pReleaseNode) {
	//���X�g�̍X�V
	if (m_pTopNode == pReleaseNode) m_pTopNode = pReleaseNode->pNextNode;	//���̃m�[�h���擪�������ꍇ�A���̃m�[�h��擪�ɂ���
	if (m_pCurNode == pReleaseNode) m_pCurNode = pReleaseNode->pPrevNode;	//���̃m�[�h���Ō���������ꍇ�A�O�̃m�[�h���Ō���ɂ���
	if (pReleaseNode->pPrevNode != nullptr) pReleaseNode->pPrevNode->pNextNode = pReleaseNode->pNextNode;	//�O�̃m�[�h��pNext�ɁA���̃m�[�h��pNext����
	if (pReleaseNode->pNextNode != nullptr) pReleaseNode->pNextNode->pPrevNode = pReleaseNode->pPrevNode;	//���̃m�[�h��pPrev�ɁA���̃m�[�h��pPrev����

	//�m�[�h���폜
	delete pReleaseNode;
}