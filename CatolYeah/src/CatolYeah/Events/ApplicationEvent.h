#ifndef __APPLICATION_EVENTS_H__
#define __APPLICATION_EVENTS_H__
#pragma once

#include "Event.h"
#include <sstream>

namespace CatolYeah {

	class CATOLYEAH_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		
		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		EVENT_CLASS_TYPE(WindowClose)


		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "WindowClose"; }
#endif
		/* -------------- */
	};

	class CATOLYEAH_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			:	m_width(width), 
				m_height(height) {}

		unsigned int GetWindowWidth() const { return m_width; }
		unsigned int GetWindowHeight() const { return m_height; }

		virtual int GetCategoryFlags() const override { return EventCategoryApplication; }

		EVENT_CLASS_TYPE(WindowResize)


		/* For DEBUG only */
#ifdef CY_CONFIG_DEBUG
		virtual const char* GetName() const override { return "WindowResize"; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResize event: w=" << m_width << " / h=" << m_height;
			return ss.str();
		}
#endif
		/* -------------- */

	private:
		unsigned int m_width, m_height;
	};
}

#endif /* __APPLICATION_EVENTS_H__ */