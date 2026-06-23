#version 330 core

layout(location = 0) in vec3 aPosition;

uniform mat4 uView;
uniform mat4 uProjection;

out float vDepth;

void main() {
    vec4 viewPos = uView * vec4(aPosition, 1.0);
    gl_Position  = uProjection * viewPos;
    gl_PointSize = 2.0;
    vDepth = -viewPos.z;
}
