#version 330
// neu co bien khong dung toi thi trinh bien dich se tu dong xoa khi bien dich.
in vec3 aPos;	// location = 0
in vec3 aNormal;		// location = 1
in vec2 aUV;			// location = 2, neu normal khong duoc dung toi no se bi xoa va location cua uv se la 1
in vec2 aBlend1;			// x: bone y:weight
in vec2 aBlend2;			// x: bone y:weight
in vec2 aBlend3;			// x: bone y:weight
in vec2 aBlend4;			// x: bone y:weight


uniform mat4 uMVP;
uniform mat4 uModel;
//uniform mat4 uTransform[70];

//out vec3 oNormal;
//out vec2 oUV;
//out vec3 oFragPos;
//out vec3 oPosition;
out VS_OUT
{
    vec2 oUV;
    vec3 oNormal;
    vec3 oPosition;
}vs_out;


void main()
{
	vec4 pos = vec4(aPos,1.0f);

	// vec4 finalPos = vec4(0);
	// vec3 finalNormal = vec3(0);
	// int b;
	// if((ws1.x<71 )&&( ws1.x>0))
	// {
	// 	b =int(ws1.x);
	// 	finalPos += Transform[b] * pos * vec4(ws1.y) ;
	// 	finalNormal += mat3(Transform[b])* normal * vec3(ws1.y);
	// }

	// if((ws2.x<71 )&& (ws2.x>0))
	// {
	// 	b = int(ws2.x);
	// 	finalPos +=Transform[b] * pos * vec4(ws2.y) ;
	// 	finalNormal += mat3( Transform[b])* normal * vec3(ws2.y);
	// }

	// if((ws3.x<71) && (ws3.x>0))
	// {
	// 	b =int(ws3.x);
	// 	finalPos += Transform[b] * pos * vec4(ws3.y) ;
	// 	finalNormal += mat3(Transform[b])* normal * vec3(ws3.y);
	// }

	// if((ws4.x<71) &&( ws4.x>0))
	// {
	// 	b =int(ws4.x);
	// 	finalPos += Transform[b] * (pos * vec4(ws4.y) );
	// 	finalNormal +=   mat3(Transform[b])* normal * vec3(ws4.y);
	// }


	gl_Position = uMVP*pos;
	vs_out.oNormal =  mat3(transpose(inverse(uModel))) *aNormal;  
	vs_out.oPosition = vec3(uModel * pos);
	vs_out.oUV = aUV;
	//oFragPos = vec3(uModel * pos);

	
}