// Generated with Shader Minifier 1.4.0 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef VERT_H_
# define VERT_H_

const char *shader_vert =
 "#version 330 core\n"
 "layout(location=0) in vec2 aPos;"
 "out vec2 tc;"
 "uniform vec2 position;"
 "uniform float scale;"
 "uniform vec2 localPos;"
 "uniform float rotation,aspectRatio;"
 "void main()"
 "{"
   "vec2 a=vec2(aPos.x*scale,aPos.y*scale)+localPos;"
   "float n=sin(rotation),s=cos(rotation);"
   "a=vec2(a.x*s-a.y*n,a.y*s+a.x*n)+position;"
   "a=vec2(a.x/aspectRatio,a.y);"
   "tc=(aPos+1)/2;"
   "gl_Position=vec4(a,0,1);"
 "}";

#endif // VERT_H_
