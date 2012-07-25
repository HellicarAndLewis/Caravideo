
varying vec2 texc;
uniform sampler2DRect tex0;

uniform float background_hue;
uniform float max_dist;

float getHue(vec4 col);


void main() {

  
  vec4 col = texture2DRect(tex0, texc);
  
  float hue = getHue(col);
  
  if (abs(hue - background_hue) < 0.1) {
    col.a = 0.0;
  }
  
//  col.a = 0.1;
  gl_FragColor = col;
  
}

float getHue(vec4 col) {
  float r = col.r;
  float g = col.g;
  float b = col.b;
  
  float hue, max, min;
  
  if (col.r > col.g && col.r > col.b) max = r;
  else if (col.g > col.r && col.g > col.b) max = g;
  else if (col.b > col.r && col.b > col.g) max = b;
  
  if (col.r < col.g && col.r < col.b) min = r;
  else if (col.g < col.r && col.g < col.b) min = g;
  else if (col.b < col.r && col.b < col.g) min = b;
  
  if (max == r) {
    hue = (g - b) / (max - min);
    if (g < b) {
      hue+= 6.0;
    }
  }
  else if (max == g) {
    hue = ((b - r) / (max - min)) + 2.0;
  }
  else if (max == b) {
    hue = ((r - g) / (max - min)) + 4.0;
  }
  
  else {
    hue = 0.0;
  }
  
  hue/= 6.0;
  return hue;
}