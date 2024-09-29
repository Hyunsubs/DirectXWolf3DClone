#pragma once


#define SINGLE(Type) private:\
						Type();\
						~Type();\
						friend class CSingleton<Type>;

#define DEVICE 	CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()
#define MAX_LAYER 32

#define DT CTimeMgr::GetInst()->GetDeltaTime()
#define EngineDT CTimeMgr::GetInst()->GetEngineDeltaTime()

#define KEY_CHECK(KEY, STATE) CKeyMgr::GetInst()->GetKeyState(KEY) == STATE
#define KEY_TAP(KEY) CKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::TAP
#define KEY_PRESSED(KEY) CKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::PRESSED
#define KEY_RELEASED(KEY) CKeyMgr::GetInst()->GetKeyState(KEY) == KEY_STATE::RELEASED

#define CLONE(Type) virtual Type* Clone(){return new Type(*this); }
#define CLONE_DISABLE(Type) virtual Type* Clone() { return nullptr; }

#define DELETE(p) if(nullptr != p) delete p; p = nullptr

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 720

#define PI 3.141592
#define TWO_PI PI*2

#define FOV_ANGLE (66 * (PI / 180))
#define DIST_PROJ_PLANE ((WINDOW_WIDTH / 2) / tan(FOV_ANGLE / 2))

#define FINDASSET(TYPE, NAME) CAssetMgr::GetInst()->FindAsset<TYPE>(NAME)
#define FINDMESH(NAME) CAssetMgr::GetInst()->FindAsset<CMesh>(NAME)
#define FINDMTRL(NAME) CAssetMgr::GetInst()->FindAsset<CMaterial>(NAME)
#define FINDSOUND(NAME) CAssetMgr::GetInst()->FindAsset<CSound>(NAME)
#define FINDSPRITE(NAME) CAssetMgr::GetInst()->FindAsset<CSprite>(NAME)
#define FINDFLIPBOOK(NAME) CAssetMgr::GetInst()->FindAsset<CFlipBook>(NAME)
#define FINDMATERIAL(NAME) CAssetMgr::GetInst()->FindAsset<CMaterial>(NAME)
#define FINDPREFAB(NAME) CAssetMgr::GetInst()->FindAsset<CPrefab>(NAME)
#define FINDTEXTURE(NAME) CAssetMgr::GetInst()->FindAsset<CTexture>(NAME)

#define FONT_RGBA(r, g, b, a) (((((BYTE)a << 24 ) | (BYTE)b << 16) | (BYTE)g << 8) | (BYTE)r)


#define TILE_SIZE 64

#define DOOR_TILE 99
#define LEVER_TILE 110

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;