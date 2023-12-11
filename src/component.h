#pragma once
#include <inttypes.h>
#define DEFINE_COMPONENT(name, id, priority, code) \
	class name : public component \
	{ code \
	public: static uint64_t get_id() { return id; } \
	public: static uint64_t get_priority() { return priority; } \
	public: static component_key get_key() { return { get_id(), get_priority() }; } \
	};

struct component_key
{
	uint64_t id, priority;
};

struct component_comparator
{
	bool operator()(component_key const& a, component_key const& b) const
	{
		return a.priority < b.priority;
	}
};

class component
{
public:
	static uint64_t get_id()
	{
		return MAX_VALUE_T(uint64_t);
	}
	static uint64_t get_priority()
	{
		return MAX_VALUE_T(uint64_t);
	}
	static component_key get_key()
	{
		return { get_id(), get_priority() };
	}
public:
	virtual void run() = 0;
protected:
	component() {}
	virtual ~component() {}
};

DEFINE_COMPONENT(physics_component, 0, 0,
public:
	void run() override
	{
		printf("PHYS\n");
	}
);
DEFINE_COMPONENT(script_component, 1, 1,
public:
	script_component(const std::string& msg) :
		m_msg(msg)
	{}
	void run() override
	{
		printf("SCRIPT: %s\n", m_msg.c_str());
	}
private:
	std::string m_msg;
);
