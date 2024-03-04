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



	void mesh::insert_instance(wptr<shaders> const& shaders, sptr<mesh_instance> instance)
	{
		// if we don't have any batches for these shaders yet, make space for them
		auto it = m_batches.find(shaders.get());
		if (it == m_batches.end())
		{
			m_batches.insert({ shaders.get(), {} });
			m_num_empty_batches.insert({ shaders.get(), 0 });
		}

		std::vector<mesh_instance_batch>& batches = m_batches.at(shaders.get());
		bool found = false;
		// check if there's an existing batch that can fit the new instance
		for (u64 i = 0; i < batches.size(); i++)
		{
			if (!batches[i].is_full())
			{
				// this batch is currently empty, but won't be after we insert the new instance
				if (batches[i].empty())
					m_num_empty_batches[shaders.get()]--;

				batches[i].insert(instance);
				instance->bind_mesh(i);
				found = true;
				break;
			}
		}

		// all existing batches full, need to make a new one
		if (!found)
		{
			batches.emplace_back(wptr(this), shaders);
			batches.back().insert(instance);
			instance->bind_mesh(batches.size() - 1);
		}
	}
	void mesh::remove_instance(wptr<shaders> const& shaders, u64 const batch_number, u64 const index)
	{
		std::vector<mesh_instance_batch>& batches = m_batches.at(shaders.get());
		ASSERT(batch_number < batches.size());

		batches.at(batch_number).remove(index);
		if (batches.at(batch_number).empty())
			m_num_empty_batches[shaders.get()]++;

		// TODO? remove empty batches (but need to update mesh_instance::m_batch_number)

		// no more batches for given shaders, clean up
		if (m_num_empty_batches.at(shaders.get()) == m_batches.at(shaders.get()).size())
			m_batches.erase(shaders.get());
	}
} // namespace djinn
