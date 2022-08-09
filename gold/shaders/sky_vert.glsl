#version 400

layout(location=0) in vec3 i_pos;
layout(location=1) in vec2 i_uv;
layout(location=2) in vec4 i_col;
out vec4 p_col;

uniform mat4 uniform_matrix_perspective;
uniform mat4 uniform_matrix_view;
uniform mat4 uniform_matrix_model;

void main(void)
{
    gl_Position = uniform_matrix_perspective * uniform_matrix_view * uniform_matrix_model * vec4(i_pos, 1.0);
    p_col = i_col;
}