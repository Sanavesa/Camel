#pragma once

#include "Core.h"

namespace Camel
{
	class Input
	{
	public:
		static inline bool GetKey(const SDL_Scancode key) noexcept
		{
			return GetInstance().m_KeyboardState[key];
		}

		static inline bool GetKeyDown(const SDL_Scancode key) noexcept
		{
			return GetInstance().m_KeyboardState[key] && !GetInstance().m_PreviousKeyboardState[key];
		}

		static inline bool GetKeyUp(const SDL_Scancode key) noexcept
		{
			return !GetInstance().m_KeyboardState[key] && GetInstance().m_PreviousKeyboardState[key];
		}

		static inline bool GetMouseButton(const Uint8 button) noexcept
		{
			return GetInstance().m_MouseState & SDL_BUTTON(button);
		}

		static inline bool GetMouseButtonDown(const Uint8 button) noexcept
		{
			return (GetInstance().m_MouseState & SDL_BUTTON(button)) && !(GetInstance().m_PreviousMouseState & SDL_BUTTON(button));
		}

		static inline bool GetMouseButtonUp(const Uint8 button) noexcept
		{
			return !(GetInstance().m_MouseState & SDL_BUTTON(button)) && (GetInstance().m_PreviousMouseState & SDL_BUTTON(button));
		}

		static inline glm::ivec2 GetMousePosition() noexcept
		{
			return { GetInstance().m_MouseX, GetInstance().m_MouseY };
		}

		static inline glm::ivec2 GetMouseDelta() noexcept
		{
			return { GetInstance().m_MouseX - GetInstance().m_PreviousMouseX, GetInstance().m_MouseY - GetInstance().m_PreviousMouseY };
		}

		static inline glm::ivec2 GetMouseScroll() noexcept
		{
			return { GetInstance().m_MouseScrollX, GetInstance().m_MouseScrollY };
		}

		static inline bool IsQuitting() noexcept
		{
			return GetInstance().m_IsQuitting;
		}

	private:
		static void Update() noexcept
		{
			GetInstance().UpdateImpl();
		}

		friend class Application;

	private:
		static Input& GetInstance()
		{
			static Input instance;
			return instance;
		}

		Input()
			: m_IsQuitting(false)
		{
			// Current
			m_KeyboardState = SDL_GetKeyboardState(nullptr);
			m_MouseState = SDL_GetMouseState(&m_MouseX, &m_MouseY);
			m_MouseScrollX = 0;
			m_MouseScrollY = 0;

			// Previous
			memset(m_PreviousKeyboardState, 0, SDL_NUM_SCANCODES);
			m_PreviousMouseState = m_MouseState;
			m_PreviousMouseX = m_MouseX;
			m_PreviousMouseY = m_MouseY;
		}

		void UpdateImpl() noexcept
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
					m_IsQuitting = true;

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

		bool m_IsQuitting;
	};
}