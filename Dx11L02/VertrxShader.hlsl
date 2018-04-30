////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
// ��ɫ������ִ��ģ����Ⱦ��һ��С������Щ������HLSL��д������������color.vs��color.ps��Դ�ļ��С�
////////////////////////////////////////////////////////////////////////////////

//////////////
#include "Header.hlsli"

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float4 color : COLOR;
};

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
// ��GPU�����͸����Ķ��㻺���������ʱ��������ö�����ɫ����
// �����㻺���е�ÿ������ʱ�����������ΪColorVertesShader�Ķ�����ɫ����
// �����������ɫ���Ķ��㻺���е����ݸ�ʽ��������ɫ��Դ�ļ��е����ݽṹ��������VertexInputType����ƥ�䡣
// ������ɫ��������ᱻ���͸�������ɫ�������������������涨�����PixelInputType��
//
// ������ɫ�������������������ɫ��������������ͬ�����Ὣ���붥���λ�÷ֱ������������ӿھ����ͶӰ����
// ��Щ������������ǵ��ӽǽ�������ں��ʵ�λ�ò����ճ�������Ļ�ϡ�����������ɫ�ᱻ���Ƶ���������У�Ȼ���������������������ɫ�������������
// ͬʱע����Ϊ����ֻ�ܴ�λ�õõ�xyz���������ǽ�����λ�õ�wֵ���ó�1������������δ�����ֵ��
////////////////////////////////////////////////////////////////////////////////
PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    output.tex = input.tex;

    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}

///////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType TextureVertexShader(VertexInputType input)
{
    PixelInputType output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    return output;
}
