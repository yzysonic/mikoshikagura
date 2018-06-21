float4x4 WorldViewProj : WorldViewProjection;

struct Vertex3D
{
    float4 position : POSITION0;
    float4 color	: COLOR0;
    float2 uv		: TEXCOORD0;
};

void NormalVS(inout Vertex3D vertex)
{
    vertex.position = mul(vertex.position, WorldViewProj);
}