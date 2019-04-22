#version 400 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D tex;
void main()
{
    if(gl_FrontFacing){
        FragColor = texture(tex,texCoord);
    }else{
        FragColor = texture(tex,texCoord);
        //FragColor=vec4(0.0f,1.0f,0.0f,1.0f);
    }
}
