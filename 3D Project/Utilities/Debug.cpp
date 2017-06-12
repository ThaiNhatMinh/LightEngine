#include "..\pch.h"

Debug::Debug(Scene * pScene):m_pScene(pScene)
{
}

Debug::~Debug()
{
}

void Debug::onStartUp()
{
	m_Line = new Line;
}

void Debug::onShutDown()
{
	delete m_Line;
}

void Debug::DrawLine(const vec3 & from, const vec3 & to, const vec3 & color)
{
	Shader* pShader = gResources()->GetShader("Debug");
	pShader->Use();
	pShader->SetUniform("color", color);
	pShader->SetUniformMatrix("MVP", m_pScene->GetViewProj().ToFloatPtr());

	m_Line->Draw(from, to);
}

Debug * gDebug()
{
	return Debug::InstancePtr();
}
