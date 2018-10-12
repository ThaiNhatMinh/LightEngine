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
uniform mat4 uTransform[70];
uniform int uNumNode;
out VS_OUT
{
    vec2 oUV;
    vec3 oNormal;
    vec3 oPosition;
}vs_out;


void main()
{
	vec4 pos = vec4(aPos,1.0f);

	vec4 finalPos = vec4(0);
	vec3 finalNormal = vec3(0);
	int b;
	if((aBlend1.x<71 )&&( aBlend1.x>0))
	{
		b =int(aBlend1.x);
		finalPos += uTransform[b] * pos * vec4(aBlend1.y) ;
		finalNormal += mat3(uTransform[b])* aNormal * vec3(aBlend1.y);
	}

	if((aBlend2.x<71 )&& (aBlend2.x>0))
	{
		b = int(aBlend2.x);
		finalPos +=uTransform[b] * pos * vec4(aBlend2.y) ;
		finalNormal += mat3( uTransform[b])* aNormal * vec3(aBlend2.y);
	}

	if((aBlend3.x<71) && (aBlend3.x>0))
	{
		b =int(aBlend3.x);
		finalPos += uTransform[b] * pos * vec4(aBlend3.y) ;
		finalNormal += mat3(uTransform[b])* aNormal * vec3(aBlend3.y);
	}

	if((aBlend4.x<71) &&( aBlend4.x>0))
	{
		b =int(aBlend4.x);
		finalPos += uTransform[b] * (pos * vec4(aBlend4.y) );
		finalNormal +=   mat3(uTransform[b])* aNormal * vec3(aBlend4.y);
	}


	gl_Position = uMVP*finalPos;
	vs_out.oNormal =  mat3(transpose(inverse(uModel))) *finalNormal;  
	vs_out.oPosition = vec3(uModel * finalPos);
	vs_out.oUV = aUV;
	//oFragPos = vec3(uModel * pos);

	
}