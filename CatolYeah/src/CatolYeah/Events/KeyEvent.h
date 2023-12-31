#ifndef __KEY_EVENT_H__
#define __KEY_EVENT_H__
#pragma once

#include "Event.h"

namespace CatolYeah {

	/*
	 * @class KeyEvent
	 * @brief Base class class to describe a key event
	 */
	class KeyEvent : public Event
	{
	public:
		virtual int GetKeyCode() const { return t_keyCode; }
		virtual int GetCategoryFlags() const override 
		{
			return (EventCategory::EventCategoryInput | 
					EventCategory::EventCategoryKeyboard);
		}

	protected:
		KeyEvent(int keycode)
			:	t_keyCode(keycode) {};

		int t_keyCode;
	};

	/*
	 * @class KeyPressedEvent
	 * @brief Class to describe a "key pressed" event
	 */
	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeat_count)
			:	KeyEvent(keycode),
				m_repeatCount(repeat_count) {}

		inline int GetRepeatCount() const { return m_repeatCount; }

		/* Substituting this code by the MACRO EVENT_CLASS_TYPE
		static EventType GetStaticEventType() { return EventType::KeyPressed; }
		virtual EventType GetEventType() const override { return GetStaticEventType(); }
		*/
		EVENT_CLASS_TYPE(KeyPressed)


		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "KeyPressed"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << t_keyCode << " (" << m_repeatCount << " repeats)";
			return ss.str();
		}
#endif
		/* --------------- */
	
	private:
		int m_repeatCount;
	};

	/*
	 * @class KeyReleasedEvent
	 * @brief Class to describe a "key released" event
	 */
	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			:	KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyReleased)


		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "KeyReleased"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << t_keyCode;
			return ss.str();
		}
#endif
		/* --------------- */
	};

	/*
	 * @class KeyTypedEvent
	 * @brief Class to describe a "key released" event
	 */
	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
			: KeyEvent(keycode) {}

		EVENT_CLASS_TYPE(KeyTyped)

		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "KeyTyped"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << t_keyCode;
			return ss.str();
		}
#endif
		/* --------------- */
	};

}

#endif /* __KEY_EVENT_H__ */