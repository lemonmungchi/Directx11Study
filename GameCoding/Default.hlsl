
struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;      //�ý��� VALUE  ������ �־���Ѵ�.
    float4 color : COLOR;
};

//���� ���̴�- ��ġ���� -> �����Ͷ�����-������������ ��������� ��/�ܺ� �Ǵ� �� ����
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;
    
    return output;
}

//��� �ȼ����� ��� - �������
float4 PS(VS_OUTPUT input) : SV_Target  //���� Ÿ���ϴ� ������ �����ֱ�
{
    
    
    return input.color;
}