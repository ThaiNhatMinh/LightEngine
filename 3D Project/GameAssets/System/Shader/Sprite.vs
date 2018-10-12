#version 130

in vec3 aPos;
out vec2 oUV;

uniform vec3 uSpritePos;
uniform vec3 uCameraRight;
uniform vec3 uCameraUp;
uniform vec2 uSpriteSize;
uniform mat4 uMVP;

void main()
{	
	vec3 vertexCenter = uSpritePos + uCameraRight*uSpriteSize.x * aPos.x + uCameraUp*uSpriteSize.y* aPos.y;
	
	// Output position of the vertex
	gl_Position = uMVP * vec4(vertexCenter,1.0f);
	
	// UV of the vertex. No special space for this one.
	oUV = aPos.xy + vec2(0.5, 0.5);
	
}