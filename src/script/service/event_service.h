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
			JSValue fn, user;
			listener(JSContext* const _ctx, JSValue const& _fn, JSValue const& _user) :
				ctx(_ctx),
				fn(_fn),
				user(JS_DupValue(ctx, _user)) // may go out of scope in the script, clone so it's valid regardless
			{}
			~listener()
			{
				JS_FreeValue(ctx, user);
			}
		};
	public:
		DCM(event_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
		static void dispatch(std::string const& event, JSValue payload);
		static void subscribe(std::string const& event, JSContext* const ctx, JSValue const& fn, JSValue const& user);
		static void unsubscribe(std::string const& event, JSContext* const ctx, JSValue const& fn);
		static void unsubscribe_all(JSContext* const ctx);
	private:
		std::unordered_map<std::string, std::unordered_set<listener*>> m_listeners;
		std::unordered_map<JSContext*, std::unordered_map<std::string, listener*>> m_ctx2listeners;
	private:
		event_service();
	};
} // namespace djinn
