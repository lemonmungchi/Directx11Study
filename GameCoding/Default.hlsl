
struct VS_INPUT
{
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;      //�ý��� VALUE  ������ �־���Ѵ�.
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

cbuffer TransformData : register(b0)
{
    float4 offset;
}

//���� ���̴�- ��ġ���� -> �����Ͷ�����-������������ ��������� ��/�ܺ� �Ǵ� �� ����
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position+offset;
    output.uv = input.uv;
    
    return output;
}

//�Լ��� ���ڰ��� ����, ������Ʈ���� �޶������ִ�.
Texture2D texture0 : register(t0);      //�������Ϳ� ���
SamplerState sampler0 : register(s0);

//��� �ȼ����� ��� - �������
float4 PS(VS_OUTPUT input) : SV_Target  //���� Ÿ���ϴ� ������ �����ֱ�
{
    float4 color = texture0.Sample(sampler0, input.uv);     //�ؽ�ó0�� uv��ǥ�� ���� ���� ������
    
    return color;
}