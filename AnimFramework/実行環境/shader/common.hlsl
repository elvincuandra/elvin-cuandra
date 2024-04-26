struct MATERIAL
{
    float4 Diffuse;
    float4 Ambient;
    float4 Specular;
    float4 Emission;
    float Shininess;
    bool TextureEnable;
    float2 dummy;
};


cbuffer World : register(b0)
{
    matrix world;
}

cbuffer View : register(b1)
{
    matrix view;
}

cbuffer Proj : register(b2)
{
    matrix proj;
}

cbuffer Material : register(b3)
{
    MATERIAL material;
};

cbuffer BoneMatrixBuffer : register(b4)
{
    matrix BoneMatrix[400];
};