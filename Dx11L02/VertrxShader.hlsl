////////////////////////////////////////////////////////////////////////////////
// Filename: color.vs
// 着色程序是执行模型渲染的一个小程序。这些程序用HLSL编写并保存在名叫color.vs和color.ps的源文件中。
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
