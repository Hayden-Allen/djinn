#version 410 core
TYPE(ANIMATED);
uniform mat4 d_vp, d_view;
out vec3 v_pos_world, v_norm_world;
out vec2 v_tex;
out vec4 v_weights;

void main()
{
    mat4 bones = (
        d_instance.d_bones[d_index.x] * d_weight.x +
        d_instance.d_bones[d_index.y] * d_weight.y +
        d_instance.d_bones[d_index.z] * d_weight.z +
        d_instance.d_bones[d_index.w] * d_weight.w
    );
	mat4 transform = d_instance.d_model * bones;
    vec4 pos = vec4(d_pos, 1);
    gl_Position = d_vp * transform * pos;
    v_pos_world = vec3(transform * pos);
    v_tex = d_tex;
    
    // WARNING: may cause serious issues!!!!!!!!!!!!!!!!!
    // make sure to check this if anything looks weird
    // if you forgot to check this, well, um, i guess this isn't a sufficient way to document this potential problem
    mat3 normal = mat3(transform);
    v_norm_world = normal * d_norm;
    v_weights = vec4(d_index);
}
