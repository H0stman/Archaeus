struct vs_out 
{
   vector position_clip : SV_POSITION; // required output of VS
   float3 colour : COL;
};

vector ps_main(vs_out input) : SV_TARGET 
{
	return vector(input.colour, 1.0f); // must return an RGBA colour
}