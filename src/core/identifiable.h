#pragma once
#include "pch.h"

namespace djinn
{
	class identifiable
	{
	public:
		DCM(identifiable);
		virtual ~identifiable() {}
	public:
		id_t get_id() const { return m_id; }
	protected:
		identifiable(id_t const id) :
			m_id(id)
		{}
	protected:
		id_t m_id;
	};
} // namespace djinn
