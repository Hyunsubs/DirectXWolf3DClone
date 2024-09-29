#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;


class CTileMap :
    public CRenderComponent
{
private:
    int                     m_Row;              // TileMap �� �� ����
    int                     m_Col;              // TileMap �� �� ����
    Vec2                    m_TileSize;         // Tile 1���� ũ��

    Ptr<CTexture>           m_TileAtlas;        // Tile ���� �̹������� �����ϰ� �ִ� ��Ʋ�� �ؽ���
    Vec2                    m_AtlasResolution;  // Atlas �ؽ��� �ػ�
    Vec2                    m_AtlasTileSize;    // Atlas �ؽ��� ������ Ÿ�� 1���� ũ��
    Vec2                    m_AtlasTileSliceUV; // Atlas �ؽ��� ������ Ÿ�� 1���� ũ�⸦ UV �� ��ȯ�� ũ��(SliceUV)

    int                     m_AtlasMaxRow;      // Atlas �ؽ��İ� �����ϰ� �ִ� Ÿ���� �ִ� �� ����
    int                     m_AtlasMaxCol;      // Atlas �ؽ��İ� �����ϰ� �ִ� Ÿ���� �ִ� �� ����

  
    vector<tTileInfo>       m_vecTileInfo;      // ������ Ÿ���� ����
    CStructuredBuffer*      m_Buffer;           // ������ Ÿ�������� t �������ͷ� �����ϱ� ���� ����ȭ����


public:
    void SetRowCol(UINT _Row, UINT _Col);
    void SetTileSize(Vec2 _Size);
    void SetAtlasTexture(Ptr<CTexture> _Atlas);
    void SetAtlasTileSize(Vec2 _TileSize);
    void ChangeTileInfo(UINT _Row, UINT _Col, UINT _Idx);
    void SetTileInfo(vector<tTileInfo>& _TileInfo) { m_vecTileInfo = _TileInfo; }

    vector<tTileInfo>& GetTileInfo() { return m_vecTileInfo; }
    UINT GetRow() { return m_Row; }
    UINT GetCol() { return m_Col; }
    Vec2 GetTileSize() { return m_TileSize; }
    int GetAtlasMaxRow() { return m_AtlasMaxRow; }
    int GetAtlasMaxCol() { return m_AtlasMaxCol; }
    Vec2 GetTileSliceUV() { return m_AtlasTileSliceUV; }

    Vec2 GetAtlasTileSize() { return m_AtlasTileSize; }

    Ptr<CTexture> GetAtlasTexture() { return m_TileAtlas; }

    CStructuredBuffer* GetTileBuffer() { return m_Buffer; }

private:
    void ChangeTileMapSize();


public:
    virtual void Init() override;
    virtual void FinalTick() override;
    virtual void Render() override;
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;
public:
    CLONE(CTileMap)
    CTileMap();
    CTileMap(const CTileMap& _Origin);
    ~CTileMap();
};

