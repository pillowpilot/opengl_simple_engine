#version 430

in  vec4 varyingColor;
out vec4 color;

uniform float time_factor;
uniform mat4 proj_matrix;

void main()
{
    color = varyingColor;
}