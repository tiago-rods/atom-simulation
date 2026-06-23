#version 330 core

in float vDepth;
out vec4 fragColor;

void main() {
    // Simple white particles for Sprint 1 smoke test
    fragColor = vec4(1.0, 1.0, 1.0, 0.8);
}
