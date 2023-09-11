#version 330 core

uniform sampler2D textureIn;

in vec2 textureCoord;
in vec4 color;
out vec4 colorOut;

void main() {
	colorOut = texture(textureIn, textureCoord) * color;
}
