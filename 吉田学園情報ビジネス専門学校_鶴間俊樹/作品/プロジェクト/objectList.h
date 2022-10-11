//=============================================================================
//
// �I�u�W�F�N�g���X�g���� [objectList.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_LIST_H_
#define _OBJECT_LIST_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �I�u�W�F�N�g���X�g�N���X
//*****************************************************************************
class CObjectList
{
public:
	CObjectList();	//�f�t�H���g�R���X�g���N�^
	~CObjectList();	//�f�X�g���N�^
	void Init(void);	//����������
	void AppendNode(CObject* pObject);				//�m�[�h�̒ǉ�
	bool DeleteNode(int nIdx, CObject** ppObject);	//�m�[�h�̍폜
	bool MatchObject(CObject* pObject);	//��v����I�u�W�F�N�g�����邩�ǂ���
	CObject* GetObjectIndex(int nIdx);	//���X�g�̏ォ��n�Ԗڂ̃I�u�W�F�N�g�̎擾
	int GetNumNode(void);		//���X�g�ɂ���m�[�h�̐��̎擾
	void DeleteAllObject(void);	//���X�g�̂��ׂẴI�u�W�F�N�g���폜����
	static void ReleaseDeleteNode(CObject* pDeleteObj);	//�S���X�g�̃m�[�h�̃I�u�W�F�N�g�̃|�C���^�ƈ����̃I�u�W�F�N�g�̃|�C���^����v����ꍇ�m�[�h��j��

private:
	//���X�g�̃m�[�h
	struct ListNode
	{
		ListNode* pPrevNode;	//�O�̃m�[�h
		ListNode* pNextNode;	//���̃m�[�h

		CObject* pObject;		//�I�u�W�F�N�g�̃|�C���^
	};

	void ReleaseNode(ListNode* pReleaseNode);	//�m�[�h�̔j��

	ListNode* m_pTopNode;	//�擪�̃m�[�h
	ListNode* m_pCurNode;	//�Ō���̃m�[�h
	int m_nNumNode;			//���X�g�ɂ���m�[�h�̐�

	//�S�I�u�W�F�N�g���X�g�̃��X�g
	static CObjectList* m_pTopAllList;	//�擪�̃��X�g�̃|�C���^
	static CObjectList* m_pCurAllList;	//�Ō���̃��X�g�̃|�C���^
	CObjectList* m_pPrevAllList;		//�O�̃��X�g�̃|�C���^
	CObjectList* m_pNextAllList;		//���̃��X�g�̃|�C���^
};

#endif // !_OBJECT_LIST_H_