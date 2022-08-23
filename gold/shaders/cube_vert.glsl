#version 460

layout(location=0) in vec3 i_pos;
layout(location=1) in vec2 i_uv;
layout(location=2) in vec4 i_col;
layout(location=3) in vec3 i_norm;
out vec2 p_uv;
out vec4 p_col;
out vec3 p_norm;

uniform mat4 uniform_matrix_perspective;
uniform mat4 uniform_matrix_view;
uniform mat4 uniform_matrix_model;

uniform vec3 uni_light_pos;
out vec3 p_light_pos;
uniform vec3 uni_light_col;
out vec3 p_light_col;

uniform float uni_ambient_modifier;
out float p_ambient_modifier;

uniform float uni_spec_modifier;
out float p_spec_modifier;

uniform float uni_shininess;
out float p_shininess;

out vec3 p_mod_pos;

uniform vec3 uni_cam_pos;
out vec3 p_cam_pos;

void main(void)
{
    gl_Position = uniform_matrix_perspective * uniform_matrix_view * uniform_matrix_model * vec4(i_pos, 1.0);

    p_uv = i_uv;

    p_col = i_col;
    p_norm = mat3(transpose(inverse(uniform_matrix_model))) * i_norm;

    p_light_pos = uni_light_pos;
    p_light_col = uni_light_col;

    p_ambient_modifier = uni_ambient_modifier;
    p_spec_modifier = uni_spec_modifier;
    p_shininess = uni_shininess;

    p_mod_pos = vec3(uniform_matrix_model * vec4(i_pos, 1.0));
    p_cam_pos = uni_cam_pos;
}