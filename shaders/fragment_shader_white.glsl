#version 430

out vec4 color;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.8;
    vec3 ambient = ambientStrength * lightColor;

    vec3 result = ambient * objectColor;
    color = vec4(result, 1.0);
}

