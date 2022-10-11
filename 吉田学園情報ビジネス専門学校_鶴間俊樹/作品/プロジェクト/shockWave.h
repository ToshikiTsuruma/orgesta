//=============================================================================
//
// �Ռ��g���� [shockWave.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjectList;

//*****************************************************************************
// �Ռ��g�N���X
//*****************************************************************************
class CShockwave : public CMeshcylinder
{
public:
	CShockwave();	//�f�t�H���g�R���X�g���N�^
	CShockwave(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fHeight);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CShockwave();	//�f�X�g���N�^
	static CShockwave* Create(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��

private:
	void Rotate(void);	//��]����
	void AttackCollision(int nObjtype);	//�U���̓����蔻��

	int m_nLife;	//���ł���܂ł̃J�E���^�[
	float m_fAddRadius;		//�ǉ����锼�a
	float m_fAddHeight;		//�ǉ����鍂��
	float m_fRotateSpeed;	//��]���x
	CObjectList* m_pListAttacked;	//�U���ς݂̃I�u�W�F�N�g�̃��X�g
};

#endif // !_SHOCKWAVE_H_