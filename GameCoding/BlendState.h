#pragma once
class BlendState
{
public:
	BlendState(ComPtr<ID3D11Device> device);
	~BlendState();

	const float* GetBlendFactor() { return &_blendFactor; }
	uint32 GetSampleMask() { return _sampleMask; }
	ComPtr<ID3D11BlendState> GetComPtr() { return _blendState; }

    void Create(D3D11_RENDER_TARGET_BLEND_DESC blendDesc =
    {
        true,  // BlendEnable
        D3D11_BLEND_SRC_ALPHA,  // SrcBlend
        D3D11_BLEND_INV_SRC_ALPHA,  // DestBlend
        D3D11_BLEND_OP_ADD,  // BlendOp
        D3D11_BLEND_ONE,  // SrcBlendAlpha
        D3D11_BLEND_ZERO,  // DestBlendAlpha
        D3D11_BLEND_OP_ADD,  // BlendOpAlpha
        D3D11_COLOR_WRITE_ENABLE_ALL  // RenderTargetWriteMask
    }, float factor = 0.f);
private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11BlendState> _blendState;
	float _blendFactor = 0.f;
	uint32 _sampleMask = 0xFFFFFFFF;
};

