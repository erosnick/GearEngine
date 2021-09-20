layout(std140, set = 0, binding = 1) uniform FrameUniforms {
    mat4 view_matrix;
    mat4 main_view_matrix;
    mat4 proj_matrix;
    mat4 sun_matrixs[SHADOW_CASCADE_COUNT];
    vec4 sun_direction;
    vec4 cascade_splits;
} frame_uniforms;

layout(std140, set = 0, binding = 2) uniform ObjectUniforms {
    mat4 model_matrix;
    mat4 normal_matrix;
} object_uniforms;

#if defined(HAS_SHADOWING)
layout(set = 1, binding = 0) uniform sampler2DArray cascade_shadow_map;
#endif