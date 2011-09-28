[[uniforms]]
uniform mat4 mvp;
uniform vec4 color;
uniform sampler2D tex;
uniform int isAlphaMask;

[[varyings]]
varying vec2 texCoord;

[[vertex]]
attribute vec4 position;
attribute vec2 textureCoordinates;

void main() {
	texCoord = textureCoordinates;
	gl_Position = mvp * position;
}

[[fragment]]
void main() {
	vec4 texColor = texture2D(tex, texCoord);
	gl_FragColor = texColor * color;

	if(bool(isAlphaMask)) {
		gl_FragColor = vec4(1.0,1.0,1.0,texColor.a) * color;
	}
}
