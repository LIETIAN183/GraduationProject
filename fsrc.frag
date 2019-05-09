#version 400 core

in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform mat4 frag_view;
uniform mat4 frag_model;
uniform sampler2D tex;
uniform vec3 lightColor;
uniform bool IsLight;
void main()
{
    //objectColor
    vec4 objectColor = texture(tex,texCoord);
    //ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    //diffuse
    //固定光源
    vec3 fNormal = mat3(transpose(inverse(frag_view * frag_model))) * Normal;
    vec3 norm = normalize(fNormal);
    vec3 LightPos = vec3(frag_view * vec4(lightPos, 1.0));
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular---Blinn-Phong模型
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfDir, norm), 0.0), 32.0);
    //Phong模型
    //vec3 reflectDir = normalize(reflect(-lightDir, norm));
    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0);
    vec3 specular = specularStrength * spec * lightColor;

    //result
    vec3 result = (ambient + diffuse + specular)* vec3(objectColor);

    if(IsLight){
        FragColor = vec4(result, 1.0);
    }else{
        FragColor = objectColor;
    }

}
