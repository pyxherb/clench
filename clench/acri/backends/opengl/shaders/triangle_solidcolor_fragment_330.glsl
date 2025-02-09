#version 330 core
out vec4 FragColor;

layout (std140) uniform RenderInfo {
	vec4 color;
} u_renderInfo;

void main() {
	FragColor = u_renderInfo.color;
}
