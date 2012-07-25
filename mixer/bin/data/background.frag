
varying vec2 texc;
uniform sampler2DRect tex0;

uniform float scale_factor;
uniform vec2 inputSize;

void main() {


  
  vec2 anchor = vec2(0.5, 0.5);
  vec2 size = inputSize;
  mat2 scale = mat2(scale_factor, 0.0, 0.0, scale_factor);
  

  vec2 p = texc - anchor * inputSize;
  p *= scale;
  p+= (anchor * inputSize);
//  vec2 nnn = nn + anchor * inputSize;
  
  vec2 m = mod(p, inputSize);
  
  vec4 col = texture2DRect(tex0, m);
  
  gl_FragColor = col;
  

//  vec2 p =  gl_TexCoord[0].st * t;
//  gl_FragColor = texture2DRect(tex0, p); 


}