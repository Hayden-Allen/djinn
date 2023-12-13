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

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#undef near
#undef far

#include "hapi/hapi.h"
using namespace hapi;
#include "hats/hats.h"
using namespace hats;
#include "haul/haul.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "mingl/mingl.h"
using namespace mgl;
#define QJS_USE_CPP
#include "quickjs.h"



#ifdef _DEBUG
#	define ASSERT(x) \
		if (!(x)) __debugbreak();
#else
#	define ASSERT(x)
#endif
#define MAX_VALUE_T(T) std::numeric_limits<T>::max();
#define MAX_VALUE(x)   MAX_VALUE_T(decltype(x))
#define DCM(name)               \
	name(name const&) = delete; \
	name(name&&) = delete;

typedef u32 id_t;
