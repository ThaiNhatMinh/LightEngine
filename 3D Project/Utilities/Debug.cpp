#include "pch.h"

Debug::Debug(Scene * pScene):m_pScene(pScene)
{
	pShader = gResources()->GetShader("Debug");
}

Debug::~Debug()
{
}



void Debug::DrawLine(const vec3 & from, const vec3 & to, const vec3 & color, const mat4& m )const
{
	
	pShader->Use();
	pShader->SetUniform("color", color);
	pShader->SetUniformMatrix("MVP", glm::value_ptr(m_pScene->GetViewProj()));
	pShader->SetUniformMatrix("Model", glm::value_ptr(m));
	m_Line.Draw(from, to);
}

