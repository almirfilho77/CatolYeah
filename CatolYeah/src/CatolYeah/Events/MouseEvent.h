#ifndef __MOUSE_EVENTS_H__
#define __MOUSE_EVENTS_H__
#pragma once

#include "Event.h"
#include "CatolYeah/MouseCodes.h"

namespace CatolYeah {

	/*
	 * @class MouseButtonEvent
	 * @brief Base class to describe a mouse button event
	 */
	class CATOLYEAH_API MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return t_button; }

		virtual int GetCategoryFlags() const override { return (EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton); }

	protected:
		MouseButtonEvent(const MouseCode button)
			:	t_button(button) {}

		MouseCode t_button;
	};


	/*
	 * @class MouseButtonPressedEvent
	 * @brief Class to describe a "mouse button pressed" event
	 */
	class CATOLYEAH_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			:	MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonPressed)

		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "MouseButtonPressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressed: " << t_button;
			return ss.str();
		}
#endif
		/* -------------- */
	};


	/*
	 * @class MouseButtonReleasedEvent
	 * @brief Class to describe a "mouse button released" event
	 */
	class CATOLYEAH_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		EVENT_CLASS_TYPE(MouseButtonReleased)

		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "MouseButtonReleased"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleased: " << t_button;
			return ss.str();
		}
#endif
		/* -------------- */
	};


	/*
	 * @class MouseMovedEvent
	 * @brief Class to describe a "mouse moved" event
	 */
	class CATOLYEAH_API MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
			:	m_mouseX(x),
				m_mouseY(y) {}

		float GetMouseX() const { return m_mouseX; }
		float GetMouseY() const { return m_mouseY; }

		virtual int GetCategoryFlags() const override { return (EventCategoryInput | EventCategoryMouse); }

		EVENT_CLASS_TYPE(MouseMoved)


#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "MouseMoved"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved: x=" << m_mouseX << " / y=" << m_mouseY;
			return ss.str();
		}
#endif

	private:
		float m_mouseX, m_mouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float xOffset, const float yOffset)
			: m_xOffset(xOffset), m_yOffset(yOffset) {}

		float GetXOffset() const { return m_xOffset; }
		float GetYOffset() const { return m_yOffset; }

		virtual int GetCategoryFlags() const override { return (EventCategoryInput | EventCategoryMouse); }

		EVENT_CLASS_TYPE(MouseScrolled)


#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "MouseScrolled"; }
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: x=" << m_xOffset << ", y=" << m_yOffset;
			return ss.str();
		}
#endif
	private:
		float m_xOffset, m_yOffset;
	};
}

#endif /* __MOUSE_EVENTS_H__ */
