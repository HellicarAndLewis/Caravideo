
varying vec2 texc;
uniform sampler2DRect tex0;

uniform float background_hue;
uniform float max_dist;

//bloom
uniform bool do_bloom;
uniform float bloom_amount;
uniform float bloom_mix;

//chromatic aberration
uniform bool do_chab;
uniform float chab_amount;

//fisheye
//uniform float lensradius;
uniform float signcurvature;


float getHue(vec4 col);


void main() {

  
  vec4 col = texture2DRect(tex0, texc);
	
	//first perform background subtraction
  float hue = getHue(col);
  
  if (abs(hue - background_hue) < 0.1) {
    col.a = 0.0;
		gl_FragColor = col;
		return;
  }

	//we are now only dealing with the foreground
	
	if (do_chab) {
		vec2 offset = vec2(1, 0) * chab_amount;
		col.g = texture2DRect(tex0, texc + offset).g;
		col.b = texture2DRect(tex0, texc + offset * 2.0).b;
	}
	
	//do bloom
	if (do_bloom) {
		vec4 sum;
		
		int r = 4;
		for (int i = -r; i < r; i++) {
			for (int j = -r; j < r; j++) {
				sum+= texture2DRect(tex0, texc + vec2(i, j)) * bloom_amount;
			}
		}
		vec4 mix_factor = mix(vec4(0, 0, 0, 0), col * col, bloom_mix);
		
		col = sum * mix_factor;// * col.a;
	}
	
	
	float lensradius = 2.0;
	vec2 texd = vec2(640.0, 480.0);
	
	float curvature = abs(signcurvature);
	float extent = lensradius;
	float optics = extent / log2(curvature * extent + 1.0) / 1.4427;
	vec2 normalizeTD = texc/texd;
	vec2 PP = normalizeTD - vec2(0.5,0.5);
	float P0 = PP[0];
	float P1 = PP[1];
	float radius = sqrt(P0 * P0 + P1 * P1);
	
	float cosangle = P0 / radius;
	float sinangle = P1 / radius;
	
	float rad1, rad2, newradius;
	rad1 = (exp2((radius / optics) * 1.4427) - 1.0) / curvature;
	rad2 = optics * log2(1.0 + curvature * radius) / 1.4427;
	newradius = signcurvature > 0.0 ? rad1 : rad2;
	
	vec2 FE = vec2(newradius * cosangle + 0.5,newradius * sinangle + 0.5);
	FE = radius <= extent ? FE : normalizeTD;
	//    FE = curvature < EPSILON ? normalizeTD : FE;
	
	gl_FragColor = col + texture2DRect(tex0, FE*texd);
	
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