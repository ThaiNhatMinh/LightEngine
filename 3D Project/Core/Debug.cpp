#include "pch.h"


Debug::Debug()
{
}

void Debug::Init(Context * c)
{
	pShader = c->m_pResources->GetShader("Debug");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 2, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
	c->m_pDebuger = std::unique_ptr<Debug>(this);
}

void Debug::ShutDown()
{
}

void Debug::SetView(CameraComponent * cam)
{
	m_View = cam->GetViewMatrix();
	m_Proj = cam->GetProjMatrix();
	VP = cam->GetVPMatrix();
}

Debug::~Debug()
{
}



void Debug::DrawLine(const vec3 & from, const vec3 & to, const vec3 & color, const mat4& m )
{
	
	DebugData db;
	db.color = color;
	db.pos[0] = from;
	db.pos[1] = to;
	m_Lists.push_back(db);
}

void Debug::Render()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pShader->Use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	pShader->SetUniformMatrix("MVP", glm::value_ptr(VP));
	for (size_t i = 0; i < m_Lists.size(); i++)
	{
		DebugData& db = m_Lists[i];
		pShader->SetUniform("color", db.color);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(vec3), db.pos);
		glDrawArrays(GL_LINES, 0, 2);
		
	}
	m_Lists.clear();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

