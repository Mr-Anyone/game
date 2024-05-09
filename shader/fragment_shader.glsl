#version 330 core 

out vec4 FragColor;
in vec4 objPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 normal; 
in vec3 pos;

void main(){
    float ambientStrength = 0.2f; 
    float specularStrength = 0.5f; 

    vec3 ambient = ambientStrength * lightColor;
    

    float diffuse = max(dot(normalize(lightPos - pos), normal), 0.0);
    //FragColor = vec4(objectColor*(min(ambient + diffuse, 1.0)), 1.0f); 
    FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f); 
}
