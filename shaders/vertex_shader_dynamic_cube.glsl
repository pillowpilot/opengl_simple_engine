#version 430

layout (location = 0) in vec3 position;

uniform float time_factor;
uniform mat4 proj_matrix;

out vec4 varyingColor;

mat4 buildTranslate(float x, float y, float z);
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);

mat4 buildViewMatrix()
{
    mat4 viewMatrix = buildTranslate(0.0, 0.0, -32.0);
    return viewMatrix;
}

mat4 buildModelMatrix(float timeFactor)
{
    float modelTranslationXAxis = 8.0f * sin(0.35f * timeFactor);
    float modelTranslationYAxis = 8.0f * cos(0.52f * timeFactor);
    float modelTranslationZAxis = 8.0f * sin(0.70f * timeFactor);
    mat4 modelTranslation = buildTranslate(modelTranslationXAxis, modelTranslationYAxis, modelTranslationZAxis);

    mat4 modelRotationXAxis = buildRotateX(1.75f * timeFactor);
    mat4 modelRotationYAxis = buildRotateY(1.75f * timeFactor);
    mat4 modelRotationZAxis = buildRotateZ(1.75f * timeFactor);
    mat4 modelRotation = modelRotationXAxis * modelRotationYAxis * modelRotationZAxis;

    mat4 modelMatrix = modelTranslation * modelRotation;
    return modelMatrix;
}

void main()
{
    mat4 viewMatrix = buildViewMatrix();
    mat4 modelMatrix = buildModelMatrix(gl_InstanceID + time_factor);
    mat4 mvMatrix = viewMatrix * modelMatrix;

    gl_Position = proj_matrix * (mvMatrix) * vec4(position, 1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, 
                      0.0, 1.0, 0.0, 0.0, 
                      0.0, 0.0, 1.0, 0.0, 
                      x,   y,   z,   1.0);
    return trans;
}

mat4 buildRotateX(float rad)
{
    mat4 xrot = mat4(1.0, 0.0,      0.0,       0.0, 
                     0.0, cos(rad), -sin(rad), 0.0, 
                     0.0, sin(rad), cos(rad),  0.0, 
                     0.0, 0.0,      0.0,       1.0);
    return xrot;
}

mat4 buildRotateY(float rad)
{
    mat4 yrot = mat4(cos(rad),  0.0, sin(rad), 0.0, 
                     0.0,       1.0, 0.0,      0.0, 
                     -sin(rad), 0.0, cos(rad), 0.0, 
                     0.0,       0.0, 0.0,      1.0);
    return yrot;
}

mat4 buildRotateZ(float rad)
{
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0, 
                     sin(rad), cos(rad),  0.0, 0.0, 
                     0.0,      0.0,       1.0, 0.0, 
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}
