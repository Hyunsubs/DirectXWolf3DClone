#pragma once
#include <Engine/CScript.h>

class CBulletScript
	: public CScript
{
private:
    float Speed;
    float life;
    Vec2 Direction;


public:
    virtual void Begin() override;
    virtual void Tick() override;

    void SetDirection(Vec2 _Direction) { Direction = _Direction; }
    void SetSpeed(float _Speed) { Speed = _Speed; }

    void WallCheck(Vec3 _Pos);

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;


    CLONE(CBulletScript)
	CBulletScript();
    CBulletScript(CBulletScript& _Origin);
	~CBulletScript();

};

