//=============================================================================
//
// レンダリング処理 [renderer.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "renderer.h"
#include "manager.h"
#include "camera.h"
#include "object.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEFAULT_EFFECT_FILE_NAME "effect/DefaultEffect.fx"	//読み込むエフェクトファイルの名前
#define DEFAULT_EFFECT_TECHNIQUE_NAME "RenderScene"	//エフェクトファイルのTechniqueの名前

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// サーフェイスの幅高取得関数
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
// デフォルトコンストラクタ
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
// デストラクタ
//=============================================================================
CRenderer::~CRenderer()
{
#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif
	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow) {
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//---------------------------------------
	// Direct3Dオブジェクトの作成
	//---------------------------------------
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	//---------------------------------------
	// 現在のディスプレイモードを取得
	//---------------------------------------
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//---------------------------------------
	// デバイスのプレゼンテーションパラメータの設定
	//---------------------------------------
	ZeroMemory(&d3dpp, sizeof(d3dpp));					// ワークをゼロクリア
	d3dpp.BackBufferCount = 1;							// バックバッファの数
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;				// カラーモードの指定
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;			// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil = TRUE;				// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;			// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;							// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	//---------------------------------------
	// デバイスの生成
	//---------------------------------------
	// ディスプレイアダプタを表すためのデバイスを作成
	// 描画と頂点処理をハードウェアで行なう
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// 描画をハードウェアで行い、頂点処理はCPUで行なう
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// 描画と頂点処理をCPUで行なう
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// 生成失敗
				return E_FAIL;
			}
		}
	}

	//---------------------------------------
	// レンダーステートの設定
	//---------------------------------------
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// カリングを行わない
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	//---------------------------------------
	// サンプラーステートの設定
	//---------------------------------------
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	//---------------------------------------
	// テクスチャステージステートの設定
	//---------------------------------------
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);



	//---------------------------------------
	// 描画デバイスサーフェイス群を取得保持
	//---------------------------------------
	m_pD3DDevice->GetRenderTarget(0, &m_pDefaultSurf);
	m_pD3DDevice->GetDepthStencilSurface(&m_pDefaultDepthSurf);

	//---------------------------------------
	// Z値テクスチャとZ値用深度バッファサーフェイスの作成
	//---------------------------------------
	int nPixel = 1920;	//Z値テクスチャの解像度	1:1の比率で使用

	// Z値テクスチャを作成	//Z値を書き込むテクスチャ
	if (FAILED(D3DXCreateTexture(m_pD3DDevice, nPixel, nPixel, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16, D3DPOOL_DEFAULT, &m_pTexZBuff))) return 0;
	// Z値テクスチャの幅高を保持
	UINT uiZTexWidth;
	UINT uiZTexHeight;
	// Z値テクスチャサーフェイスを保持
	m_pTexZBuff->GetSurfaceLevel(0, &m_pZTexSurf);
	GetSurfaceWH(m_pZTexSurf, uiZTexWidth, uiZTexHeight);

	// 深度バッファサーフェイスの作成	※前後関係を正しくするために必要らしい
	m_pD3DDevice->CreateDepthStencilSurface(uiZTexWidth, uiZTexHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, FALSE, &m_pDepthBuff, NULL);


	//---------------------------------------
	//エフェクトの生成
	//---------------------------------------
	HRESULT hr = 0;	//ハンドル
	LPD3DXBUFFER pErrMessage = nullptr;	//エラーメッセージ

	// エフェクトの読み込み
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
		//エラーメッセージ表示
		MessageBoxA(NULL, (LPCSTR)(pErrMessage->GetBufferPointer()), "", MB_OK);
		pErrMessage->Release();
	}

	//テクニックの設定
	if (m_pEffect != nullptr) {
		m_pEffect->SetTechnique(DEFAULT_EFFECT_TECHNIQUE_NAME);
	}

	//エフェクト開始
	m_pEffect->Begin(NULL, 0);

	//頂点定義の生成
	CreateVtxDecl2D();
	CreateVtxDecl3D();

#ifdef _DEBUG
	// デバッグ情報表示用フォントの生成
	D3DXCreateFont(m_pD3DDevice, 18, 0, FW_REGULAR, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Helvetica", &m_pFont);
#endif

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CRenderer::Uninit(void) {
	//エフェクト終了
	m_pEffect->End();

#ifdef _DEBUG
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != nullptr)
	{
		m_pFont->Release();
		m_pFont = nullptr;
	}
#endif

	//頂点定義の破棄
	ReleaseVtxDecl2D();
	ReleaseVtxDecl3D();

	//Z値テクスチャの破棄
	if (m_pTexZBuff != nullptr) {
		m_pTexZBuff->Release();
		m_pTexZBuff = nullptr;
	}

	//Z値テクスチャのサーフェイスの破棄
	if (m_pZTexSurf != nullptr) {
		m_pZTexSurf->Release();
		m_pZTexSurf = nullptr;
	}

	//深度バッファの破棄
	if (m_pDepthBuff != nullptr) {
		m_pDepthBuff->Release();
		m_pDepthBuff = nullptr;
	}

	//エフェクトの破棄
	if (m_pEffect != nullptr) {
		m_pEffect->Release();
		m_pEffect = nullptr;
	}

	// デバイスの破棄
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3Dオブジェクトの破棄
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CRenderer::Update(void) {
	CObject::UpdateAll();
}

//=============================================================================
// 描画処理
//=============================================================================
void CRenderer::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//カメラの取得
	CCamera* pCamera = pManager->GetCamera();
	//フェードの取得
	CFade* pFade = pManager->GetFade();

	// Direct3Dによる描画の開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		//カメラのセット
		if (pCamera != nullptr) {
			pCamera->SetCamera();

			//----------------------------------
			//Z値バッファの描画
			//----------------------------------
			//サーフェイスとステンシルの設定
			m_pD3DDevice->SetRenderTarget(0, m_pZTexSurf);
			m_pD3DDevice->SetDepthStencilSurface(m_pDepthBuff);
			// バックバッファ＆Ｚバッファのクリア
			m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);

			//Z値バッファ描画中
			m_bDrawZTex = true;

			//各オブジェクトの描画処理
			CObject::DrawAll();

			//スペキュラー用のカメラの視点の位置を設定
			D3DXVECTOR4 posV = pCamera->GetPosV();
			SetEffectPosView(posV);
		}

		//----------------------------------
		//通常の描画
		//----------------------------------
		//サーフェイスとステンシルの設定
		m_pD3DDevice->SetRenderTarget(0, m_pDefaultSurf);
		m_pD3DDevice->SetDepthStencilSurface(m_pDefaultDepthSurf);
		// バックバッファ＆Ｚバッファのクリア
		m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), m_colBackBuff, 1.0f, 0);	//フォグと同じ色にするといい感じ

		//Z値バッファ描画中ではない
		m_bDrawZTex = false;

		//シェーダのシャドウマップの設定
		SetEffectTextureShadowMap(m_pTexZBuff);

		//各オブジェクトの描画処理
		CObject::DrawAll();

		//フェードの描画
		if (pFade != nullptr) pFade->Draw();

#ifdef _DEBUG
		// FPS表示
		DrawFPS();
#endif
		// Direct3Dによる描画の終了
		m_pD3DDevice->EndScene();
	}


	// バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

#ifdef _DEBUG
//=============================================================================
// FPS表示
//=============================================================================
void CRenderer::DrawFPS(void) {
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char str[256];

	wsprintf(str, "FPS:%d\n", GetFPS());

	// テキスト描画
	m_pFont->DrawText(nullptr, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}

#endif

//=============================================================================
// エフェクトのパスを開始
//=============================================================================
void CRenderer::BeginPassEffect(DWORD dwPassFlag) {
	//深度バッファ描画時
	if (m_bDrawZTex) {
		m_pEffect->BeginPass(0);
		return;
	}

	PASS_TYPE typePass = PASS_TYPE::DEF_2D;

	//2Dオブジェクトのパス
	if (dwPassFlag & PASS_2D) {
		typePass = PASS_TYPE::DEF_2D;	//テクスチャ無し
		//テクスチャあり
		if (dwPassFlag & PASS_TEXTURE) {
			typePass = PASS_TYPE::TEX_2D;
		}
	}
	//3Dオブジェクトのパス
	else if (dwPassFlag & PASS_3D) {
		//テクスチャあり
		if (dwPassFlag & PASS_TEXTURE) {
			//ライトあり
			if (dwPassFlag & PASS_LIGHT) {
				typePass = PASS_TYPE::LIGHT_TEX_3D;
			}
			//ライトなし
			else {
				typePass = PASS_TYPE::TEX_3D;
			}
		}
		//テクスチャ無し
		else {
			//ライトあり
			if (dwPassFlag & PASS_LIGHT) {
				typePass = PASS_TYPE::LIGHT_3D;
			}
			//ライトなし
			else {
				typePass = PASS_TYPE::DEF_3D;
			}
		}
	}

	m_pEffect->BeginPass((int)typePass);
}

//=============================================================================
// エフェクトのパスを終了
//=============================================================================
void CRenderer::EndPassEffect(void) {
	m_pEffect->EndPass();
}

//シェーダのワールドマトリックスを設定
void CRenderer::SetEffectMatrixWorld(D3DXMATRIX mtxWorld) {
	m_pEffect->SetMatrix("g_mWorld", &mtxWorld);
}

//シェーダのビューマトリックスを設定
void CRenderer::SetEffectMatrixView(D3DXMATRIX mtxView) {
	m_pEffect->SetMatrix("g_mView", &mtxView);
}

//シェーダのプロジェクトマトリックスを設定
void CRenderer::SetEffectMatrixProj(D3DXMATRIX mtxProj) {
	m_pEffect->SetMatrix("g_mProj", &mtxProj);
}

//シェーダのテクスチャを設定
void CRenderer::SetEffectTexture(LPDIRECT3DTEXTURE9 tex) {
	m_pEffect->SetTexture("g_Texture", tex);
}

//シェーダのシャドウマップテクスチャを設定
void CRenderer::SetEffectTextureShadowMap(LPDIRECT3DTEXTURE9 tex) {
	m_pEffect->SetTexture("g_texShadowMap", tex);
}

//シェーダのライトを設定
void CRenderer::SetEffectLightVector(D3DXVECTOR4 vecLight) {
	m_pEffect->SetVector("g_Light", &vecLight);
}

//シェーダのライトのビューマトリックスを設定
void CRenderer::SetEffectLightMatrixView(D3DXMATRIX mtxView) {
	m_pEffect->SetMatrix("g_mLightView", &mtxView);
}

//シェーダのライトのプロジェクションマトリックスを設定
void CRenderer::SetEffectLightMatrixProj(D3DXMATRIX mtxProj) {
	m_pEffect->SetMatrix("g_mLightProj", &mtxProj);
}

//シェーダの視点を設定
void CRenderer::SetEffectPosView(D3DXVECTOR4 posV) {
	m_pEffect->SetVector("g_posEye", &posV);
}

//シェーダのフォグの有効状態を設定
void CRenderer::SetEffectFogEnable(bool bEnable) {
	m_pEffect->SetBool("g_bEnableFog", bEnable);
}

//シェーダのフォグのカラーを設定
void CRenderer::SetEffectFogColor(D3DXCOLOR colFog) {
	D3DXVECTOR4 vecColor = D3DXVECTOR4(colFog.r, colFog.g, colFog.b, colFog.a);
	m_pEffect->SetVector("g_fogColor", &vecColor);
}

//シェーダのフォグの範囲を設定
void CRenderer::SetEffectFogRange(float fFogStart, float fFogEnd) {
	float afFogRagne[2] = { fFogEnd / (fFogEnd - fFogStart), -1 / (fFogEnd - fFogStart) };
	m_pEffect->SetFloatArray("g_fogRange", afFogRagne, 2);
}

//シェーダのマテリアルのディフューズ色を設定
void CRenderer::SetEffectMaterialDiffuse(D3DXCOLOR matDiffuse) {
	m_pEffect->SetVector("g_matDiffuse", &D3DXVECTOR4(matDiffuse.r, matDiffuse.g, matDiffuse.b, matDiffuse.a));
}

//シェーダのマテリアルのエミッシブ色を設定
void CRenderer::SetEffectMaterialEmissive(D3DXCOLOR matEmissive) {
	m_pEffect->SetVector("g_matEmissive", &D3DXVECTOR4(matEmissive.r, matEmissive.g, matEmissive.b, matEmissive.a));
}

//シェーダのマテリアルのスペキュラーを設定
void CRenderer::SetEffectMaterialSpecular(D3DXCOLOR matSpecular) {
	m_pEffect->SetVector("g_matSpecular", &D3DXVECTOR4(matSpecular.r, matSpecular.g, matSpecular.b, matSpecular.a));
}

//シェーダのマテリアルの反射の強さを設定
void CRenderer::SetEffectMaterialPower(float matPower) {
	m_pEffect->SetFloat("g_matPower", matPower);
}

//シェーダの輪郭の発光色を設定
void CRenderer::SetEffectColorGlow(D3DXCOLOR colGlow) {
	m_pEffect->SetVector("g_colGlow", &D3DXVECTOR4(colGlow.r, colGlow.g, colGlow.b, colGlow.a));
}


//=============================================================================
// 2Dポリゴンの頂点定義を生成
//=============================================================================
void CRenderer::CreateVtxDecl2D(void) {
	//デバイスがnullの場合終了
	if (m_pD3DDevice == nullptr) return;

	//すでに生成されている場合解放する
	if (m_pVtxDecl2D != nullptr) {
		m_pVtxDecl2D->Release();
	}

	//パイプラインに渡す頂点データの構造を定義
	D3DVERTEXELEMENT9 decl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITIONT, 0 }, //位置とRHW
		{ 0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },  //色
		{ 0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //テクスチャ座標
		D3DDECL_END() //最後に必ずD3DDECL_END()をつける
	};

	//頂点の定義オブジェクトを作成する
	m_pD3DDevice->CreateVertexDeclaration(decl, &m_pVtxDecl2D);
}

//=============================================================================
// 2Dポリゴンの頂点定義を破棄
//=============================================================================
void CRenderer::ReleaseVtxDecl2D(void) {
	//すでに生成されている場合解放する
	if (m_pVtxDecl2D != nullptr) {
		m_pVtxDecl2D->Release();
	}
}

//=============================================================================
// 3Dポリゴンの頂点定義を生成
//=============================================================================
void CRenderer::CreateVtxDecl3D(void) {
	//デバイスがnullの場合終了
	if (m_pD3DDevice == nullptr) return;

	//すでに生成されている場合解放する
	if (m_pVtxDecl3D != nullptr) {
		m_pVtxDecl3D->Release();
	}

	//パイプラインに渡す頂点データの構造を定義
	D3DVERTEXELEMENT9 decl[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },	//位置
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },	//法線
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //テクスチャ座標
		D3DDECL_END() //最後に必ずD3DDECL_END()をつける
	};

	//頂点の定義オブジェクトを作成する
	m_pD3DDevice->CreateVertexDeclaration(decl, &m_pVtxDecl3D);
}

//=============================================================================
// 3Dポリゴンの頂点定義を破棄
//=============================================================================
void CRenderer::ReleaseVtxDecl3D(void) {
	//すでに生成されている場合解放する
	if (m_pVtxDecl3D != nullptr) {
		m_pVtxDecl3D->Release();
	}
}