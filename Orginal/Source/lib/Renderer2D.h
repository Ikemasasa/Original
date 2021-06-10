#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <memory>

#include "ResourceManager.h"
#include "Vector.h"

class FontTexture;
class Shader;
class Sprite;

class Renderer2D
{
private:
    struct Vertex
    {
        Vector3 position;
        Vector3 normal;
        Vector2 texcoord;
        Vector4 color;
    };

    Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer = nullptr;
    std::unique_ptr<Shader> mShader;

    // é¿ë‘çÏê¨ã÷é~
    Renderer2D();
    ~Renderer2D();

    void UpdateVertex(
        ID3D11ShaderResourceView* srv,
        const Vector2& pos,
        const Vector2& scale,
        const Vector2& texPos,
        const Vector2& size,
        const Vector2& center,
        const float angle,
        const Vector4 color);

public:
    static Renderer2D& GetInstance()
    {
        static Renderer2D inst;
        return inst;
    }

    void Render(
        ID3D11ShaderResourceView* srv,
        const Vector2& pos,
        const Vector2& scale,
        const Vector2& texPos,
        const Vector2& size,
        const Vector2& center = Vector2::Zero(),
        const float angle = 0,
        const Vector4 color = Vector4::One());

    void Render(
        ID3D11ShaderResourceView* srv,
        Shader* shader,
        const Vector2& pos,
        const Vector2& scale,
        const Vector2& texPos,
        const Vector2& size,
        const Vector2& center = Vector2::Zero(),
        const float angle = 0,
        const Vector4 color = Vector4::One());
};

