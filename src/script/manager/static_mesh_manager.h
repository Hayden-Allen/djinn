#pragma once
#include "pch.h"
#include "manager.h"
#include "asset/static_mesh.h"

namespace djinn
{
	class static_mesh_manager final : public ref_counted_file_manager<static_mesh>
	{
	};
} // namespace djinn
