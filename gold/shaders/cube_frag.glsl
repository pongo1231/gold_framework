// Der Shader ist an Code-Auszügen von https://learnopengl.com/Lighting/Basic-Lighting angelehnt

#version 460

in vec2 p_uv;

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

uniform sampler2D u_texture;

void main(void)
{
    vec3 ambient = p_ambient_modifier * p_light_col;

    vec3 norm = normalize(p_norm);
    vec3 light_dir = normalize(p_light_pos - p_mod_pos);

    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * p_light_col;
    
    vec3 view_dir = normalize(p_cam_pos - p_mod_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);

    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), p_shininess);
    vec3 specular = p_spec_modifier * spec * p_light_col;

    o_col = texture(u_texture, p_uv);
    /*o_col = vec4(p_col.x * (ambient.x + diffuse.x + specular.x),
        p_col.y * (ambient.y + diffuse.y + specular.y),
        p_col.z * (ambient.z + diffuse.z + specular.z), p_col.w);*/
}