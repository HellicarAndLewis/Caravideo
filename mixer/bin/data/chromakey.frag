
varying vec2 texc;
uniform sampler2DRect tex0;

uniform float background_hue;
uniform float max_dist;

//uniform int r;
uniform float bloom_amount;
uniform float bloom_mix;

float getHue(vec4 col);


void main() {

//	int r = 3;
//	float sc = 0.25;
  
  vec4 col = texture2DRect(tex0, texc);
	
//	vec4 sum;
//	
//	int r = 5;
//	for (int i = -r; i < r; i++) {
//		for (int j = -r; j < r; j++) {
//			sum+= texture2DRect(tex0, texc + vec2(i, j)) * bloom_amount;
//		}
//	}
//	vec4 mixed = mix(vec4(0, 0, 0, 0), col * col, bloom_mix);
//	gl_FragColor = sum * mixed;
//	return;
	
  
  float hue = getHue(col);
  
  if (abs(hue - background_hue) < 0.1) {
    col.a = 0.0;
  }
  
  gl_FragColor = col;
  
}

float getHue(vec4 col) {
  float r = col.r;
  float g = col.g;
  float b = col.b;
  
  float hue, max, min;
  
	min = r;
	if (g < min) min = g;
	if (b < min) min = b;
	
	max = r;
	if (g > max) max = g;
	if (b > max) max = b;
	
	float d = max - min;
	
  if (max == r) {
    hue = (g - b) / d;
    if (g < b) {
      hue+= 6.0;
    }
  }
  else if (max == g) {
    hue = ((b - r) / d) + 2.0;
  }
  else if (max == b) {
    hue = ((r - g) / d) + 4.0;
  }
  
  else {
    hue = 0.0;
  }
  
  hue/= 6.0;
  return hue;
}