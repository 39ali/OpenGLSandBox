#pragma once
#include "math/Vector3f.h"
#include <GLFW/glfw3.h>
#include "math/Quaternion.h"
class Camera {
public :
	Camera():m_Origin(0,0,-3), m_Target(0,0,1), m_Up(0,1,0) {
	
	}
	Camera(const vec3& origin ,const vec3& target, const vec3& up,int width, int height) 
		:m_Origin(origin),m_Target(target),m_Up(up)
	{
		Init(width,height);
	}

	
	void Update( GLFWwindow* window , float timeStep) {
		
		if (glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS) {
			m_Origin += (m_Target)*timeStep*m_Speed;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			m_Origin -= (m_Target)*timeStep*m_Speed;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			
			vec3 left = m_Target.cross(m_Up);
			left=left.normalize()*timeStep*m_Speed;
			m_Origin += (left);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

			vec3 right = m_Up.cross(m_Target);
			right = right.normalize()*timeStep*m_Speed;
			m_Origin += (right);
		}

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		const float deltaX = xpos - m_MousePos.x;
		const float deltaY = ypos - m_MousePos.y;
		m_MousePos.x = xpos;
		m_MousePos.y = ypos;
		m_AngleH += (float)deltaX/5;
		m_AngleV += (float)deltaY/5;
		m_AngleV = std::fmax(-90.f, std::fmin(m_AngleV, 90.f));
		const vec3 V(0.0f, 1.0f, 0.0f);

		vec3 target(1.0f, 0.0, 0.0f);
		target =Quaternion::Rotate(m_AngleH, V, target).normalize();

		vec3 H = V.cross(target).normalize();
		target = Quaternion::Rotate(m_AngleV, H, target).normalize();

		m_Target = target;
		m_Up = m_Target.cross(H);
	}
private :
	void Init(int width , int height) {
	
		vec3 hTarget(m_Target.x ,0.0f,m_Target.z);
		hTarget = hTarget.normalize();

		if (hTarget.z >= 0.0f) {
		
			if (hTarget.x >= 0.0f) {
			
				m_AngleH = 360.0f - toDegrees(asin(hTarget.z));
	
			}
			else {
				m_AngleH = 180.0f + toDegrees(asin(hTarget.z));
			}
		}
		else {
			if (hTarget.x >= 0.0f)
				m_AngleH = toDegrees(asin(-hTarget.z));
			else
				m_AngleH = 180.f-toDegrees(asin(-hTarget.z));
		}

		m_AngleV = -toDegrees(asin(hTarget.y));

		
		m_MousePos.x = width / 2;
		m_MousePos.y = height / 2;
	}

public :
	vec3 m_Origin;
	vec3 m_Target;
	vec3 m_Up;
	float m_AngleH;
	float m_AngleV;
private:
	float m_Speed = 25.5;
	vec3 m_MousePos;
};