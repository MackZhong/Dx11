/////////////
// GLOBALS //
// 在着色程序中首先出现的是全局变量。
// 这些变量可以在外部的C++程序中被修改。
// 可以使用很多类型的变量例如int或float，然后在外部程序修改以便着色程序使用。
// 通常会把大多数类型的全局变量放在一个叫cbuffer的缓冲区对象类型中，即便只有一个变量。
// 由于显卡存储缓存区对象的方式，正确的组织这些缓冲区对于着色器程序的高效执行很有帮助。
// 这里有三个matrix变量，由于会在每帧同时更新它们，把它们放在同一个缓冲区。
/////////////
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

//////////////
// TYPEDEFS //
// 与C语言类似我们可以创建自定义类型。
// 为了程序更容易编写和可读，HLSL拥有多种数据类型，例如float4。
// 本例中我们使用了拥有xyzw的向量类型和rgba的颜色类型。
// POSITION, COLOR, 和SV_POSITOIN是用于告诉GPU变量用途的语义词。
// 由于顶点着色器和像素着色器的语义不一样，虽然结构体里的变量一样，但我还是要创建两个结构体。
// 顶点着色器可识别POSITION，像素着色器可识别SV_POSITION，它们还都识别COLOR。
// 如果需要使用多个同一个类型的变量，需要在语义词后添加数字例如：COLOR0, COLOR1, 以此类推。

//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float4 color : COLOR;
};