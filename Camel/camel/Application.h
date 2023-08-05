#pragma once

#include "Core.h"

namespace Camel
{
	class Application
	{
	public:
		Application(const int width, const int height, const std::string& title)
		{
			SDL_Init(SDL_INIT_VIDEO);

			// Use OpenGL 3.3 core profile
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			m_Window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
			m_Context = SDL_GL_CreateContext(m_Window);

			glewInit();

			// Enable blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// Enable depth test
			glEnable(GL_DEPTH_TEST);

			// Enable face culling
			glEnable(GL_CULL_FACE);
			glFrontFace(GL_CW); // Left Handedness

			CAMEL_LOG_INFO("GL Version: {}", std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
		}

		virtual ~Application()
		{
			SDL_GL_DeleteContext(m_Context);
			SDL_DestroyWindow(m_Window);
			SDL_Quit();
		}

		inline int GetWidth() const noexcept
		{
			int width;
			SDL_GetWindowSize(m_Window, &width, nullptr);
			return width;
		}

		inline int GetHeight() const noexcept
		{
			int height;
			SDL_GetWindowSize(m_Window, nullptr, &height);
			return height;
		}

		inline float GetAspectRatio() const noexcept
		{
			int width, height;
			SDL_GetWindowSize(m_Window, &width, &height);
			return (float)width / height;
		}

		inline const Input& GetInput() const noexcept { return m_Input; }
		inline Input& GetInput() noexcept { return m_Input; }

		inline void Quit() noexcept { m_IsRunning = false; }

		void Run()
		{
			m_IsRunning = true;
			OnStart();

			while (m_IsRunning)
			{
				m_Input.Update();

				if (m_Input.HasQuit())
				{
					Quit();
					break;
				}

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				OnUpdate(0.0f);

				SDL_GL_SwapWindow(m_Window);
			}
		}

		virtual void OnStart() = 0;
		virtual void OnUpdate(float deltaTime) = 0;

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;
		Input m_Input;

		bool m_IsRunning;
	};
}