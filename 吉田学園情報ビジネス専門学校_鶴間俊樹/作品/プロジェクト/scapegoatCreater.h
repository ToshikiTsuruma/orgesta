//=============================================================================
//
// ���ѐ������� [scapegoatCreater.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCAPEGOAT_CREATER_H_
#define _SCAPEGOAT_CREATER_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CScore;
class CBillboard;
class CObject2D;

//*****************************************************************************
// ���ѐ����N���X
//*****************************************************************************
class CScapegoatCreater : public CObject
{
public:
	CScapegoatCreater();	//�f�t�H���g�R���X�g���N�^
	~CScapegoatCreater();	//�f�X�g���N�^

	static CScapegoatCreater* Create(void);	//��������
	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void) {}		//�`�揈��

	void AddNumKillEnemy(int nNum);	//�|�����G�̐��𑝂₷

private:
	int m_nNumKillEnemy;	//�|�����G�̐�
	CScore* m_pNumKillEnemyUI;	//�|�����G�̐���UI
	CScore* m_pNumUseEnemyUI;	//�g�p����G�̐���UI
	CBillboard* m_pInteractUI;	//���ѐ������̃C���^���N�g�{�^���̃r���{�[�h
	CBillboard* m_pNixSignUI;	//�֎~�}�[�N�̃r���{�[�h
	CObject2D* m_pTextEnergy;	//�G�l���M�[�̃e�L�X�g

};

#endif // !_SCAPEGOAT_CREATER_H_