//=============================================================================
//
// ���т̏��� [scapegoat.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCAPEGOAT_H_
#define _SCAPEGOAT_H_

#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CGauge3D;

//*****************************************************************************
// ���уN���X
//*****************************************************************************
class CScapegoat : public CObjectModel
{
public:
	CScapegoat();	//�f�t�H���g�R���X�g���N�^
	~CScapegoat();	//�f�X�g���N�^

	static CScapegoat* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//��������
	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��

	void Heal(int nHeal);		//�̗͂��񕜂���
	void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead);		//�_���[�W
	void Dead(void);		//���S������
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾

private:
	int m_nLife;	//�̗�
	CGauge3D* m_pGaugeLife;	//�̗̓Q�[�W
	int m_nCntAttack;	//	�U�������J�E���^
};

#endif // !_SCAPEGOAT_H_