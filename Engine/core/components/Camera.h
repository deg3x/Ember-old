#pragma once

#include "Component.h"

enum class CameraProjection : uint8_t
{
	PERSPECTIVE  = 0x00,
	ORTHOGRAPHIC = 0x01
};

class ENGINE_API Camera : public Component
{
	MAKE_COMPONENT_TYPE(CAMERA)

protected:
	float perspectiveFOV;
	float orthographicSize;
	float nearClipping;
	float farClipping;
	CameraProjection projectionType;
	
public:
	Camera();

	glm::mat4x4 GetViewMatrix() const;
	glm::mat4x4 GetProjectionMatrix() const;

	inline void SetPerspectiveFOV(float newFOV)
	{
		perspectiveFOV = newFOV;
	}

	inline void SetOrthographicSize(float newOrthographicSize)
	{
		orthographicSize = newOrthographicSize;
	}

	inline void SetNearClipping(float newNearClipping)
	{
		nearClipping = newNearClipping;
	}

	inline void SetFarClipping(float newFarClipping)
	{
		farClipping = newFarClipping;
	}

	inline void SetProjectionType(CameraProjection newProjectionType)
	{
		projectionType = newProjectionType;
	}

	inline float GetPerspectiveFOV() const
	{
		return perspectiveFOV;
	}

	inline float GetOrthographicSize() const
	{
		return orthographicSize;
	}

	inline float GetNearClipping() const
	{
		return nearClipping;
	}

	inline float GetFarClipping() const
	{
		return farClipping;
	}

	inline CameraProjection GetProjectionType() const
	{
		return projectionType;
	}
};
