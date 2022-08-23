// Der Shader ist an Code-Auszügen von https://learnopengl.com/Lighting/Basic-Lighting angelehnt

#version 460

// Unsere fragment Farbe
in vec4 p_col;
out vec4 o_col;

// Die Normale
in vec3 p_norm;

// Lichtposition und -farbe
in vec3 p_light_pos;
in vec3 p_light_col;

// Stärke der Ambiente und Specular
in float p_ambient_modifier;
in float p_spec_modifier;
in float p_shininess;

in vec3 p_mod_pos;
in vec3 p_cam_pos;

void main(void)
{
    o_col = vec4(0.5, 0.5, 0.6, 1.0);
}