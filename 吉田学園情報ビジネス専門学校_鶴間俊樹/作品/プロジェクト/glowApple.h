//=============================================================================
//
// �O���[�A�b�v������ [glowApple.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _GLOW_APPLE_H_
#define _GLOW_APPLE_H_

#include "main.h"
#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define GLOW_APPLE_EMISSIVE (D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f))

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CAppleTree;
class CParticle;

//*****************************************************************************
// �O���[�A�b�v���N���X
//*****************************************************************************
class CGlowApple : public CObjectModel
{
public:
	enum class APPLE_TYPE {
		RED = 0,
		GREEN,
		WHITE,
		BLACK,
		SILVER,
		GOLD,
		ENUM_MAX
	};

	CGlowApple();		//�f�t�H���g�R���X�g���N�^
	CGlowApple(CAppleTree* pTree);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CGlowApple();		//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	void Dead(void);		//���S������
	CAppleTree* GetAppleTree(void) { return m_pAppleTree; }	//���̗ь炪�Ȃ�ь�̖؂̎擾
	void CreateGlowParticle(APPLE_TYPE type);	//�����\���p�p�[�e�B�N���̐���

	static D3DXCOLOR GetAppleColor(APPLE_TYPE type);	//��ނ��Ƃ̗ь�̐F���擾
	static void GetAppleText(char* pStr, APPLE_TYPE type);		//��ނ��Ƃ̗ь�̐����e�L�X�g

private:
	CAppleTree* m_pAppleTree;	//���̗ь炪�Ȃ�ь�̖�
	CParticle* m_pGlowParticle;	//�����\���̂��߂̃p�[�e�B�N��
};

#endif // !_GLOW_APPLE_H_