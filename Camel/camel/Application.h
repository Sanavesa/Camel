#pragma once

#include "Core.h"

namespace Camel
{
	class Application
	{
	public:
		Application(const int width, const int height, const std::string& title)
			: m_IsRunning(false), m_Window(nullptr), m_Context(nullptr)
		{
			if (SDL_Init(SDL_INIT_VIDEO) != 0)
			{
				CAMEL_LOG_ERROR("Failed to initialize SDL: {}", SDL_GetError());
				return;
			}

			// Use OpenGL 3.3 core profile
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

			m_Window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
			if (!m_Window)
			{
				CAMEL_LOG_ERROR("Failed to create window: {}", SDL_GetError());
				SDL_Quit();
				return;
			}

			m_Context = SDL_GL_CreateContext(m_Window);
			if (!m_Context)
			{
				CAMEL_LOG_ERROR("Failed to create OpenGL context: {}", SDL_GetError());
				SDL_DestroyWindow(m_Window);
				SDL_Quit();
				return;
			}

			GLenum glewError = glewInit();
			if (glewError != GLEW_OK)
			{
				CAMEL_LOG_ERROR("Error initializing GLEW: {}", std::string(reinterpret_cast<const char*>(glewGetErrorString(glewError))));
				SDL_GL_DeleteContext(m_Context);
				SDL_DestroyWindow(m_Window);
				SDL_Quit();
				return;
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glEnable(GL_DEPTH_TEST);

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

		inline void Quit() noexcept { m_IsRunning = false; }

		void Run()
		{
			m_IsRunning = true;
			Input::Update();

			OnStart();

			Uint64 previousTicks = SDL_GetTicks64();
			float deltaTime = 0.0f;

			while (m_IsRunning)
			{
				// Calculate delta time in seconds
				Uint64 currentTicks = SDL_GetTicks64();
				deltaTime = (float)(currentTicks - previousTicks) / 1000.0f;
				previousTicks = currentTicks;

				Input::Update();

				if (Input::IsQuitting())
				{
					Quit();
					break;
				}

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				OnUpdate(deltaTime);

				SDL_GL_SwapWindow(m_Window);
			}
		}

		virtual void OnStart() = 0;
		virtual void OnUpdate(float deltaTime) = 0;

	private:
		SDL_Window* m_Window;
		SDL_GLContext m_Context;

		bool m_IsRunning;
	};
}