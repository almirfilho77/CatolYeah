#ifndef __SANDBOX2D_H__
#define __SANDBOX2D_H__
#pragma once

#include "CatolYeah.h"
#include <glm/glm/glm.hpp>

class Sandbox2D : public CatolYeah::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() {}

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnImGuiRender() override;
	virtual void OnUpdate(CatolYeah::Timestep ts) override;
	virtual void OnEvent(CatolYeah::Event& e) override;

private:
	CatolYeah::OrthographicCameraController m_cameraController;

	CatolYeah::Ref<CatolYeah::VertexArray> m_squareVAO = nullptr;
	CatolYeah::Ref<CatolYeah::Texture2D> m_texture = nullptr;


	glm::vec4 m_squareColor = { 0.8f, 0.2f, 0.3f, 1.0f };
	glm::vec4 m_barColor = { 0.2f, 0.8f, 0.3f, 1.0f };
	glm::vec3 m_squarePosition = { 0.0f, 0.0f, 0.0f };
	
	float m_aspectRatio;
	float m_squareTranslationSpeed = 5.0f;
};
#endif //__SANDBOX2D_H__