static const float2 Resolution = float2(1280.0f - 0.5f, 720.0f - 0.5f);

sampler2D sampler0;
sampler2D sampler1;

void OverlayPS(
in float2 position : VPOS,
in float2 texCoord : TEXCOORD0,
in float2 texCoord2 : TEXCOORD1,
in float4 diffuse : COLOR0,
out float4 color : COLOR0)
{
    float4 Tex0 = tex2D(sampler0, texCoord);
    Tex0 *= diffuse;
    //Tex0.a *= 2;
    Tex0.rgb = Tex0.rgb * Tex0.a + 0.5 * (1.0 - Tex0.a);

    float4 Tex1 = tex2D(sampler1, position / Resolution);

    if (Tex1.r < 0.5)
    {
        color.r = 2.0 * Tex0.r * Tex1.r;
    }
    else
    {
        color.r = 1.0 - 2.0 * (1.0 - Tex0.r) * (1.0 - Tex1.r);
    }

    if (Tex1.g < 0.5)
    {
        color.g = 2.0 * Tex0.g * Tex1.g;
    }
    else
    {
        color.g = 1.0 - 2.0 * (1.0 - Tex0.g) * (1.0 - Tex1.g);
    }

    if (Tex1.b < 0.5)
    {
        color.b = 2.0 * Tex0.b * Tex1.b;
    }
    else
    {
        color.b = 1.0 - 2.0 * (1.0 - Tex0.b) * (1.0 - Tex1.b);
    }

    color.a = Tex1.a;
}