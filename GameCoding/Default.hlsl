
struct VS_INPUT
{
    float4 position : POSITION;
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION; //시스템 VALUE  무조건 있어야한다.
    //float4 color : COLOR;
    float2 uv : TEXCOORD;
};
//카메라와 물체 따로 -> 카메라는 하나 물체는 계속 바꾸면서 사용
cbuffer CameraData : register(b0)
{
    row_major matrix matView;
    row_major matrix matProjection;
}

cbuffer TransformData : register(b1)
{
    row_major matrix matWorld;
}

cbuffer AnimationData : register(b2)
{
    float2 spriteOffset;
    float2 spriteSize;
    float2 textureSize;
    float useAnimation;
    //CPU->CPU넘겨줄때만 더미 필요
}

//정점 쉐이더- 위치관련 -> 레스터라이저-정점바탕으로 도형만들고 내/외부 판단 및 보간
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    //World view projection
    float4 position = mul(input.position, matWorld);
    position = mul(position, matView);
    position = mul(position, matProjection);
    
    output.position = position;
    output.uv = input.uv;
    
    if(useAnimation==1.f)
    {
        // 500 -부분 / 1000 전체
        output.uv *= spriteSize / textureSize;
        output.uv += spriteOffset / textureSize;
    }
    
    return output;
}

//함수의 인자같은 존재, 오브젝트마다 달라질수있다.
Texture2D texture0 : register(t0); //레지스터에 등록

//uv 옵션 관련
SamplerState sampler0 : register(s0);

//모든 픽셀단위 대상 - 색상관련
float4 PS(VS_OUTPUT input) : SV_Target //랜더 타켓하는 곳으로 보내주기
{
    float4 color = texture0.Sample(sampler0, input.uv); //텍스처0번 uv좌표를 통해 색상 빼오기
    
    return color;
}