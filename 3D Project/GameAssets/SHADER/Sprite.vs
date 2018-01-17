#version 130

in vec3 pos;
out vec2 vUV;

uniform vec3 SpritePos;
uniform vec3 CameraUp;
uniform vec3 CameraRight;
uniform vec2 SpriteSize;
uniform mat4 MVP;

void main()
{	
	vec3 vertexCenter = SpritePos + CameraRight*SpriteSize.x * pos.x + CameraUp*SpriteSize.y* pos.y;
	
	// Output position of the vertex
	gl_Position = MVP * vec4(vertexCenter,1.0f);
	
	// UV of the vertex. No special space for this one.
	vUV = pos.xy + vec2(0.5, 0.5);
	
}