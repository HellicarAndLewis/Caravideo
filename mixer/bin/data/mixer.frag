
varying vec2 texc;
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;

uniform bool do_mix;
uniform float cross_fader;

uniform bool do_slide;
uniform float slide_amount;
uniform bool slide_which;

void main() {
  
	vec4 col = vec4(0, 0, 0, 1);
	
  vec4 col0 = texture2DRect(tex0, texc);
	vec4 col1 = texture2DRect(tex1, texc);
  
	if (do_slide) {
		if(slide_which) col = (slide_amount < texc.y) ? col0 : col1;
		else col = (slide_amount < texc.y) ? col1 : col0;
	}
	
	if (do_mix) {
		col = mix(col0, col1, cross_fader);
	}
	
	
  gl_FragColor = col;
  
}