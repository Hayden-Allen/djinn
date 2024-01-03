#pragma once

#ifndef _WIN32
#	define __debugbreak		__builtin_debugtrap
#	define sprintf_s(buf, ...) snprintf((buf), sizeof(buf), __VA_ARGS__)
#endif

#include <stdio.h>
#include <filesystem>
#include <iostream>
#include <numeric>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include <thread>
#include <array>

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	undef min
#	undef max
#	undef near
#	undef far
#endif

#include "hapi/hapi.h"
using namespace hapi;
#include "hats/hats.h"
using namespace hats;
#include "haul/haul.h"
#include "glad/glad.h"

#define GLFW_INCLUDE_GLEXT
#include "GLFW/glfw3.h"
#include "mingl/mingl.h"
using namespace mgl;

#define QJS_USE_CPP
#include "quickjs.h"
#include "stb/stb_image.h"
#include "nanovg.h"
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "miniaudio.h"
#include "m3d.h"



#ifdef DJINN_DIST
#	define ASSERT(x)
#else
#	define ASSERT(x) \
		if (!(x)) __debugbreak();
#endif
#define MIN_VALUE_T(T) std::numeric_limits<T>::lowest()
#define MIN_VALUE(x)   MIN_VALUE_T(decltype(x))
#define MAX_VALUE_T(T) std::numeric_limits<T>::max()
#define MAX_VALUE(x)   MAX_VALUE_T(decltype(x))
#define DCM(name)               \
	name(name const&) = delete; \
	name(name&&) = delete;

typedef u32 id_t;
