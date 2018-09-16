#include "pch.h"
#include "PointLight.h"
#include "..\Material.h"

void Light::render::PointLight::SetupParam(LightParam * param)
{
	(*param)["Ia"]->SetAsVec3(glm::value_ptr(Ia));
	(*param)["Id"]->SetAsVec3(glm::value_ptr(Id));
	(*param)["Is"]->SetAsVec3(glm::value_ptr(Is));
	(*param)["Position"]->SetAsVec3(glm::value_ptr(Pos));
	(*param)["Constant"]->SetAsFloat(constant);
	(*param)["Linear"]->SetAsFloat(linear);
	(*param)["Quadratic"]->SetAsFloat(quadratic);
}
