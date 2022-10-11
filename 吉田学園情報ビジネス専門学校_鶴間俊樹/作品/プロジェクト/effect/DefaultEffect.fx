//=============================================================================
//
// デフォルトのエフェクトファイル [DefaultEffect.fx]
// Author : 鶴間俊樹
//
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================
//マトリックス
float4x4 g_mWorld;	//ワールドマトリックス
float4x4 g_mView;	//ビューマトリックス
float4x4 g_mProj;	//プロジェクションマトリックス

float4x4 g_mLightView;	//ライト目線のビューマトリックス
float4x4 g_mLightProj;	//ライト目線のプロジェクションマトリックス

//位置座標
float4 g_Light;		//ライト
float4 g_posEye;	//視点の位置

//マテリアル
float4 g_matDiffuse;	//マテリアルのディフューズ色
float4 g_matEmissive;	//マテリアルのエミッシブ色
float4 g_matSpecular;	//マテリアルのスペキュラー色　
float g_matPower;		//マテリアルの反射の強度

float4 g_colGlow;	//輪郭の発光色

//フォグ
bool g_bEnableFog;	//フォグが有効かどうか	//if文は良くないらしいので後に方法を調べる
float3 g_fogColor;	//フォグの色
float2 g_fogRange;	//フォグの計算用のベクトル

//テクスチャ
texture g_Texture;	//テクスチャ
texture g_texShadowMap;	//シャドウマップテクスチャ

//=============================================================================
// テクスチャサンプラー
//=============================================================================
//通常のテクスチャサンプラー
sampler texSampler = sampler_state
{
	texture = g_Texture;
	AddressU = WRAP;
	AddressV = WRAP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//シャドウマップテクスチャ用のテクスチャサンプラー
sampler texSamplerShadow = sampler_state
{
	texture = g_texShadowMap;

	//シャドウマップのUVの範囲外は1.0にして影が無いようにする
	BorderColor = float4(1.0, 1.0, 1.0, 1.0);
	AddressU = BORDER;
	AddressV = BORDER;
	AddressW = CLAMP;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//=============================================================================
// 頂点シェーダ
//=============================================================================
//------------------------------
//頂点シェーダの出力定義
//------------------------------
struct VS_OUTPUT
{
	float4 Pos : POSITION;	//位置
	float4 Diffuse : COLOR0;//色
	float4 TexUV : TEXCOORD0;	//テクスチャ座標
	float4 PosWVP : TEXCOORD1;	//変換後の位置

	float4 ZCalcTex : TEXCOORD2;   	// Z値算出用テクスチャ
	float4 ColShadow : COLOR2;		//影ができる場合の色
};

//------------------------------
//デフォルトVS
//------------------------------
VS_OUTPUT RenderSceneVSDefault(
	float4 vPos : POSITION,
	float4 vTexUV : TEXCOORD0)
{
	VS_OUTPUT Out;	//出力

	//WVP変換
	Out.Pos = mul(vPos, g_mWorld);
	Out.Pos = mul(Out.Pos, g_mView);
	Out.Pos = mul(Out.Pos, g_mProj);

	Out.PosWVP = Out.Pos;

	//色
	Out.Diffuse = g_matDiffuse;

	//テクスチャ座標
	Out.TexUV = vTexUV;

	// ライトの目線によるワールドビュー射影変換をする
	float4x4 mat = mul(g_mWorld, g_mLightView);
	mat = mul(mat, g_mLightProj);
	Out.ZCalcTex = mul(vPos, mat);

	//影の色
	Out.ColShadow = float4(0.0, 0.0, 0.0, 0.0);

	return Out;
}

//------------------------------
//VSライト
//------------------------------
VS_OUTPUT RenderSceneVSLight(
	float4 vPos : POSITION,
	float4 vNor : NORMAL,
	float4 vTexUV : TEXCOORD0
)
{
	VS_OUTPUT Out;

	//WVP変換
	Out.Pos = mul(vPos, g_mWorld);
	float3 vecView = normalize(g_posEye.xyz - Out.Pos.xyz);	//視線ベクトル

	Out.Pos = mul(Out.Pos, g_mView);
	Out.Pos = mul(Out.Pos, g_mProj);

	Out.PosWVP = Out.Pos;

	//頂点の法線をワールド変換して正規化
	float3 nor = normalize(mul((float3)vNor, (float3x3)g_mWorld));	//回転のみ
	float3 light = normalize((float3)g_Light);	//頂点からライトへのベクトル


	//影の色(減算する量)
	Out.ColShadow = float4(1.0, 1.0, 1.0, 1.0);

	//ディフューズの計算
	float col = saturate(dot(nor, -light));	//計算後の色の明るさ

	//光があたっていない場合影なし
	if (col <= 0.0) {
		Out.ColShadow = float4(0.0, 0.0, 0.0, 0.0);
	}

	 //ハーフランバート
	//col += 1.0;
	//col *= 0.5;

	//スペキュラーの計算
	float3 vecHarf = normalize(-light + vecView);	//ハーフベクトル

	//色と掛け合わせる
	Out.Diffuse.xyz = g_matDiffuse.xyz * col + pow(g_matSpecular.xyz * saturate(dot(nor, vecHarf)), g_matPower);	//ディフューズ + スペキュラー
	Out.Diffuse.w = g_matDiffuse.w;

	//輪郭を光らせる
	Out.Diffuse.xyz += (pow(1.0 - saturate(dot(vecView, nor)), 3) + 0.1) * (g_colGlow.xyz - Out.Diffuse.xyz);

	//テクスチャ座標
	Out.TexUV = vTexUV;

	// ライトの目線によるワールドビュー射影変換をする
	float4x4 mat = mul(g_mWorld, g_mLightView);
	mat = mul(mat, g_mLightProj);
	Out.ZCalcTex = mul(vPos, mat);


	return Out;
}

//=============================================================================
// ピクセルシェーダ
//=============================================================================
//------------------------------
//ピクセルシェーダ出力定義
//------------------------------
struct PS_OUTPUT
{
	float4 RGB : COLOR0;	//カラー
};

//------------------------------
// 2dテクスチャ無し
//------------------------------
PS_OUTPUT RenderScenePS2D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = In.Diffuse;

	return Out;
}

//------------------------------
// 2dテクスチャあり
//------------------------------
PS_OUTPUT RenderScenePSTex2D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = tex2D(texSampler, In.TexUV) * In.Diffuse;

	return Out;
}

//------------------------------
// 3dデフォルト
//------------------------------
PS_OUTPUT RenderScenePSDefault3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = In.Diffuse;

	//エミッシブを加算
	Out.RGB.xyz += g_matEmissive.xyz;

	//フォグを加算
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * (1.0 - saturate(g_fogRange.x + g_fogRange.y * In.PosWVP.w));

	return Out;
}

//------------------------------
// 3Dライト
//------------------------------
PS_OUTPUT RenderScenePSLight3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = In.Diffuse;

	//-----------------------------
	//影の描画
	//-----------------------------
	// ライト目線によるZ値の再算出
	float ZValue = In.ZCalcTex.z / In.ZCalcTex.w;

	// テクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + In.ZCalcTex.x / In.ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - In.ZCalcTex.y / In.ZCalcTex.w) * 0.5f;

	float visibility = 0.0;	//影の濃度

	float2 poissonDisk[4] = {
		float2(-0.94201624, -0.39906216),
		float2(0.94558609, -0.76890725),
		float2(-0.094184101, -0.92938870),
		float2(0.34495938, 0.29387760)
	};

	//ポアソンサンプリング
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// 同じ座標のZ値を抽出　少しずらす
		float SM_Z = tex2D(texSamplerShadow, TransTexCoord + poissonDisk[nCnt] / 1300.0).x;

		// 算出点がシャドウマップのZ値よりも大きければ影と判断
		if (ZValue > SM_Z + 0.001f && SM_Z < 1.0) {	//シャドウマップのZ値が1.0だったら影ができないようにする
			visibility -= 0.2;
		}
	}

	//影の色を描画
	Out.RGB.xyz += Out.RGB.xyz * visibility * In.ColShadow.xyz;

	//-----------------------------
	//エミッシブを加算
	//-----------------------------
	Out.RGB.xyz += g_matEmissive.xyz;

	//-----------------------------
	//フォグを加算
	//-----------------------------
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * (1.0 - saturate(g_fogRange.x + g_fogRange.y * In.PosWVP.w));

	return Out;
}

//------------------------------
// 3dテクスチャ
//------------------------------
PS_OUTPUT RenderScenePSTex3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = tex2D(texSampler, In.TexUV) * In.Diffuse;

	//エミッシブを加算
	Out.RGB.xyz += g_matEmissive.xyz;

	//フォグを加算
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * (1.0 - saturate(g_fogRange.x + g_fogRange.y * In.PosWVP.w));

	return Out;
}

//------------------------------
// 3dライトテクスチャ
//------------------------------
PS_OUTPUT RenderScenePSLightTex3D(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.RGB = tex2D(texSampler, In.TexUV) * In.Diffuse;

	//-----------------------------
	//影の描画
	//-----------------------------
	// ライト目線によるZ値の再算出
	float ZValue = In.ZCalcTex.z / In.ZCalcTex.w;

	// テクスチャ座標に変換
	float2 TransTexCoord;
	TransTexCoord.x = (1.0f + In.ZCalcTex.x / In.ZCalcTex.w) * 0.5f;
	TransTexCoord.y = (1.0f - In.ZCalcTex.y / In.ZCalcTex.w) * 0.5f;

	float visibility = 0.0;	//影の濃度

	float2 poissonDisk[4] = {
		float2(-0.94201624, -0.39906216),
		float2(0.94558609, -0.76890725),
		float2(-0.094184101, -0.92938870),
		float2(0.34495938, 0.29387760)
	};

	//ポアソンサンプリング
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		// 同じ座標のZ値を抽出　少しずらす
		float SM_Z = tex2D(texSamplerShadow, TransTexCoord + poissonDisk[nCnt] / 1300.0).x;

		// 算出点がシャドウマップのZ値よりも大きければ影と判断
		if (ZValue > SM_Z + 0.001f && SM_Z < 1.0) {	//シャドウマップのZ値が1.0だったら影ができないようにする
			visibility -= 0.2;
		}
	}

	//影の色を描画
	Out.RGB.xyz += Out.RGB.xyz * visibility * In.ColShadow.xyz;

	//-----------------------------
	//エミッシブを加算
	//-----------------------------
	Out.RGB.xyz += g_matEmissive.xyz;

	//-----------------------------
	//フォグを加算
	//-----------------------------
	if (g_bEnableFog) Out.RGB.xyz += (g_fogColor - Out.RGB) * (1.0 - saturate(g_fogRange.x + g_fogRange.y * In.PosWVP.w));

	return Out;
}

//=============================================================================
// 深度バッファ描画用
//=============================================================================
struct VS_OUTPUT_SHADOW
{
	float4 Pos : POSITION;   // 射影変換座標
	float4 ShadowMapTex : TEXCOORD0;   // Zバッファテクスチャ
};

//------------------------------
// 頂点シェーダ
//------------------------------
VS_OUTPUT_SHADOW ZBufferCalc_VS(float4 vPos : POSITION, float4 color : COLOR0)
{
	VS_OUTPUT_SHADOW Out;

	//WVP変換
	Out.Pos = mul(vPos, g_mWorld);
	Out.Pos = mul(Out.Pos, g_mLightView);
	Out.Pos = mul(Out.Pos, g_mLightProj);

	// テクスチャ座標を頂点に合わせる
	Out.ShadowMapTex = Out.Pos;

	return Out;
}

//------------------------------
// ピクセルシェーダ
//------------------------------
float4 ZBufferPlot_PS(float4 ShadowMapTex : TEXCOORD0) : COLOR
{
	// Z値算出
	return ShadowMapTex.z / ShadowMapTex.w;
}

//=============================================================================
// テクニックとパス
//=============================================================================
technique RenderScene
{
	//深度バッファ描画用
	pass P0
	{
		VertexShader = compile vs_3_0 ZBufferCalc_VS();
		PixelShader = compile ps_3_0 ZBufferPlot_PS();
	}

	//-----------------------------------
	//2Dポリゴン
	//-----------------------------------
	//変換済みなので頂点シェーダは必要なさそう
	//2d
	pass P2D
	{
		PixelShader = compile ps_3_0 RenderScenePS2D();
	}
	//2dテクスチャあり
	pass P2D_TEX
	{
		PixelShader = compile ps_3_0 RenderScenePSTex2D();
	}

	//-----------------------------------
	//3Dポリゴン
	//-----------------------------------
	//3d
	pass P3D
	{
		VertexShader = compile vs_3_0 RenderSceneVSDefault();
		PixelShader = compile ps_3_0 RenderScenePSDefault3D();
	}
	//3Dライト
	pass P3D_LIGHT
	{
		VertexShader = compile vs_3_0 RenderSceneVSLight();
		PixelShader = compile ps_3_0 RenderScenePSLight3D();
	}
	//3dテクスチャ
	pass P3D_TEX
	{
		VertexShader = compile vs_3_0 RenderSceneVSDefault();
		PixelShader = compile ps_3_0 RenderScenePSTex3D();
	}
	//3dライトテクスチャ
	pass P3D_LIGHT_TEX
	{
		VertexShader = compile vs_3_0 RenderSceneVSLight();
		PixelShader = compile ps_3_0 RenderScenePSLightTex3D();
	}
}