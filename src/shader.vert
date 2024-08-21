#version 330 core
layout (location = 0) in vec2 aPos;
  

out vec2 tc;

uniform vec2 position;
uniform float scale;

uniform vec2 localPos;
uniform float rotation;

uniform float aspectRatio;


void main()
{
    vec2 pos1 = vec2(aPos.x * scale,aPos.y * scale) + localPos;
    float sz = sin(rotation);
    float cz = cos(rotation);

    float x = pos1.x * cz - pos1.y * sz;
    float y = pos1.y * cz + pos1.x * sz;

    vec2 pos = vec2(x, y) + position; 

    pos = vec2(pos.x / aspectRatio, pos.y);

    tc = (aPos + 1) / 2;

    gl_Position = vec4(pos, 0.0, 1.0); // see how we directly give a vec3 to vec4's constructor

}