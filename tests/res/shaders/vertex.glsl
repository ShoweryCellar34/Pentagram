#version 330 core
layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, 1.0, 1.0);
	TexCoord = (aPos + 1) / 2;
}
