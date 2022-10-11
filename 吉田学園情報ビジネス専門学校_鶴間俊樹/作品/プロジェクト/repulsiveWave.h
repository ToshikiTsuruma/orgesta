//=============================================================================
//
// �˗͔g���� [repulsiveWave.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _REPULSIVEWAVE_H_
#define _REPULSIVEWAVE_H_

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
// �˗͔g�N���X
//*****************************************************************************
class CRepulsivewave : public CMeshcylinder
{
public:
	CRepulsivewave();	//�f�t�H���g�R���X�g���N�^
	CRepulsivewave(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CRepulsivewave();	//�f�X�g���N�^
	static CRepulsivewave* Create(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside, float fAddRadius, int nLife, float fRotSpeed);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��

private:
	void Rotate(void);	//��]����
	void AttackCollision(int nObjtype);	//�U���̓����蔻��

	int m_nLife;	//���ł���܂ł̃J�E���^�[
	float m_fAddRadius;		//�ǉ����锼�a
	float m_fRotateSpeed;	//��]���x
	CObjectList* m_pListAttacked;	//�U���ς݂̃I�u�W�F�N�g�̃��X�g
};

#endif // !_REPULSIVEWAVE_H_