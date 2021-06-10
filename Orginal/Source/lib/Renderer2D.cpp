#include "Renderer2D.h"

#include <stdio.h>

#include "DirectXTK/Inc/WICTextureLoader.h"
#include "Framework.h"
#include "FontTexture.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Sprite.h"

Renderer2D::Renderer2D()
{
    // 頂点バッファ
    Vertex vertices[] =
    {
        {{-0.5f,  0.5f, 0}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, Vector4::One(),},
        {{ 0.5f,  0.5f, 0}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, Vector4::One(),},
        {{-0.5f, -0.5f, 0}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, Vector4::One(),},
        {{ 0.5f, -0.5f, 0}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, Vector4::One(),},
    };

    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(Vertex) * 4;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    D3D11_SUBRESOURCE_DATA subresource;
    ZeroMemory(&subresource, sizeof(subresource));
    subresource.pSysMem = vertices;
    FRAMEWORK.GetDevice()->CreateBuffer(&bufferDesc, &subresource, mVertexBuffer.GetAddressOf());


    mShader = std::make_unique<Shader>();
    mShader->Load(L"Shaders/Renderer2D.fx", "VSMain", "PSMain");

}

Renderer2D::~Renderer2D()
{
    mShader->UnLoad();
}

void Renderer2D::UpdateVertex(ID3D11ShaderResourceView* srv, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center, const float angle, const Vector4 color)
{
    ID3D11DeviceContext* context = FRAMEWORK.GetContext();

    // 画像のサイズ(全体)を取得
    Vector2 texSize;
    {
        ID3D11Resource* tex;
        srv->GetResource(&tex);
        ID3D11Texture2D* tex2d = static_cast<ID3D11Texture2D*>(tex);
        D3D11_TEXTURE2D_DESC desc;
        tex2d->GetDesc(&desc);
        tex2d->Release();

        texSize.x = static_cast<float>(desc.Width);
        texSize.y = static_cast<float>(desc.Height);
    }


    // 頂点バッファ更新
    D3D11_VIEWPORT viewport;
    UINT numViewports = 1;
    context->RSGetViewports(&numViewports, &viewport);

    Vertex vertices[] = {
        { Vector3(0.0f, 0.0f, 0.0f), {0.0f, 0.0f, -1.0f}, Vector2(0.0f, 0.0f), color },
        { Vector3(1.0f, 0.0f, 0.0f), {0.0f, 0.0f, -1.0f}, Vector2(1.0f, 0.0f), color },
        { Vector3(0.0f, 1.0f, 0.0f), {0.0f, 0.0f, -1.0f}, Vector2(0.0f, 1.0f), color },
        { Vector3(1.0f, 1.0f, 0.0f), {0.0f, 0.0f, -1.0f}, Vector2(1.0f, 1.0f), color },
    };

    float sinValue = sinf(angle);
    float cosValue = cosf(angle);
    float mx = (size.x * scale.x) / size.x * center.x;
    float my = (size.y * scale.y) / size.y * center.y;
    for (int i = 0; i < 4; i++)
    {
        vertices[i].position.x *= (size.x * scale.x);
        vertices[i].position.y *= (size.y * scale.y);

        vertices[i].position.x -= mx;
        vertices[i].position.y -= my;

        float rx = vertices[i].position.x;
        float ry = vertices[i].position.y;
        vertices[i].position.x = rx * cosValue - ry * sinValue;
        vertices[i].position.y = rx * sinValue + ry * cosValue;

        vertices[i].position.x += mx;
        vertices[i].position.y += my;

        vertices[i].position.x += (pos.x - scale.x * center.x);
        vertices[i].position.y += (pos.y - scale.y * center.y);

        vertices[i].position.x = vertices[i].position.x * 2 / viewport.Width - 1.0f;
        vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;

        vertices[i].texcoord.x = (texPos.x + vertices[i].texcoord.x * size.x) / texSize.x;
        vertices[i].texcoord.y = (texPos.y + vertices[i].texcoord.y * size.y) / texSize.y;
    }

    context->UpdateSubresource(mVertexBuffer.Get(), 0, NULL, vertices, 0, 0);

}

void Renderer2D::Render(ID3D11ShaderResourceView* srv, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center, const float angle, const Vector4 color)
{
    if (scale.x * scale.y == 0.0f) return;
    if (size.x * size.y == 0.0f) return;

    ID3D11DeviceContext* context = FRAMEWORK.GetContext();   

    // 頂点更新
    UpdateVertex(srv, pos, scale, texPos, size, center, angle, color);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);

    context->RSSetState(FRAMEWORK.GetRasterizer(Framework::RS_CULL_NONE));
    context->OMSetDepthStencilState(FRAMEWORK.GetDepthStencil(Framework::DS_FALSE), 0);
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    context->PSSetShaderResources(0, 1, &srv);
    mShader->Activate();

    context->Draw(4, 0);

    // 戻す
    FRAMEWORK.ResetParam();
}

void Renderer2D::Render(ID3D11ShaderResourceView* srv, Shader* shader, const Vector2& pos, const Vector2& scale, const Vector2& texPos, const Vector2& size, const Vector2& center, const float angle, const Vector4 color)
{
    if (scale.x * scale.y == 0.0f) return;
    if (size.x * size.y == 0.0f) return;

    ID3D11DeviceContext* context = FRAMEWORK.GetContext();

    // 頂点更新
    UpdateVertex(srv, pos, scale, texPos, size, center, angle, color);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &stride, &offset);

    context->RSSetState(FRAMEWORK.GetRasterizer(Framework::RS_CULL_NONE));
    context->OMSetDepthStencilState(FRAMEWORK.GetDepthStencil(Framework::DS_FALSE), 0);
    context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    context->PSSetShaderResources(0, 1, &srv);
    shader->Activate();

    context->Draw(4, 0);

    // 戻す
    FRAMEWORK.ResetParam();
}