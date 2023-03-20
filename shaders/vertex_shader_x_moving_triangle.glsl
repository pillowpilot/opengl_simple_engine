#version 420
uniform float offset;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 scale_matrix;

void main(void)
{ 
    if(gl_VertexID == 0) gl_Position = scale_matrix * vec4(0.0 + offset, 0.0, 0.0, 1.0); // vertex at origin
    else if (gl_VertexID == 1) gl_Position = scale_matrix * vec4(-0.25 + offset, 0.25, 0.0, 1.0); // left vertex
    else gl_Position = scale_matrix * vec4(0.25 + offset, 0.25, 0.0, 1.0); // right vertex
}
