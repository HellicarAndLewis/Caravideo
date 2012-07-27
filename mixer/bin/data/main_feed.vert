
varying vec2 texc;

void main() {

  gl_Position = ftransform();
  
  texc = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
}