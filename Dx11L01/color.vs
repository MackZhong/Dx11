////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
// ��ɫ������ִ��ģ����Ⱦ��һ��С������Щ������HLSL��д������������color.vs��color.ps��Դ�ļ��С�
////////////////////////////////////////////////////////////////////////////////

/////////////
// GLOBALS //
// ����ɫ���������ȳ��ֵ���ȫ�ֱ�����
// ��Щ�����������ⲿ��C++�����б��޸ġ�
// ����ʹ�úܶ����͵ı�������int��float��Ȼ�����ⲿ�����޸��Ա���ɫ����ʹ�á�
// ͨ����Ѵ�������͵�ȫ�ֱ�������һ����cbuffer�Ļ��������������У�����ֻ��һ��������
// �����Կ��洢����������ķ�ʽ����ȷ����֯��Щ������������ɫ������ĸ�Чִ�к��а�����
// ����������matrix���������ڻ���ÿ֡ͬʱ�������ǣ������Ƿ���ͬһ����������
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
// ��C�����������ǿ��Դ����Զ������͡�
// Ϊ�˳�������ױ�д�Ϳɶ���HLSLӵ�ж����������ͣ�����float4��
// ����������ʹ����ӵ��xyzw���������ͺ�rgba����ɫ���͡�
// POSITION, COLOR, ��SV_POSITOIN�����ڸ���GPU������;������ʡ�
// ���ڶ�����ɫ����������ɫ�������岻һ������Ȼ�ṹ����ı���һ�������һ���Ҫ���������ṹ�塣
// ������ɫ����ʶ��POSITION��������ɫ����ʶ��SV_POSITION�����ǻ���ʶ��COLOR��
// �����Ҫʹ�ö��ͬһ�����͵ı�������Ҫ������ʺ�����������磺COLOR0, COLOR1, �Դ����ơ�
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
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
    
    // Store the input color for the pixel shader to use.
    output.color = input.color;
    
    return output;
}