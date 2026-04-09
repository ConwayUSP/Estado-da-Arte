#version 430 core
out vec4 FragColor;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
    FragColor = vec4(lightColor * objectColor, 1.0);
    lightningShader.use();
    lightningShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightningShader.setVec3("lightColor", 1.0f, 1.00f, 1.00f);
}
