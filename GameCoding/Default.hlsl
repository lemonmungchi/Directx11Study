
struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;      //시스템 VALUE  무조건 있어야한다.
    float4 color : COLOR;
};

//정점 쉐이더- 위치관련 -> 레스터라이저-정점바탕으로 도형만들고 내/외부 판단 및 보간
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = input.position;
    output.color = input.color;
    
    return output;
}

//모든 픽셀단위 대상 - 색상관련
float4 PS(VS_OUTPUT input) : SV_Target  //랜더 타켓하는 곳으로 보내주기
{
    
    
    return input.color;
}