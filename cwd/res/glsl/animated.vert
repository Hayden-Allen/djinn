#version 410 core
TYPE(ANIMATED);
uniform mat4 d_vp, d_view;
out vec3 v_norm;

void main()
{
    mat4 bones = (
        d_instance.d_bones[d_index.x] * d_weight.x +
        d_instance.d_bones[d_index.y] * d_weight.y +
        d_instance.d_bones[d_index.z] * d_weight.z +
        d_instance.d_bones[d_index.w] * d_weight.w
    );
	mat4 transform = d_instance.d_model * bones;
    gl_Position = d_vp * transform * vec4(d_pos, 1);
    
    // WARNING: may cause serious issues!!!!!!!!!!!!!!!!!
    // make sure to check this if anything looks weird
    // if you forgot to check this, well, um, i guess this isn't a sufficient way to document this potential problem
    mat3 normal = mat3(transform);
    v_norm = normal * d_norm;
}
