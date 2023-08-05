#pragma once

#include "Core.h"

namespace Camel
{
	class Input
	{
	public:
		friend class Application;

		Input()
		{
			// Current
			m_KeyboardState = SDL_GetKeyboardState(NULL);
			m_MouseState = SDL_GetMouseState(&m_MouseX, &m_MouseY);
			m_MouseScrollX = 0;
			m_MouseScrollY = 0;

			// Previous
			memset(m_PreviousKeyboardState, 0, SDL_NUM_SCANCODES);
			m_PreviousMouseState = m_MouseState;
			m_PreviousMouseX = m_MouseX;
			m_PreviousMouseY = m_MouseY;
		}

		inline bool GetKey(const SDL_Scancode key) const noexcept { return m_KeyboardState[key]; }
		inline bool GetKeyDown(const SDL_Scancode key) const noexcept { return m_KeyboardState[key] && !m_PreviousKeyboardState[key]; }
		inline bool GetKeyUp(const SDL_Scancode key) const noexcept { return !m_KeyboardState[key] && m_PreviousKeyboardState[key]; }

		inline bool GetMouseButton(const Uint8 button) const noexcept { return m_MouseState & SDL_BUTTON(button); }
		inline bool GetMouseButtonDown(const Uint8 button) const noexcept { return (m_MouseState & SDL_BUTTON(button)) && !(m_PreviousMouseState & SDL_BUTTON(button)); }
		inline bool GetMouseButtonUp(const Uint8 button) const noexcept { return !(m_MouseState & SDL_BUTTON(button)) && (m_PreviousMouseState & SDL_BUTTON(button)); }

		inline SDL_Point GetMousePosition() const noexcept { return { m_MouseX, m_MouseY }; }
		inline SDL_Point GetMouseDelta() const noexcept { return { m_MouseX - m_PreviousMouseX, m_MouseY - m_PreviousMouseY }; }

		inline SDL_Point GetMouseScroll() const noexcept { return { m_MouseScrollX, m_MouseScrollY }; }

	private:
		inline bool HasQuit() const noexcept { return m_Quit; }

		// Must be called after done from input
		void Update() noexcept
		{
			// Update previous
			memcpy(m_PreviousKeyboardState, m_KeyboardState, SDL_NUM_SCANCODES);
			m_PreviousMouseState = m_MouseState;
			m_PreviousMouseX = m_MouseX;
			m_PreviousMouseY = m_MouseY;
			m_MouseScrollX = 0;
			m_MouseScrollY = 0;

			// Update current
			m_MouseState = SDL_GetMouseState(&m_MouseX, &m_MouseY);

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (event.type == SDL_QUIT)
					m_Quit = true;

				if (event.type == SDL_MOUSEWHEEL)
				{
					m_MouseScrollX = event.wheel.x;
					m_MouseScrollY = event.wheel.y;
				}
			}
		}

	private:
		const Uint8* m_KeyboardState;
		Uint8 m_PreviousKeyboardState[SDL_NUM_SCANCODES];

		Uint32 m_MouseState;
		Uint32 m_PreviousMouseState;

		int m_MouseX, m_MouseY, m_MouseScrollX, m_MouseScrollY;
		int m_PreviousMouseX, m_PreviousMouseY;

		bool m_Quit = false;
	};
}