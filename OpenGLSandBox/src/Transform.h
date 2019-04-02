#pragma once
#include "Camera.h"
#include "math\Matrix4f.h"
#include "math\Vector3f.h"
class Transform {
 public:
  Transform() : m_Scale({1, 1, 1}) {}
  ~Transform() {}
  inline void SetScale(const vec3& scale) { m_Scale = scale; }
  inline void SetRotate(const vec3& rotate) { m_Rotate = rotate; }
  inline void SetTranslate(const vec3& translate) { m_Translate = translate; }
  inline void SetPerspectiveProj(const float znear, const float zfar,
                                 const float width, const float height,
                                 const float FOV) {
    m_Perspective.makePerspectiveProjection(znear, zfar, width, height, FOV);
  }
  inline void SetCamera(const vec3& origin,  const vec3& target, const vec3& up) {
	  m_Camera.m_Origin = origin;
	  m_Camera.m_Target = target;
	  m_Camera.m_Up = up;
  }
  Matrix4f GetMVPTransform() {
    Matrix4f Scale, Rotate, Translate, Camera, Transform;

    Translate.makeTranslate(m_Translate);
    Rotate.makeRotateTransform(m_Rotate);
    Scale.makeScale(m_Scale);
    Camera.makeCameraTransform(m_Camera.m_Origin, m_Camera.m_Target,
                               m_Camera.m_Up);
    Transform = m_Perspective * Camera * Translate * Rotate * Scale;
    return Transform;
  }

  Matrix4f GetWorldTransform() {
	  Matrix4f Scale, Rotate, Translate, Camera, Transform;

	  Translate.makeTranslate(m_Translate);
	  Rotate.makeRotateTransform(m_Rotate);
	  Scale.makeScale(m_Scale);
	  Camera.makeCameraTransform(m_Camera.m_Origin, m_Camera.m_Target,
		  m_Camera.m_Up);
	  Transform =   Translate * Rotate * Scale;
	  return Transform;
  }

 private:
  vec3 m_Translate;
  vec3 m_Rotate;
  vec3 m_Scale;
  struct CamInfo {
    vec3 m_Origin;
    vec3 m_Target;
    vec3 m_Up;
  } m_Camera;
  Matrix4f m_Perspective;
};
