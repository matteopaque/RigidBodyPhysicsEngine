#version 330 core
layout (location = 0) in vec3 Position;
out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main () {
    gl_Position = projection * view * model * vec4(Position, 1.0);
   // gl_Position = vec4(0.f, 0.f, 0.f, 0.f);
    //color = vec4(Position, 1.0);
    color = vec4(Position, 1.0);
}