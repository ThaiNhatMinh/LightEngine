#include "..\pch.h"



void Camera::UpdateVector()
{

	vec3 front;
	front.x = cos(Math::ToRadian(m_Yaw)) * cos(Math::ToRadian(m_Pitch));
	front.y = sin(Math::ToRadian(m_Pitch));
	front.z = sin(Math::ToRadian(m_Yaw)) * cos(Math::ToRadian(m_Pitch));

	

	m_Front = Math::Normalize(front);
	m_Right = Math::Normalize(Math::Cross(m_Front, WorldUp));
	m_Up = Math::Normalize(Math::Cross(m_Right, m_Front));
}

Camera::Camera()
{
	m_Position = vec3(0, 0, 200);
	m_Front = Math::Normalize(vec3(0, 0, 0) - m_Position);
	WorldUp = vec3(0, 1, 0);
	m_Speed = 50.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	MouseSensitivity = 0.25;
	UpdateVector();
}

Camera::Camera(const vec3 & pos, const vec3 & target, const vec3 & up)
{
	m_Position = pos;
	m_Front = Math::Normalize(target - m_Position);;
	WorldUp = up;
	m_Speed = 50.0f;
	m_Pitch = 0;
	m_Yaw = -90;
	MouseSensitivity = 0.25;
	UpdateVector();
}


Camera::~Camera()
{
}
/*
void Camera::OnKeyboard(int key,float deltaTIme)
{
	if (key == GLFW_KEY_W) m_Position +=  m_Front*m_Speed*deltaTIme;
	if (key == GLFW_KEY_S) m_Position -= m_Front*m_Speed*deltaTIme;
	if (key == GLFW_KEY_D) m_Position += m_Right*m_Speed*deltaTIme;
	if (key == GLFW_KEY_A) m_Position -= m_Right*m_Speed*deltaTIme;

}
*/
void Camera::Update(float deltaTIme)
{
	// Camera controls
	if (gInput()->OnKey(GLFW_KEY_W)) m_Position += m_Front*m_Speed*deltaTIme;
	if (gInput()->OnKey(GLFW_KEY_S)) m_Position -= m_Front*m_Speed*deltaTIme;
	if (gInput()->OnKey(GLFW_KEY_D)) m_Position += m_Right*m_Speed*deltaTIme;
	if (gInput()->OnKey(GLFW_KEY_A)) m_Position -= m_Right*m_Speed*deltaTIme;
	//cout << m_Position << endl;
}

void Camera::OnMouse(float dx, float dy)
{
	m_Pitch += dy*MouseSensitivity;
	m_Yaw += dx*MouseSensitivity;
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	UpdateVector();
}

mat4 Camera::GetViewMatrix()
{
	mat4 view = Math::LookAt(m_Position, m_Position + m_Front, m_Up);
	return view;
}
