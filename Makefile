hs2h = deps/hmah-shader-to-header-main/bin/hsth

all:
	gcc -g3 src/*.c src/gamescene/*.c -I"deps/SDL-main/include" -L"deps/SDL-main/build" -I"deps/gl3w/include" deps/gl3w/src/gl3w.c -I"deps/cglm-master/include" -l:libSDL3.a -lm -o bin/a.out
shader:
	$(hs2h) -n field_shader -v src/shader.vert -f src/shader.frag -m src/field_shader.h -s src/field_shader.c