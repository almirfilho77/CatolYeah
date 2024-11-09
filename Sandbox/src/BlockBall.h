#pragma once
#include <CatolYeah.h>

#include <glm/gtc/matrix_transform.hpp>

namespace BlockBall
{
	class Rect
	{
	public:
		Rect() = default;
		Rect(const glm::vec3 &position, const glm::vec4 &color);

		void SetPosition(float position_y);

		inline CatolYeah::Ref<CatolYeah::VertexArray>& GetVertexArray() { return m_vertexArray; }
		inline CatolYeah::Ref<CatolYeah::Shader>& GetShader() { return m_shader; }
		inline glm::vec3& GetPosition() { return m_position; }
		inline const glm::vec3& GetPosition() const { return m_position; }

	private:
		CatolYeah::Ref<CatolYeah::VertexArray> m_vertexArray;
		CatolYeah::Ref<CatolYeah::Shader> m_shader;
		glm::vec3 m_position;
		glm::vec4 m_color;
	};

	/*class Circle
	{
	public:
		Circle();
	};*/

	class BlockBall : public CatolYeah::Layer
	{
	public:
		BlockBall();
		~BlockBall() {}

		virtual void OnAttach() override;
		virtual void OnUpdate(CatolYeah::Timestep ts) override;
		virtual void OnEvent(CatolYeah::Event& e) override;
		virtual void OnImGuiRender() override;

	private:
		float m_aspectRatio;
		Rect m_playerA, m_playerB;
		float m_rectTranslationSpeed = 3.0f;
		//Circle m_ball;
		uint8_t m_scorePlayerA = 0, m_scorePlayerB = 0;

		CatolYeah::OrthographicCameraController m_cameraController;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};
		std::vector<ProfileResult> m_profileResults;
	};

}// BlockBall