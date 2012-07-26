
varying vec2 texc;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform float cross_fader;

void main() {
  
  vec4 col0 = texture2DRect(tex0, texc);
	vec4 col1 = texture2DRect(tex1, texc);
  
  gl_FragColor = mix(col0, col1, cross_fader);
  
}