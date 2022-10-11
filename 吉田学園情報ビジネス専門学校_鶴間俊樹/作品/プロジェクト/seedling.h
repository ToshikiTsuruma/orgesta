//=============================================================================
//
// �c�؂̏��� [seedling.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SEEDLING_H_
#define _SEEDLING_H_

#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �c�؃N���X
//*****************************************************************************
class CSeedling : public CObjectModel
{
public:
	CSeedling();	//�f�t�H���g�R���X�g���N�^
	~CSeedling();	//�f�X�g���N�^

	static CSeedling* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//��������
	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void CreateScapegoat(void);	//���т𐶐�

	void SetNumNeedEnemy(int nNum) { m_nNumNeedEnemy = nNum; }	//���т𐶐����邽�߂ɕK�v�ȁA�|�����G�̐��̐ݒ�
	int GetNumNeedEnemy(void) { return m_nNumNeedEnemy; }	//���т𐶐����邽�߂ɕK�v�ȁA�|�����G�̐��̎擾

private:
	int m_nNumNeedEnemy;	//���т𐶐����邽�߂ɕK�v�ȁA�|�����G�̐�

};

#endif // !_SEEDLING_H_