#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(ComPtr<ID3D11DeviceContext> deviceContext)
	:_deviceContext(deviceContext)
{
}

Pipeline::~Pipeline()
{
}

void Pipeline::UpdatePipeline(PipelineInfo info)
{
	//IA - 세팅부분
	_deviceContext->IASetInputLayout(info.inputLayout->GetComPtr().Get());
	_deviceContext->IASetPrimitiveTopology(info.topology);		//삼각형으로 만들어주기
	//VS
	if (info.vertexShader) 
		_deviceContext->VSSetShader(info.vertexShader->GetComPtr().Get(), nullptr, 0);		//이걸로 일하게 
	_deviceContext->VSSetConstantBuffers(0, 1, _constantBuffer->GetComPtr().GetAddressOf());

	//RS
	_deviceContext->RSSetState(_rasterizerState->GetComPtr().Get());

	//PS
	_deviceContext->PSSetShader(_pixelShader->GetComPtr().Get(), nullptr, 0);
	_deviceContext->PSSetShaderResources(0, 1, _texture1->GetComPtr().GetAddressOf());		//0번슬롯에 1개
	_deviceContext->PSSetSamplers(0, 1, _samplerState->GetComPtr().GetAddressOf());
	//OM
	_deviceContext->OMSetBlendState(_blendState->GetComPtr().Get(), _blendState->GetBlendFactor(), _blendState->GetSampleMask());
	//_deviceContext->Draw(_vertices.size(), 0);
	_deviceContext->DrawIndexed(_geometry->GetIndexCount(), 0, 0);
}
