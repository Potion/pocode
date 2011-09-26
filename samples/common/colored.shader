[[uniforms]]
uniform mat4 mvp;
uniform vec4 color;

[[varyings]]

[[vertex]]
attribute vec4 position;

void main() {
	gl_Position = mvp * position;
}

[[fragment]]
void main() {
	gl_FragColor = color;
}
