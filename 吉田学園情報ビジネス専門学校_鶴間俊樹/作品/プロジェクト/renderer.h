//=============================================================================
//
// レンダリング処理 [renderer.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//パス指定のフラグ
#define PASS_2D			(0b0001)	//2Dオブジェクト
#define PASS_3D			(0b0010)	//3Dオブジェクト
#define PASS_TEXTURE	(0b0100)	//テクスチャあり
#define PASS_LIGHT		(0b1000)	//ライトあり

//*****************************************************************************
// レンダリングクラス
//*****************************************************************************
class CRenderer
{
public:
	enum class PASS_TYPE{
		DRAW_ZBUFF = 0,

		DEF_2D,
		TEX_2D,

		DEF_3D,
		LIGHT_3D,
		TEX_3D,
		LIGHT_TEX_3D,

		ENUM_MAX
	};

	CRenderer();	//デフォルトコンストラクタ
	~CRenderer();	//デストラクタ
	HRESULT Init(HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }	//デバイスを取得
	void SetVtxDecl2D(void) { if (m_pD3DDevice != nullptr) m_pD3DDevice->SetVertexDeclaration(m_pVtxDecl2D); }	//頂点定義の設定
	void SetVtxDecl3D(void) { if (m_pD3DDevice != nullptr) m_pD3DDevice->SetVertexDeclaration(m_pVtxDecl3D); }	//頂点定義の設定
	void BeginPassEffect(DWORD dwPassFlag);	//エフェクトのパスを開始
	void EndPassEffect(void);			//エフェクトのパスを終了
	void SetBackBuffColor(D3DXCOLOR col) { m_colBackBuff = col; }	//バックバッファの色の設定
	bool GetDrawZTex(void) { return m_bDrawZTex; }	//Zテクスチャに描画中かどうか
	LPDIRECT3DTEXTURE9 GetZBuffTex(void) { return m_pTexZBuff; }	//Zバッファのテクスチャを取得

	//シェーダのグローバル変数の設定
	void SetEffectMatrixWorld(D3DXMATRIX mtxWorld);		//シェーダのワールドマトリックスを設定
	void SetEffectMatrixView(D3DXMATRIX mtxView);		//シェーダのビューマトリックスを設定
	void SetEffectMatrixProj(D3DXMATRIX mtxProj);		//シェーダのプロジェクトマトリックスを設定

	void SetEffectTexture(LPDIRECT3DTEXTURE9 tex);		//シェーダのテクスチャを設定
	void SetEffectTextureShadowMap(LPDIRECT3DTEXTURE9 tex);		//シェーダのシャドウマップテクスチャを設定

	void SetEffectLightVector(D3DXVECTOR4 vecLight);	//シェーダのライトのベクトルを設定
	void SetEffectLightMatrixView(D3DXMATRIX mtxView);	//シェーダのライトのビューマトリックスを設定
	void SetEffectLightMatrixProj(D3DXMATRIX mtxProj);	//シェーダのライトのプロジェクションマトリックスを設定
	void SetEffectPosView(D3DXVECTOR4 posV);			//シェーダの視点を設定

	void SetEffectFogEnable(bool bEnable);				//シェーダのフォグの有効状態を設定
	void SetEffectFogColor(D3DXCOLOR colFog);			//シェーダのフォグのカラーを設定
	void SetEffectFogRange(float fFogStart, float fFogEnd);	//シェーダのフォグの範囲を設定

	void SetEffectMaterialDiffuse(D3DXCOLOR matDiffuse);	//シェーダのマテリアルのディフューズ色を設定
	void SetEffectMaterialEmissive(D3DXCOLOR matEmissive);	//シェーダのマテリアルのエミッシブ色を設定
	void SetEffectMaterialSpecular(D3DXCOLOR matSpecular);	//シェーダのマテリアルのスペキュラー色を設定
	void SetEffectMaterialPower(float matPower);			//シェーダのマテリアルの反射の強さを設定

	void SetEffectColorGlow(D3DXCOLOR colGlow);			//シェーダの輪郭の発光色を設定

private:
	void CreateVtxDecl2D(void);		//2Dポリゴンの頂点定義を生成
	void ReleaseVtxDecl2D(void);	//2Dポリゴンの頂点定義を破棄
	void CreateVtxDecl3D(void);		//3Dポリゴンの頂点定義を生成
	void ReleaseVtxDecl3D(void);	//3Dポリゴンの頂点定義を破棄

	LPDIRECT3D9	m_pD3D;					// Direct3Dオブジェクト
	LPDIRECT3DDEVICE9 m_pD3DDevice;		// Deviceオブジェクト(描画に必要)
	LPD3DXEFFECT m_pEffect;				//エフェクト
	LPDIRECT3DVERTEXDECLARATION9 m_pVtxDecl2D;	//2Dポリゴンの頂点定義
	LPDIRECT3DVERTEXDECLARATION9 m_pVtxDecl3D;	//3Dポリゴンの頂点定義

	D3DXCOLOR m_colBackBuff;	//バックバッファの色
	bool m_bDrawZTex;	//Zテクスチャに描画中かどうか
	LPDIRECT3DSURFACE9 m_pDefaultSurf;		//もとのサーフェイス
	LPDIRECT3DSURFACE9 m_pDefaultDepthSurf;	//もとのステンシルバッファ
	LPDIRECT3DTEXTURE9 m_pTexZBuff;		//Z値のテクスチャ
	LPDIRECT3DSURFACE9 m_pZTexSurf;		//Zテクスチャのサーフェイス
	LPDIRECT3DSURFACE9 m_pDepthBuff;	//深度バッファサーフェイス

#ifdef _DEBUG
	void DrawFPS(void);	//FPS描画処理
	LPD3DXFONT m_pFont = NULL;			// フォントへのポインタ
#endif

};

#endif // !_RENDERER_H_