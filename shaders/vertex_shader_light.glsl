#version 430

layout (location = 0) in vec3 position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

mat4 buildTranslate(float x, float y, float z);

void main()
{
    gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}

mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0, 
		      0.0, 0.0, 1.0, 0.0, 
		      x,   y,   z,   1.0);
    return trans;
}

