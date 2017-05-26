cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix projection;
	matrix view;
	matrix model;
};
struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct VIn
{
	float4 position : POSITION;
	float4 color : COLOR;
};
VOut main(VIn input)
{
	VOut output;

	output.position = input.position;
	output.position = mul(output.position,model);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color =input.color;

	return output;
}


