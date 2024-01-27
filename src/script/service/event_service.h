#pragma once
#include "pch.h"
#include "service.h"
#include "core/util.h"

namespace djinn
{
	class entity;

	class event_service final : public haul::parent<service<event_service>>
	{
		friend super;
	public:
		struct listener
		{
			JSContext* ctx;
			JSValue fn;
			bool operator==(listener const& other) const
			{
				return ctx == other.ctx && JS_VALUE_GET_PTR(fn) == JS_VALUE_GET_PTR(other.fn);
			}
		};
	public:
		DCM(event_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
		static void dispatch(std::string const& event, JSValue payload);
		static void subscribe(std::string const& event, JSContext* const ctx, JSValue const& fn);
		static void unsubscribe(std::string const& event, JSContext* const ctx, JSValue const& fn);
		static void unsubscribe_all(JSContext* const ctx);
	private:
		std::unordered_map<std::string, std::unordered_set<listener>> m_listeners;
		std::unordered_map<JSContext*, std::unordered_map<std::string, listener>> m_ctx2listeners;
	private:
		event_service();
	};
} // namespace djinn

template<>
struct std::hash<::djinn::event_service::listener>
{
	u64 operator()(::djinn::event_service::listener const& l) const
	{
		return ::djinn::u::hash_combine(std::hash<JSContext*>()(l.ctx), std::hash<void*>()(JS_VALUE_GET_PTR(l.fn)));
	}
};
