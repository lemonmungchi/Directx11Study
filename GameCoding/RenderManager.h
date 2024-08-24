#pragma once
#include "RenderHelper.h"

class RenderManager
{
public:
	RenderManager(ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext);

	void Init();
	//렌더링하는 주체여서 그래픽객체 가지고있기
	void Update(shared_ptr<Graphics> graphics);

private:
	//고속복사
	void PushCameraData();
	void pushTransformData();
	void PushAnimationData();

	void GatherRenderableObjects();
	void RenderObjects();

private:
	ComPtr<ID3D11Device> _device;
	ComPtr<ID3D11DeviceContext> _deviceContext;
	shared_ptr<Pipeline> _pipeline;
private:
	//Camera
	//쉐이더단계에서 더해줄 수 있는 인자같은 존재
	CameraData _cameraData;
	shared_ptr<ConstantBuffer<CameraData>> _cameraBuffer;

	//SRT scale, rotate translate
	TransformData _transformData;
	shared_ptr<ConstantBuffer<TransformData>> _transformBuffer;

	//Animation
	AnimationData _animationData;
	shared_ptr<ConstantBuffer<AnimationData>> _animationBuffer;
private:
	//그려주는 파이프라인 상태
	//RS
	shared_ptr<RasterizerState> _rasterizerState;
	shared_ptr<SamplerState> _samplerState;
	shared_ptr<BlendState> _blendState;

	//그려야할 물체들
	vector<shared_ptr<GameObject>> _renderObjects;
};

