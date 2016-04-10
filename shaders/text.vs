#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 model;

uniform vec2 size;
uniform vec2 imgSize;
uniform vec2 offset;

void main() {
	gl_Position = projection * model * vec4(vertex.xy, 0.0f, 1.0f);

	TexCoords = vertex.zw;
	TexCoords.x = (vertex.z * (size.x / imgSize.x) + (offset.x / imgSize.x));
	TexCoords.y = (vertex.w * (size.y / imgSize.y) + (offset.y / imgSize.y));
}