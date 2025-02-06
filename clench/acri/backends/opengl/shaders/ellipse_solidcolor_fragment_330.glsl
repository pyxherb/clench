#version 330 core
out vec4 FragColor;

in vec4 vertexColor;

layout (std140) uniform RenderInfo {
	vec2 resolution;
	vec2 offset;
	vec2 origin;
	vec2 radius;
} u_renderInfo;

void main() {
	vec2 pos = (gl_FragCoord.xy - u_renderInfo.offset) / u_renderInfo.resolution;
	pos -= 0.5f;
	pos *= 2.0f;

	float a = pos.x - u_renderInfo.origin.x;
	float b = pos.y - u_renderInfo.origin.y;

	float result = a * a / (u_renderInfo.radius.x * u_renderInfo.radius.x) +
				b * b / (u_renderInfo.radius.y * u_renderInfo.radius.y);

	if(result <= 1.0f) {
		FragColor = vertexColor;
	} else {
		FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
	}
}
