// Generated with Shader Minifier 1.4.0 (https://github.com/laurentlb/Shader_Minifier/)
#ifndef FRAG_H_
# define FRAG_H_

const char *shader_frag =
 "#version 330 core\n"
 "out vec4 FragColor;"
 "in vec2 tc;"
 "uniform sampler2D ourTexture;"
 "uniform float hue_shift;"
 "vec3 t(vec3 v,float t)"
 "{"
   "float d=dot(v,vec3(.596,-.275,-.321)),c=dot(v,vec3(.212,-.523,.311)),i=atan(c,d),a=sqrt(d*d+c*c);"
   "i+=t;"
   "c=a*sin(i);"
   "d=a*cos(i);"
   "v=vec3(dot(v,vec3(.299,.587,.114)),d,c);"
   "return vec3(dot(v,vec3(1,.956,.621)),dot(v,vec3(1,-.272,-.647)),dot(v,vec3(1,-1.107,1.704)));"
 "}"
 "void main()"
 "{"
   "vec4 v=texture(ourTexture,tc);"
   "if(v[3]>.5)"
     "FragColor=vec4(t(v.xyz,hue_shift),1);"
   "else"
     " discard;"
 "}";

#endif // FRAG_H_
