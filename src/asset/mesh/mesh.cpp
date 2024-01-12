#include "pch.h"
#include "mesh.h"
#include "asset/shaders.h"

namespace djinn
{
	mesh::~mesh() {}



	bool mesh::is_animated() const
	{
		return false;
	}



	mesh::mesh() {}



	void mesh::insert_instance(wptr<shaders> const& x, sptr<mesh_instance> instance)
	{
		auto it = m_batches.find(x);
		if (it == m_batches.end())
		{
			m_batches.try_emplace(x, wptr(this), x);
		}
		m_batches.at(x).insert(instance);
	}
	void mesh::remove_instance(wptr<shaders> const& shaders, u64 const index)
	{
		m_batches.at(shaders).remove(index);
		if (m_batches.at(shaders).empty())
			m_batches.erase(shaders);
	}
} // namespace djinn
