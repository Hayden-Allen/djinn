#pragma once

#ifndef _WIN32
#	ifndef NDEBUG
#		define _DEBUG
#	endif
#	define __debugbreak __builtin_debugtrap
#	define sprintf_s	 snprintf
#	define vsprintf_s	 vsnprintf
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

#ifdef DJINN_DIST
#	define HAPI_OPAQUE
#endif
#include "hapi/hapi.h"
using namespace hapi;
#include "hats/hats.h"
using namespace hats;
#include "haul/haul.h"
#ifdef _DEBUG
// #	define GLAD_WRAPPER_ENABLED // FIXME: We have disabled error checking for now, so we can keep compiling on macOS until we're able to fix the error
#endif
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

#define M3D_NUMBONE 4
#include "model3d/m3d.h"

#include "BulletCollision/NarrowPhaseCollision/btRaycastCallback.h"
#include "btBulletDynamicsCommon.h"



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
