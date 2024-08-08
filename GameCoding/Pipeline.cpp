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
	
	//RS
	if(info.rasterizerState)
		_deviceContext->RSSetState(info.rasterizerState->GetComPtr().Get());

	//PS
	if(info.pixelShader)
		_deviceContext->PSSetShader(info.pixelShader->GetComPtr().Get(), nullptr, 0);

	//OM
	if(info.blendState)
		_deviceContext->OMSetBlendState(info.blendState->GetComPtr().Get(), info.blendState->GetBlendFactor(), info.blendState->GetSampleMask());
}

void Pipeline::SetVertexBuffer(shared_ptr<VertexBuffer> buffer)
{
	uint32 stride = buffer->GetStride();
	uint32 offset = buffer->GetOffset();
	_deviceContext->IASetVertexBuffers(0, 1, buffer->GetComPtr().GetAddressOf(), &stride, &offset);
}

void Pipeline::SetIndexBuffer(shared_ptr<IndexBuffer> buffer)
{
	_deviceContext->IASetIndexBuffer(buffer->GetComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
}

void Pipeline::SetTexture(uint32 slot, uint32 scope, shared_ptr<Texture> texture)
{
	if (scope & SS_VertexShader)
		_deviceContext->VSSetShaderResources(0, 1, texture->GetComPtr().GetAddressOf());		//0번슬롯에 1개
	
	if (scope & SS_PixelShader)
		_deviceContext->PSSetShaderResources(0, 1, texture->GetComPtr().GetAddressOf());		//0번슬롯에 1개
}

void Pipeline::SetSamplerState(uint32 slot, uint32 scope, shared_ptr<SamplerState> samplerState)
{
	if (scope & SS_VertexShader)
		_deviceContext->VSSetSamplers(0, 1, samplerState->GetComPtr().GetAddressOf());

	if (scope & SS_PixelShader)
		_deviceContext->PSSetSamplers(0, 1, samplerState->GetComPtr().GetAddressOf());
}

void Pipeline::Draw(uint32 vertexcount, uint32 startVertexLocation)
{
	_deviceContext->Draw(vertexcount, startVertexLocation);
}

void Pipeline::DrawIndexed(uint32 indexCount, uint32 startIndexLocation, uint32 baseVertexLocation)
{
	_deviceContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
}
