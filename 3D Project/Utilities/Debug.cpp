#include "pch.h"

Debug::Debug(Scene * pScene):m_pScene(pScene)
{
	pShader = gResources()->GetShader("Debug");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*2, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*)0);
}

Debug::~Debug()
{
}



void Debug::DrawLine(const vec3 & from, const vec3 & to, const vec3 & color, const mat4& m )
{
	
	DebugData db;
	db.color = color;
	db.data[0] = from;
	db.data[1] = to;
	db.MVP = m_pScene->GetViewProj()*m;
	m_Lists.push_back(db);
}

void Debug::Render()
{
	static Line l;
	pShader->Use();
	//glBindVertexArray(VAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	for (int i = 0; i < m_Lists.size(); i++)
	{
		DebugData& db = m_Lists[i];
		pShader->SetUniform("color", db.color);
		pShader->SetUniformMatrix("MVP", glm::value_ptr(db.MVP));
		//glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(vec3), db.data);
		//glDrawArrays(GL_LINES, 0, 2);
		l.Draw(db.data[0], db.data[1]);
	}
	m_Lists.clear();
}

