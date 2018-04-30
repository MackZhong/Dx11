////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
// 着色程序是执行模型渲染的一个小程序。这些程序用HLSL编写并保存在名叫color.vs和color.ps的源文件中。
////////////////////////////////////////////////////////////////////////////////

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
// 当GPU处理传送给它的顶点缓存里的数据时，它会调用顶点着色器。
// 处理顶点缓存中的每个顶点时都会调用命名为ColorVertesShader的顶点着色程序。
// 输入给顶点着色器的顶点缓存中的数据格式必须与着色器源文件中的数据结构（这里是VertexInputType）相匹配。
// 顶点着色器的输出会被发送给像素着色器。这边输出类型在上面定义的是PixelInputType。
//
// 顶点着色器的输出变量与像素着色器的输入类型相同。它会将输入顶点的位置分别乘以世界矩阵、视口矩阵和投影矩阵。
// 这些操作会根据我们的视角将顶点放在合适的位置并最终呈现在屏幕上。最后输入的颜色会被复制到输出变量中，然后输出变量被当作像素着色器的输入变量。
// 同时注意因为我们只能从位置得到xyz，所以我们将输入位置的w值设置成1，否则它会是未定义的值。
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