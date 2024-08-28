hs2h = deps/hmah-shader-to-header-main/bin/hsth

shader:
	$(hs2h) -n field_shader -v src/shader.vert -f src/shader.frag -m src/field_shader.h -s src/field_shader.c