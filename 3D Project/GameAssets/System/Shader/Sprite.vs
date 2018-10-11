#version 130

in vec3 aPos;
out vec2 oUV;

uniform vec3 SpritePos;
uniform vec3 CameraUp;
uniform vec3 CameraRight;
uniform vec2 SpriteSize;
uniform mat4 MVP;

void main()
{	
	vec3 vertexCenter = SpritePos + CameraRight*SpriteSize.x * aPos.x + CameraUp*SpriteSize.y* aPos.y;
	
	// Output position of the vertex
	gl_Position = MVP * vec4(vertexCenter,1.0f);
	
	// UV of the vertex. No special space for this one.
	oUV = aPos.xy + vec2(0.5, 0.5);
	
}