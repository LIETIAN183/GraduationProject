#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vTexCoord;
out vec2 tcs_texCoord;
uniform mat4 matrix;
void main()
{
    //matrix
    gl_Position = vec4(aPos, 1.0);
    tcs_texCoord = vTexCoord;
}
