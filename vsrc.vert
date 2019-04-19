#version 400 core
layout (location = 0) in vec3 aPos;
uniform mat4 matrix;
void main()
{
    gl_Position = matrix * vec4(aPos, 1.0);
    //gl_Position = vec4(aPos, 1.0);
}
