
struct VS_INPUT
{
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION; //�ý��� VALUE  ������ �־���Ѵ�.
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    //hlsl���� ������ �ڵ���� ������ �޶�
    row_major matrix matWorld;
    row_major matrix matView;
    row_major matrix matProjection;
}

//���� ���̴�- ��ġ���� -> �����Ͷ�����-������������ ��������� ��/�ܺ� �Ǵ� �� ����
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    //World view projection
    float4 position = mul(input.position, matWorld);
    position = mul(position, matView);
    position = mul(position, matProjection);
    
    output.position = position;
    output.uv = input.uv;
    
    return output;
}

//�Լ��� ���ڰ��� ����, ������Ʈ���� �޶������ִ�.
Texture2D texture0 : register(t0); //�������Ϳ� ���
SamplerState sampler0 : register(s0);

//��� �ȼ����� ��� - �������
float4 PS(VS_OUTPUT input) : SV_Target //���� Ÿ���ϴ� ������ �����ֱ�
{
    float4 color = texture0.Sample(sampler0, input.uv); //�ؽ�ó0�� uv��ǥ�� ���� ���� ������
    
    return color;
}