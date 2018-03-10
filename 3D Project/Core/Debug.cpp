#include "pch.h"


Debug::Debug(Context* c)
{
	pShader = c->GetSystem<Resources>()->GetShader("Debug");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * 2, NULL, GL_STREAM_DRAW);
	glEnableVertexAttribArray(SHADER_POSITION_ATTRIBUTE);
	glVertexAttribPointer(SHADER_POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(DebugData), (GLvoid*)0);
	glEnableVertexAttribArray(SHADER_COLOR_ATTRIBUTE);
	glVertexAttribPointer(SHADER_COLOR_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, sizeof(DebugData), (GLvoid*)(sizeof(vec3)));
	c->AddSystem(this);
	glBindVertexArray(0);
}

void Debug::Update()
{
	//VP = Camera::GetCurrentCamera()->GetVPMatrix();
}

Debug::~Debug()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}



void Debug::DrawLine(const vec3 & from, const vec3 & to, const vec3 & color, const mat4& m )
{
	
	DebugData db;
	db.color = color;
	db.pos = m*vec4(from,1.0f);
	m_Lists.push_back(db);
	db.pos = m*vec4(to,1.0f);
	m_Lists.push_back(db);
}



void Debug::DrawLineBox(vec3 min, vec3 max, vec3 color, const mat4& m)
{
	DrawLine(vec3(min.x, min.y, min.z), vec3(max.x, min.y, min.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(max.x, min.y, min.z), vec3(max.x, min.y, max.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(max.x, min.y, max.z), vec3(min.x, min.y, max.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(min.x, min.y, max.z), vec3(min.x, min.y, min.z), vec3(0.5f, 1.0f, 0.7f));

	DrawLine(vec3(min.x, max.y, min.z), vec3(max.x, max.y, min.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(max.x, max.y, min.z), vec3(max.x, max.y, max.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(max.x, max.y, max.z), vec3(min.x, max.y, max.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(min.x, max.y, max.z), vec3(min.x, max.y, min.z), vec3(0.5f, 1.0f, 0.7f));

	DrawLine(vec3(min.x, max.y, min.z), vec3(min.x, min.y, min.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(max.x, max.y, min.z), vec3(max.x, min.y, min.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(max.x, max.y, max.z), vec3(max.x, min.y, max.z), vec3(0.5f, 1.0f, 0.7f));
	DrawLine(vec3(min.x, max.y, max.z), vec3(min.x, min.y, max.z), vec3(0.5f, 1.0f, 0.7f));
}

void Debug::DrawCoord(const mat4 & m)
{
	vec3 pos = m[3];
	vec3 x = m[0];
	vec3 y = m[1];
	vec3 z = m[2];
	DrawLine(pos, pos+x*10.0f, vec3(1, 0, 0));
	DrawLine(pos, pos+y*10.0f, vec3(0, 1, 0));
	DrawLine(pos, pos+z*10.0f, vec3(0, 0,1));
}

void Debug::Render()
{
	if (m_Lists.empty()) return;

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	pShader->Use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(DebugData) * m_Lists.size(), &m_Lists[0], GL_STREAM_DRAW);
	pShader->SetUniformMatrix("MVP", glm::value_ptr(VP));
	glDrawArrays(GL_LINES, 0, m_Lists.size());
		
	m_Lists.clear();
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

