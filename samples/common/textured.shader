[[uniforms]]
uniform mat4 mvp;
uniform vec4 color;
uniform sampler2D tex;

[[varyings]]
varying vec2 texCoord;

[[vertex]]
attribute vec4 position;
attribute lowp vec2 textureCoordinates;

void main() {
	texCoord = textureCoordinates;
	gl_Position = mvp * position;
}

[[fragment]]
void main() {
	gl_FragColor = texture2D(tex,texCoord) * color;
}
