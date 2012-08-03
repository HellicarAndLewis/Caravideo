#version 120

varying vec2 texc;
uniform sampler2DRect tex;
uniform vec2 dim;

#define PI2 6.283185307179586

uniform int divisions;

void main() {
	

	//set up the two vectors to calculate the angle of the current tex coord
	vec2 a = vec2(dim.x/2.0, dim.y/2.0) - texc;
	vec2 b = vec2(1.0, 0.0);

	float theta = acos(dot(a, b)/(length(a) * length(b)));

	//make the angle between 0 and 2PI, not 0 and PI
	if (texc.y > dim.y/2.0) {
		theta = PI2 - theta;
	}
	
	float segment_angle = PI2 / float(divisions);
	int n_segment = int(theta / segment_angle);
	
	float rotation = float(n_segment) * segment_angle;
	
	//if we are on every other segment then mirror
	if (n_segment % 2 == 1) {
		rotation = ((n_segment-1) * segment_angle) + (2 * (theta - rotation));
	}
	
	mat2 rotmat = mat2(cos(rotation), sin(rotation), -sin(rotation), cos(rotation));

	//translate to origin then rotate and translate back to get the wanted texel
	vec2 offset = vec2(0.5, 0.5);
	vec2 p = texc - (offset * dim);
	p*= rotmat;
	p+= (offset * dim);
	
	vec4 c = texture2DRect(tex, p);
	
	gl_FragColor = c;
}