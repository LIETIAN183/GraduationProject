#version 400 core

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform mat4 frag_view;
uniform sampler2D tex;
uniform vec3 lightColor;
void main()
{
    //objectColor
    vec4 objectColor = texture(tex,texCoord);
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 LightPos = vec3(frag_view * vec4(lightPos, 1.0));
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-FragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    //result
    vec3 result = (ambient + diffuse + specular)* vec3(objectColor);
    FragColor = vec4(result, 1.0);


    /*
    if(gl_FrontFacing){
        FragColor = texture(tex,texCoord);
    }else{
        FragColor = texture(tex,texCoord);
    }*/
    //FragColor = texture(tex,texCoord);
}
