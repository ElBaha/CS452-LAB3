 #version 150

in vec3 position;
in vec3 startColor;
out vec3 vertexColor;
/*in vec2 texcoord;*/
/*out vec2 Texcoord;*/

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = (projection * view * model * vec4(position.x, position.y, position.z, 1.0));
	/*Actual order is M*V*P, but GLSL orders from right.*/
	vertexColor = startColor;
	/*Texcoord = texcoord;*/
}
