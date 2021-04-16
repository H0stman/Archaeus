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
};

/* outputs from vertex shader go here. can be interpolated to pixel shader */
struct vs_out 
{
   vector position_clip : SV_POSITION; // required output of VS
};

vs_out vs_main(vs_in input) 
{
	vector temp = mul(vector(input.position_local, 1.0f), transpose(world));
	temp = mul(temp, transpose(view));
	temp = mul(temp, transpose(projection));
  	vs_out output = (vs_out)0; // zero the memory first
  	output.position_clip = temp;
  	return output;
}

vector ps_main(vs_out input) : SV_TARGET 
{
	return vector(input.position_clip.x / 1424, input.position_clip.y / 720, input.position_clip.z, 1.0); // must return an RGBA colour
}