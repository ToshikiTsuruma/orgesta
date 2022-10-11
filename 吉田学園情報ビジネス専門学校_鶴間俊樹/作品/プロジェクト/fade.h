//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �t�F�[�h�N���X
//*****************************************************************************
class CFade
{
public:
	CFade();	//�f�t�H���g�R���X�g���N�^
	~CFade();	//�f�X�g���N�^
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetFade(CScene::SCENE_TYPE typeScene, float fFadeSpeed, int nStopTime);	//�t�F�[�h�̐ݒ�
	void SkipFade(void);	//�t�F�[�h�A�E�g���X�L�b�v������
	bool GetFade(void);		//�t�F�[�h�����ǂ������擾
	bool GetChangeFade(void);	//�t�F�[�h�C������t�F�[�h�A�E�g�ɐ؂�ւ�����^�C�~���O���ǂ���

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	float m_fFadeSpeed;	//�t�F�[�h���鑬�x
	bool m_bFadein;		//�t�F�[�h�����ǂ���
	float m_colA;		//�J���[�̃A���t�@�l
	int m_nStopTime;	//�t�F�[�h�؂�ւ�莞�̒�~����
	int m_nCntStop;		//��~���Ԃ̃J�E���^�[
	CScene::SCENE_TYPE m_nextScene;	//���̃V�[��
	bool m_bChangeFade;	//�t�F�[�h�̐؂�ւ�莞

};

#endif // !_FADE_H_