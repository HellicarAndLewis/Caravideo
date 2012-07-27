
varying vec2 texc;
varying vec2 texd;

void main() {

  gl_Position = ftransform();
  
  texc = vec2(gl_TextureMatrix[0] * gl_MultiTexCoord0);
	
	texd = vec2 (abs(gl_TextureMatrix[0][0][0]),abs(gl_TextureMatrix[0][1][1]));

}