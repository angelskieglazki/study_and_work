#version 330

// from vertex shader
in vec3 vColor;

// time in seconds with fractional part
uniform float time;

void main() {  
	gl_FragColor = vec4(vColor.x, vColor.y, vColor.z + abs(sin(time)), 1.0);
}
