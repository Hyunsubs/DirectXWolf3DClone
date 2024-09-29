#include "pch.h"
#include "CAssetMgr.h"
#include "CDevice.h"

void CAssetMgr::Init()
{

	CreateEngineMesh();

	CreateEngineTexture();

	CreateEngineSprite();

	CreateEngineGraphicShader();

	CreateEngineComputeShader();

	CreateEngineMaterial();
}


void CAssetMgr::CreateEngineMesh()
{
	Ptr<CMesh> pMesh = nullptr;
	Vtx v;

	// PointMesh
	pMesh = new CMesh;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT i = 0;
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh);

	// RectMesh 생성	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);


	// Index 버퍼 생성
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;
	arrIdx[3] = 0; 	arrIdx[4] = 2;	arrIdx[5] = 3;

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh", pMesh);


	// RectMesh_Debug
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0;

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh_Debug", pMesh);


	// CircleMesh 
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;


	int Slice = 40;
	float fTheta = XM_2PI / Slice;
	float Radius = 0.5f;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// 테두리
	float Angle = 0.f;
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -(v.vPos.y - 0.5f));
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		vecVtx.push_back(v);

		Angle += fTheta;
	}

	// 인덱스
	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh", pMesh);

	// CircleMesh_Debug
	vecIdx.clear();
	for (size_t i = 1; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back((UINT)i);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh_Debug", pMesh);


	// LineMesh 생성	
	// 0 -- 1
	// | \  |
	// 3 -- 2
	// LineMesh_Debug
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2; arrIdx[3] = 3; arrIdx[4] = 0;

	arrVtx[0].vPos = Vec3(0.f, 1.f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.f, 1.f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.f, 0.f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(0.f, 0.f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetEngineAsset();
	AddAsset(L"LineMesh_Debug", pMesh);

}

void CAssetMgr::CreateEngineTexture()
{
	// PostProcess 용도 텍스쳐 생성
	Vec2 Resolution = CDevice::GetInst()->GetResolution();
	Ptr<CTexture> pPostProcessTex = CreateTexture(
		L"PostProcessTex"
		, (UINT)Resolution.x, (UINT)Resolution.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);

	Ptr<CTexture> pDownScaleTex = CreateTexture(
		L"DownScaleTex"
		, (UINT)(Resolution.x * 0.2f), (UINT)(Resolution.y * 0.2f)
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


	// Noise Texture
	Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");
}

void CAssetMgr::CreateEngineSprite()
{

}

void CAssetMgr::CreateEngineGraphicShader()
{
	Ptr<CGraphicShader> pShader = nullptr;

	// Std2DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	pShader->AddTexParam(TEX_0, "OutputTexture");

	AddAsset(L"Std2DShader", pShader);


	// Std2DAlphaBlend
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Alphablend");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"Std2DAlphaBlendShader", pShader);

	// UIShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\ui.fx", "VS_UI2D");
	pShader->CreatePixelShader(L"shader\\ui.fx", "PS_UI2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	AddAsset(L"UIShader", pShader);



	// DebugShapeShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(L"DebugShapeShader", pShader);


	// TileMapShader
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader);


	// ParticleShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);

	AddAsset(L"ParticleRenderShader", pShader);


	// GrayFilterShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayFilter");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"GrayFilterShader", pShader);

	// DistortionShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"DistortionShader", pShader);


	// RayCaster2D Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\raycaster.fx", "VS_RayCast2D");
	pShader->CreatePixelShader(L"shader\\raycaster.fx", "PS_RayCast2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);
	AddAsset(L"RayCaster2DShader", pShader);

	// Misc Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	// Fade Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Fade");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Fade");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"FadeShader", pShader);
	pShader->AddScalarParam(FLOAT_0, "FadeFactor");

	// Yellow Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Fade");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Yellow");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"YellowShader", pShader);

	// Red Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Fade");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Red");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"RedShader", pShader);

	// Melting Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Fade");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Melting");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"MeltingShader", pShader);

	// NoiseRed Shader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Fade");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_NoiseRed");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"NoiseRedShader", pShader);

}

#include "CParticleTickCS.h"

void CAssetMgr::CreateEngineComputeShader()
{
	// ParticleTick
	Ptr<CComputeShader> pCS = nullptr;

	pCS = new CParticleTickCS;
	AddAsset<CComputeShader>(L"ParticleTickCS", pCS);
}

void CAssetMgr::CreateEngineMaterial()
{
	Ptr<CMaterial>	pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl", pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset(L"Std2DAlphaBlendMtrl", pMtrl);

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset(L"TileMapMtrl", pMtrl);

	// ParticleRenderMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleRenderShader"));
	AddAsset(L"ParticleRenderMtrl", pMtrl);


	// GrayFilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"GrayFilterShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"texture\\noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"texture\\noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<CTexture>(L"texture\\noise\\noise_03.jpg"));
	AddAsset(L"GrayFilterMtrl", pMtrl);

	// DistortionMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"texture\\noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"texture\\noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<CTexture>(L"texture\\noise\\noise_03.jpg"));
	AddAsset(L"DistortionMtrl", pMtrl);

	// RayCaster2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RayCaster2DShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"texture\\Wolf3DWall.png"));
	AddAsset(L"RayCaster2DMtrl", pMtrl);

	// MiscMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MiscShader"));
	AddAsset(L"MiscMtrl", pMtrl);

	// UIMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"UIShader"));
	AddAsset(L"UIMtrl", pMtrl);

	// FadeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"FadeShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"FadeMtrl", pMtrl);

	// YellowMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"YellowShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"YellowMtrl", pMtrl);

	// RedMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RedShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"RedMtrl", pMtrl);

	//MeltingMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MeltingShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"MeltingMtrl", pMtrl);

	//NoiseRedMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"NoiseRedShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"texture\\noise\\noise_01.png"));
	AddAsset(L"NoiseRedMtrl", pMtrl);
}
