#pragma once

// Logging
#include <iostream>
#include <string>
#include <format>

// SDL & OpenGL
#include "GL/glew.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#ifdef CAMEL_DEBUG_MODE

#define CAMEL_LOG_INFO(...) std::cout << "[INFO]: " << std::format(__VA_ARGS__) << std::endl;
#define CAMEL_LOG_WARN(...) std::cerr << "[WARN]: " << std::format(__VA_ARGS__) << std::endl;
#define CAMEL_LOG_ERROR(...) std::cerr << "[ERROR]: " << std::format(__VA_ARGS__) << std::endl;

#define CAMEL_ASSERT(expr, ...) \
	{ \
		if(!(expr)) { \
			std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << " inside " << __FUNCTION__ << std::endl; \
			std::cerr << "Reason: " << std::format(__VA_ARGS__) << std::endl; \
			std::abort(); \
		} \
	}

#else

#define CAMEL_LOG_INFO(...) (void)0
#define CAMEL_LOG_WARN(...) (void)0
#define CAMEL_LOG_ERROR(...) (void)0
#define CAMEL_ASSERT(expr, ...) (void)0

#endif // CAMEL_DEBUG_MODE

/*
Example usage:
	CAMEL_LOG_INFO("This is an info message. Value: {}", 42);
	CAMEL_LOG_WARN("This is a warning message. Value: {}", 42);
	CAMEL_LOG_ERROR("This is an error message. Value: {}", 42);
	CAMEL_ASSERT(value >= 0, "Value {} must be non-negative", value);
*/