cbuffer staticMeshBuffer : register(b0)
{
    float4x4 W;
    float4x4 VP;
    float4x4 bones[256];
};

struct VS_INPUT
{
    float4 Pos : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
    uint4 BoneIDs : BONEIDS;
    
    float4 BoneWeights : BONEWEIGHTS;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    float4 pos = input.Pos;
    pos.w = 1.0f; 
    
    float4x4 transform;
    transform[0] = float4(1, 0, 0, 0);
    transform[1] = float4(0, 1, 0, 0);
    transform[2] = float4(0, 0, 1, 0);
    transform[3] = float4(0, 0, 0, 1);
    
    output.Pos = mul(pos, transform);
    output.Pos = mul(output.Pos, W);
    output.Pos = mul(output.Pos, VP);

    output.Normal = mul(input.Normal, (float3x3) transform);
    output.Normal = mul(output.Normal, (float3x3) W);
    output.Normal = normalize(output.Normal);

    output.Tangent = mul(input.Tangent, (float3x3) transform);
    output.Tangent = mul(output.Tangent, (float3x3) W);
    output.Tangent = normalize(output.Tangent);

    output.TexCoords = input.TexCoords;
    return output;
}