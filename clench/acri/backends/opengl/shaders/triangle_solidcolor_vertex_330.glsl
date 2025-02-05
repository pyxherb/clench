#version 330 core
layout (location = 0) in vec4 color;
layout (location = 1) in vec2 position;

uniform u_testColor {
	vec4 testColor;
};

out vec4 vertexColor;

void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	vertexColor = color * u_TestColor.testColor;
}
