#version 330 core
layout(location = 0) in vec3  aPosition;
layout(location = 1) in float aProbability;  // novo

uniform mat4 uView;
uniform mat4 uProjection;
uniform float uMaxProb;  // normaliza o colormap por orbital

out float vProbability;

void main() {
    vec4 viewPos = uView * vec4(aPosition, 1.0);
    gl_Position  = uProjection * viewPos;
    gl_PointSize = 2.0;
    vProbability = aProbability / max(uMaxProb, 1e-10);  // [0, 1]
}