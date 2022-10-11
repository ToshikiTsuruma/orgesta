//=============================================================================
//
// �����_�����O���� [renderer.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "object.h"
#include "fade.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEFAULT_EFFECT_FILE_NAME "effect/DefaultEffect.fx"	//�ǂݍ��ރG�t�F�N�g�t�@�C���̖��O
#define DEFAULT_EFFECT_TECHNIQUE_NAME "RenderScene"	//�G�t�F�N�g�t�@�C����Technique�̖��O

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �T�[�t�F�C�X�̕����擾�֐�
//=============================================================================
bool GetSurfaceWH(IDirect3DSurface9 *pSurf, UINT &uiWidth, UINT &uiHeight)
{
	if (!pSurf) return false;
	D3DSURFACE_DESC SufDesc;
	pSurf->GetDesc(&SufDesc);
	uiWidth = SufDesc.Width;
	uiHeight = SufDesc.Height;

	return true;
}

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_pEffect = nullptr;
	m_colBackBuff = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bDrawZTex = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{
#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif
	// �f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow) {
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//---------------------------------------
	// Direct3D�I�u�W�F�N�g�̍쐬
	//---------------------------------------
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	//---------------------------------------
	// ���݂̃f�B�X�v���C���[�h���擾
	//---------------------------------------
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//---------------------------------------
	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	//---------------------------------------
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;				// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;				// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;							// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	//---------------------------------------
	// �f�o�C�X�̐���
	//---------------------------------------
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	//---------------------------------------
	// �����_�[�X�e�[�g�̐ݒ�
	//---------------------------------------
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// �J�����O���s��Ȃ�
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// ���f�X�e�B�l�[�V�����J���[�̎w��

	//---------------------------------------
	// �T���v���[�X�e�[�g�̐ݒ�
	//---------------------------------------
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	//---------------------------------------
	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	//---------------------------------------
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);



	//---------------------------------------
	// �`��f�o�C�X�T�[�t�F�C�X�Q���擾�ێ�
	//---------------------------------------
	m_pD3DDevice->GetRenderTarget(0, &m_pDefaultSurf);
	m_pD3DDevice->GetDepthStencilSurface(&m_pDefaultDepthSurf);

	//---------------------------------------
	// Z�l�e�N�X�`����Z�l�p�[�x�o�b�t�@�T�[�t�F�C�X�̍쐬
	//---------------------------------------
	int nPixel = 1920;	//Z�l�e�N�X�`���̉𑜓x	1:1�̔䗦�Ŏg�p

	// Z�l�e�N�X�`�����쐬	//Z�l���������ރe�N�X�`��
	if (FAILED(D3DXCreateTexture(m_pD3DDevice, nPixel, nPixel, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16, D3DPOOL_DEFAULT, &m_pTexZBuff))) return 0;
	// Z�l�e�N�X�`���̕�����ێ�
	UINT uiZTexWidth;
	UINT uiZTexHeight;
	// Z�l�e�N�X�`���T�[�t�F�C�X��ێ�
	m_pTexZBuff->GetSurfaceLevel(0, &m_pZTexSurf);
	GetSurfaceWH(m_pZTexSurf, uiZTexWidth, uiZTexHeight);

	// �[�x�o�b�t�@�T�[�t�F�C�X�̍쐬	���O��֌W�𐳂������邽�߂ɕK�v�炵��
	m_pD3DDevice->CreateDepthStencilSurface(uiZTexWidth, uiZTexHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pDepthBuff, NULL);


	//---------------------------------------
	//�G�t�F�N�g�̐���
	//---------------------------------------
	HRESULT hr = 0;	//�n���h��
	LPD3DXBUFFER pErrMessage = nullptr;	//�G���[���b�Z�[�W

	// �G�t�F�N�g�̓ǂݍ���
	hr = D3DXCreateEffectFromFile(
		m_pD3DDevice,
		DEFAULT_EFFECT_FILE_NAME,
		NULL,
		NULL,
		0,
		NULL,
		&m_pEffect,
		&pErrMessage
	);

	if (FAILED(hr)) {
		//�G���[���b�Z�[�W�\��
		MessageBoxA(NULL, (LPCSTR)(pErrMessage->GetBufferPointer()), "", MB_OK);
		pErrMessage->Release();
	}

	//�e�N�j�b�N�̐ݒ�
	if (m_pEffect != nullptr) {
		m_pEffect->SetTechnique(DEFAULT_EFFECT_TECHNIQUE_NAME);
	}

	//�G�t�F�N�g�J�n
	m_pEffect->Begin(NULL, 0);

	//���_��`�̐���
	CreateVtxDecl2D();
	CreateVtxDecl3D();

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(m_pD3DDevice, 18, 0, FW_REGULAR, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Helvetica", &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CRenderer::Uninit(void) {
	//�G�t�F�N�g�I��
	m_pEffect->End();

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif

	//���_��`�̔j��
	ReleaseVtxDecl2D();
	ReleaseVtxDecl3D();

	//Z�l�e�N�X�`���̔j��
	if (m_pTexZBuff != nullptr) {
		m_pTexZBuff->Release();
		m_pTexZBuff = nullptr;
	}

	//Z�l�e�N�X�`���̃T�[�t�F�C�X�̔j��
	if (m_pZTexSurf != nullptr) {
		m_pZTexSurf->Release();
		m_pZTexSurf = nullptr;
	}

	//�[�x�o�b�t�@�̔j��
	if (m_pDepthBuff != nullptr) {
		m_pDepthBuff->Release();
		m_pDepthBuff = nullptr;
	}

	//�G�t�F�N�g�̔j��
	if (m_pEffect != nullptr) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}

	// �f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CRenderer::Update(void) {
	CObject::UpdateAll();
}

//=============================================================================
// �`�揈��
//=============================================================================
void CRenderer::Draw(void) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�J�����̎擾
	CCamera* pCamera = pManager->GetCamera();
	//�t�F�[�h�̎擾
	CFade* pFade = pManager->GetFade();

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//�J�����̃Z�b�g
		if (pCamera != nullptr) {
			pCamera->SetCamera();

			//----------------------------------
			//Z�l�o�b�t�@�̕`��
			//----------------------------------
			//�T�[�t�F�C�X�ƃX�e���V���̐ݒ�
			m_pD3DDevice->SetRenderTarget(0, m_pZTexSurf);
			m_pD3DDevice->SetDepthStencilSurface(m_pDepthBuff);
			// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
			m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);

			//Z�l�o�b�t�@�`�撆
			m_bDrawZTex = true;

			//�e�I�u�W�F�N�g�̕`�揈��
			CObject::DrawAll();

			//�X�y�L�����[�p�̃J�����̎��_�̈ʒu��ݒ�
			D3DXVECTOR4 posV = pCamera->GetPosV();
			SetEffectPosView(posV);
		}

		//----------------------------------
		//�ʏ�̕`��
		//----------------------------------
		//�T�[�t�F�C�X�ƃX�e���V���̐ݒ�
		m_pD3DDevice->SetRenderTarget(0, m_pDefaultSurf);
		m_pD3DDevice->SetDepthStencilSurface(m_pDefaultDepthSurf);
		// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
		m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), m_colBackBuff, 1.0f, 0);	//�t�H�O�Ɠ����F�ɂ���Ƃ�������

		//Z�l�o�b�t�@�`�撆�ł͂Ȃ�
		m_bDrawZTex = false;

		//�V�F�[�_�̃V���h�E�}�b�v�̐ݒ�
		SetEffectTextureShadowMap(m_pTexZBuff);

		//�e�I�u�W�F�N�g�̕`�揈��
		CObject::DrawAll();

		//�t�F�[�h�̕`��
		if (pFade != nullptr) pFade->Draw();

#ifdef _DEBUG
		// FPS�\��
		DrawFPS();
#endif
		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}


	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

#ifdef _DEBUG
//=============================================================================
// FPS�\��
//=============================================================================
void CRenderer::DrawFPS(void) {
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", GetFPS());

	// �e�L�X�g�`��
	m_pFont->DrawText(nullptr, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

#endif

//=============================================================================
// �G�t�F�N�g�̃p�X���J�n
//=============================================================================
void CRenderer::BeginPassEffect(DWORD dwPassFlag) {
	//�[�x�o�b�t�@�`�掞
	if (m_bDrawZTex) {
		m_pEffect->BeginPass(0);
		return;
	}

	PASS_TYPE typePass = PASS_TYPE::DEF_2D;

	//2D�I�u�W�F�N�g�̃p�X
	if (dwPassFlag & PASS_2D) {
		typePass = PASS_TYPE::DEF_2D;	//�e�N�X�`������
		//�e�N�X�`������
		if (dwPassFlag & PASS_TEXTURE) {
			typePass = PASS_TYPE::TEX_2D;
		}
	}
	//3D�I�u�W�F�N�g�̃p�X
	else if (dwPassFlag & PASS_3D) {
		//�e�N�X�`������
		if (dwPassFlag & PASS_TEXTURE) {
			//���C�g����
			if (dwPassFlag & PASS_LIGHT) {
				typePass = PASS_TYPE::LIGHT_TEX_3D;
			}
			//���C�g�Ȃ�
			else {
				typePass = PASS_TYPE::TEX_3D;
			}
		}
		//�e�N�X�`������
		else {
			//���C�g����
			if (dwPassFlag & PASS_LIGHT) {
				typePass = PASS_TYPE::LIGHT_3D;
			}
			//���C�g�Ȃ�
			else {
				typePass = PASS_TYPE::DEF_3D;
			}
		}
	}

	m_pEffect->BeginPass((int)typePass);
}

//=============================================================================
// �G�t�F�N�g�̃p�X���I��
//=============================================================================
void CRenderer::EndPassEffect(void) {
	m_pEffect->EndPass();
}

//�V�F�[�_�̃��[���h�}�g���b�N�X��ݒ�
void CRenderer::SetEffectMatrixWorld(D3DXMATRIX mtxWorld) {
	m_pEffect->SetMatrix("g_mWorld", &mtxWorld);
}

//�V�F�[�_�̃r���[�}�g���b�N�X��ݒ�
void CRenderer::SetEffectMatrixView(D3DXMATRIX mtxView) {
	m_pEffect->SetMatrix("g_mView", &mtxView);
}

//�V�F�[�_�̃v���W�F�N�g�}�g���b�N�X��ݒ�
void CRenderer::SetEffectMatrixProj(D3DXMATRIX mtxProj) {
	m_pEffect->SetMatrix("g_mProj", &mtxProj);
}

//�V�F�[�_�̃e�N�X�`����ݒ�
void CRenderer::SetEffectTexture(LPDIRECT3DTEXTURE9 tex) {
	m_pEffect->SetTexture("g_Texture", tex);
}

//�V�F�[�_�̃V���h�E�}�b�v�e�N�X�`����ݒ�
void CRenderer::SetEffectTextureShadowMap(LPDIRECT3DTEXTURE9 tex) {
	m_pEffect->SetTexture("g_texShadowMap", tex);
}

//�V�F�[�_�̃��C�g��ݒ�
void CRenderer::SetEffectLightVector(D3DXVECTOR4 vecLight) {
	m_pEffect->SetVector("g_Light", &vecLight);
}

//�V�F�[�_�̃��C�g�̃r���[�}�g���b�N�X��ݒ�
void CRenderer::SetEffectLightMatrixView(D3DXMATRIX mtxView) {
	m_pEffect->SetMatrix("g_mLightView", &mtxView);
}

//�V�F�[�_�̃��C�g�̃v���W�F�N�V�����}�g���b�N�X��ݒ�
void CRenderer::SetEffectLightMatrixProj(D3DXMATRIX mtxProj) {
	m_pEffect->SetMatrix("g_mLightProj", &mtxProj);
}

//�V�F�[�_�̎��_��ݒ�
void CRenderer::SetEffectPosView(D3DXVECTOR4 posV) {
	m_pEffect->SetVector("g_posEye", &posV);
}

//�V�F�[�_�̃t�H�O�̗L����Ԃ�ݒ�
void CRenderer::SetEffectFogEnable(bool bEnable) {
	m_pEffect->SetBool("g_bEnableFog", bEnable);
}

//�V�F�[�_�̃t�H�O�̃J���[��ݒ�
void CRenderer::SetEffectFogColor(D3DXCOLOR colFog) {
	D3DXVECTOR4 vecColor = D3DXVECTOR4(colFog.r, colFog.g, colFog.b, colFog.a);
	m_pEffect->SetVector("g_fogColor", &vecColor);
}

//�V�F�[�_�̃t�H�O�͈̔͂�ݒ�
void CRenderer::SetEffectFogRange(float fFogStart, float fFogEnd) {
	float afFogRagne[2] = { fFogEnd / (fFogEnd - fFogStart), -1 / (fFogEnd - fFogStart) };
	m_pEffect->SetFloatArray("g_fogRange", afFogRagne, 2);
}

//�V�F�[�_�̃}�e���A���̃f�B�t���[�Y�F��ݒ�
void CRenderer::SetEffectMaterialDiffuse(D3DXCOLOR matDiffuse) {
	m_pEffect->SetVector("g_matDiffuse", &D3DXVECTOR4(matDiffuse.r, matDiffuse.g, matDiffuse.b, matDiffuse.a));
}

//�V�F�[�_�̃}�e���A���̃G�~�b�V�u�F��ݒ�
void CRenderer::SetEffectMaterialEmissive(D3DXCOLOR matEmissive) {
	m_pEffect->SetVector("g_matEmissive", &D3DXVECTOR4(matEmissive.r, matEmissive.g, matEmissive.b, matEmissive.a));
}

//�V�F�[�_�̃}�e���A���̃X�y�L�����[��ݒ�
void CRenderer::SetEffectMaterialSpecular(D3DXCOLOR matSpecular) {
	m_pEffect->SetVector("g_matSpecular", &D3DXVECTOR4(matSpecular.r, matSpecular.g, matSpecular.b, matSpecular.a));
}

//�V�F�[�_�̃}�e���A���̔��˂̋�����ݒ�
void CRenderer::SetEffectMaterialPower(float matPower) {
	m_pEffect->SetFloat("g_matPower", matPower);
}

//�V�F�[�_�̗֊s�̔����F��ݒ�
void CRenderer::SetEffectColorGlow(D3DXCOLOR colGlow) {
	m_pEffect->SetVector("g_colGlow", &D3DXVECTOR4(colGlow.r, colGlow.g, colGlow.b, colGlow.a));
}


//=============================================================================
// 2D�|���S���̒��_��`�𐶐�
//=============================================================================
void CRenderer::CreateVtxDecl2D(void) {
	//�f�o�C�X��null�̏ꍇ�I��
	if (m_pD3DDevice == nullptr) return;

	//���łɐ�������Ă���ꍇ�������
	if (m_pVtxDecl2D != nullptr) {
		m_pVtxDecl2D->Release();
	}

	//�p�C�v���C���ɓn�����_�f�[�^�̍\�����`
	D3DVERTEXELEMENT9 decl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 }, //�ʒu��RHW
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },  //�F
		{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //�e�N�X�`�����W
		D3DDECL_END() //�Ō�ɕK��D3DDECL_END()������
	};

	//���_�̒�`�I�u�W�F�N�g���쐬����
	m_pD3DDevice->CreateVertexDeclaration(decl, &m_pVtxDecl2D);
}

//=============================================================================
// 2D�|���S���̒��_��`��j��
//=============================================================================
void CRenderer::ReleaseVtxDecl2D(void) {
	//���łɐ�������Ă���ꍇ�������
	if (m_pVtxDecl2D != nullptr) {
		m_pVtxDecl2D->Release();
	}
}

//=============================================================================
// 3D�|���S���̒��_��`�𐶐�
//=============================================================================
void CRenderer::CreateVtxDecl3D(void) {
	//�f�o�C�X��null�̏ꍇ�I��
	if (m_pD3DDevice == nullptr) return;

	//���łɐ�������Ă���ꍇ�������
	if (m_pVtxDecl3D != nullptr) {
		m_pVtxDecl3D->Release();
	}

	//�p�C�v���C���ɓn�����_�f�[�^�̍\�����`
	D3DVERTEXELEMENT9 decl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	//�ʒu
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },	//�@��
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //�e�N�X�`�����W
		D3DDECL_END() //�Ō�ɕK��D3DDECL_END()������
	};

	//���_�̒�`�I�u�W�F�N�g���쐬����
	m_pD3DDevice->CreateVertexDeclaration(decl, &m_pVtxDecl3D);
}

//=============================================================================
// 3D�|���S���̒��_��`��j��
//=============================================================================
void CRenderer::ReleaseVtxDecl3D(void) {
	//���łɐ�������Ă���ꍇ�������
	if (m_pVtxDecl3D != nullptr) {
		m_pVtxDecl3D->Release();
	}
}