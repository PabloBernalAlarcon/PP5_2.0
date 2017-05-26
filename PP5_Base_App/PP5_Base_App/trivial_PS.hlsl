Texture2D tex : register(t0);
SamplerState sampleType : register(s0);

struct PixelShaderInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float4 normal : NORMAL;
    float4 wpos : POSITION;
};

float4 Directinol(PixelShaderInput input)
{
	float3 Norm = {-1.0f, 0.0f, 0.0f};
	float3 LD = {500.0f, 500.0f, 500.0f};
	float LR = clamp(dot(-normalize(LD), normalize(Norm)), 0, 1);
	float3 Color = {1.0, 1.0, 1.0};
	float4 result = (Color, 1.0f) * LR;
	return result;
}

cbuffer PointLightData : register(b0)
{
    float3 plightPosition;
    float3 plightDirection;
    float4 plightColor;
    float radius;
}

float4 pointLight(PixelShaderInput input, float3 normal)
{
    float3 lightDir = normalize(plightPosition - input.wpos);
    float lightRatio = clamp(dot(lightDir, normal), 0, 1);

    float attenuation = 1.0f - clamp(length(plightPosition - input.wpos) / radius, 0, 1);

    return lightRatio * plightColor * attenuation;
}


float4 main(PixelShaderInput input) : SV_TARGET
{
	float3 uvs;
	uvs[0] = input.color[0];
	uvs[1] = 1-input.color[1];
	
	 float4 textureColor = tex.Sample(sampleType, uvs);

    return textureColor;
    //float4 ambient = textureColor * 0.3f;
    //return saturate((textureColor * (pointLight(input, input.normal))) + ambient);

	float4 Light = Directinol(input);

	float4 Color = {1.0, 0.0, 0.0, 1.0};

	///return Color * Light;
	return textureColor;
}