
varying vec2 texc;
uniform sampler2DRect tex0;

uniform float scale_factor;
uniform vec2 inputSize;

void main() {


  
  vec2 anchor = vec2(0.5, 0.5);
  vec2 size = inputSize;
  mat2 scale = mat2(scale_factor, 0.0, 0.0, scale_factor);
  
	//transform point into centre and then scale
  vec2 p = texc - anchor * inputSize;
  p *= scale;
  p+= (anchor * inputSize);
  
	//this is where the magic happens...
	//if scale_factor is 5 we will get 5 tiles
  vec2 m = mod(p, inputSize);
  
  vec4 col = texture2DRect(tex0, m);
  
  gl_FragColor = col;
  
}