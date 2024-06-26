#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal; 


uniform mat4 projection;
uniform mat4 view; 

out vec3 pos;
out vec3 normal;

void main(){
    gl_Position =  projection* view *  vec4(aPos.x, aPos.y, aPos.z, 1.0);

    pos = aPos;
    normal = aNormal;
}
