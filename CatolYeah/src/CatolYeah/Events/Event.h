#ifndef __EVENT_H__
#define __EVENT_H__
#pragma once

#include "CatolYeah/Core.h"

namespace CatolYeah {

#define EVENT_RETURN_PASS_ON false
#define EVENT_RETURN_HANDLED true

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)
	};

/* MACRO DEFINITION */
#define EVENT_CLASS_TYPE(type)  static EventType GetStaticEventType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticEventType(); } 
/* ----------------- */

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType GetEventType() const = 0;
		virtual int GetCategoryFlags() const = 0;
		
		/* Use for DEBUG configuration only, if you are going to implement
		 * it in your own derived class, set the precompiler to remove it
		 * in case of RELEASE or DIST mode
		 */
#ifdef CY_CONFIG_DEBUG
		virtual const char *GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }
#endif

		inline bool IsInCategory(EventCategory category)
		{
			return (GetCategoryFlags() & category);
		}

		bool IsHandled() const { return t_handled; }

	protected:
		bool t_handled = false;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			:	m_event(event) {}

		template<typename EventTy, typename EventFn>
		bool Dispatch(const EventFn& func)
		{
			if (m_event.GetEventType() == EventTy::GetStaticEventType())
			{
				m_event.t_handled = func(static_cast<EventTy&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

#ifdef CY_CONFIG_DEBUG
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
#endif
} //CatolYeah

#endif /* __EVENT_H__ */