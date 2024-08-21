all:
	gcc src/*.c -I"deps/SDL-main/include" -L"deps/SDL-main/build" -I"deps/gl3w/include" deps/gl3w/src/gl3w.c -I"deps/cglm/include" -lSDL3 -lm -o bin/a.out
shader:
	mono deps/shader_minifier.exe --format c-variables --preserve-externals src/shader.frag -o src/frag.h
	mono deps/shader_minifier.exe --format c-variables --preserve-externals src/shader.vert -o src/vert.h
	mono deps/shader_minifier.exe --format c-variables --preserve-externals src/selDbg.frag -o src/selDbg.h