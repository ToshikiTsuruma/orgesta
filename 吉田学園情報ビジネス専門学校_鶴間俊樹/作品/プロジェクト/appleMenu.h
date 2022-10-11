//=============================================================================
//
// �ь�I�����j���[���� [appleMenu.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _APPLE_MENU_H_
#define _APPLE_MENU_H_

#include "selectMenu3D.h"
#include "glowApple.h"
#include "appleTree.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_SELECT_APPLE_MENU (3)	//�I������ь�̐�

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;
class CFont;

//*****************************************************************************
// �ь�I�����j���[�N���X
//*****************************************************************************
class CAppleMenu : public CSelectMenu3D
{
public:

	CAppleMenu();		//�f�t�H���g�R���X�g���N�^
	CAppleMenu(D3DXVECTOR3 posCenter);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CAppleMenu();		//�f�X�g���N�^
	static CAppleMenu* Create(D3DXVECTOR3 posCenter, CAppleTree* pAppleTree);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	virtual void Draw(void) {}	//�`�揈��

	CGlowApple::APPLE_TYPE GetSelectAppleType(void);	//���ݑI�𒆂̗ь�̎�ނ��擾
	void EndChangeSelect(void);		//�I���̕ύX���I�������Ƃ��̏���

private:
	void SetAppleTypeRandom(void);	//�ь�̎�ނ������_���Ō���
	void SetAppleText(int nSelect);	//�ь�̐����e�L�X�g��ݒ�

	CGlowApple::APPLE_TYPE m_aTypeApple[NUM_SELECT_APPLE_MENU];	//�ь�̎�ނ̑I�������̔z��
	CAppleTree* m_pAppleTree;	//�ь�𐶐�������؂̃|�C���^
	CObject2D* m_pGrowUpText;	//grow up�̉摜�e�L�X�g
	CObject2D* m_apArrow[2];	//���UI
	CFont* m_pFontAppleText;	//�ь�̐����e�L�X�g
};

#endif // !_APPLE_MENU_H_