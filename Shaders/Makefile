all:
	fxc -Zi vertex.hlsl -T vs_5_0 -E vs_main -nologo -Fo vertex.cso
	fxc -Zi pixel.hlsl -T ps_5_0 -E ps_main -nologo -Fo pixel.cso

clean:
	-del *.cso
