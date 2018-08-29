#version 140
// neu co bien khong dung toi thi trinh bien dich se tu dong xoa khi bien dich.
in vec3 aPos;	// location = 0
in vec3 aNormal;		// location = 1
in vec2 aUV;			// location = 2, neu normal khong duoc dung toi no se bi xoa va location cua uv se la 1
in vec2 aBlend1;			// x: bone y:weight
in vec2 aBlend2;			// x: bone y:weight
in vec2 aBlend3;			// x: bone y:weight
in vec2 aBlend4;			// x: bone y:weight


uniform mat4 uMVP;
void main()
{
    
    gl_Position = uMVP*vec4(aPos + aNormal*0.2f,1.0f);
}