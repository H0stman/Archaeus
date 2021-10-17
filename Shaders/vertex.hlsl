//Transpose of matrices is required because HLSL is column major but they arrive as row major.

cbuffer matrices : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
}

/* vertex attributes go here to input to the vertex shader */
struct vs_in 
{
   float3 position_local : POS;
	float3 colour : COL;
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out 
{
   vector position_clip : SV_POSITION; // required output of VS
	float3 colour : COL;
};

vs_out vs_main(vs_in input) 
{
	vector temp = mul(vector(input.position_local, 1.0f),world);
	temp = mul(temp, view);
	temp = mul(temp, projection);
  	vs_out output = (vs_out)0; // zero the memory first
  	output.position_clip = temp;
	output.colour = input.colour;
  	return output;
}

