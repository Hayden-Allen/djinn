#include "pch.h"
#include "event_service.h"
#include "script/js.h"

namespace djinn::js::event_service
{
	JSValue dispatch(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		std::string const& event = js::extract_string(ctx, argv[0]);
		JSValue const& payload = argv[1];
		::djinn::event_service::dispatch(event, payload);
		return JS_UNDEFINED;
	}
	JSValue subscribe(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		std::string const& event = js::extract_string(ctx, argv[0]);
		JSValue const& fn = argv[1];
		::djinn::event_service::subscribe(event, ctx, fn);
		return JS_UNDEFINED;
	}
	JSValue unsubscribe(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		std::string const& event = js::extract_string(ctx, argv[0]);
		JSValue const& fn = argv[1];
		::djinn::event_service::unsubscribe(event, ctx, fn);
		return JS_UNDEFINED;
	}
} // namespace djinn::js::event_service



namespace djinn
{
	void event_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new event_service();
	}
	void event_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "dispatch", 2, js::event_service::dispatch);
		super::register_function(ctx, "subscribe", 2, js::event_service::subscribe);
		super::register_function(ctx, "unsubscribe", 2, js::event_service::unsubscribe);
	}
	void event_service::dispatch(std::string const& event, JSValue payload)
	{
		auto const& it = s_instance->m_listeners.find(event);
		if (it == s_instance->m_listeners.end())
			return;
		for (listener const& l : it->second)
		{
			JSValue args[2] = { js::create_string(l.ctx, event), payload };
			JS_Call(l.ctx, l.fn, JS_UNDEFINED, 2, args);
		}
	}
	void event_service::subscribe(std::string const& event, JSContext* const ctx, JSValue const& fn)
	{
		listener l = { ctx, fn };
		s_instance->m_listeners[event].insert(l);
		s_instance->m_ctx2listeners[ctx].insert({ event, l });
	}
	void event_service::unsubscribe(std::string const& event, JSContext* const ctx, JSValue const& fn)
	{
		ASSERT(s_instance->m_listeners.contains(event));
		listener l = { ctx, fn };
		ASSERT(s_instance->m_listeners[event].contains(l));
		s_instance->m_listeners[event].erase(l);
		if (s_instance->m_listeners[event].empty())
			s_instance->m_listeners.erase(event);
		s_instance->m_ctx2listeners[ctx].erase(event);
		if (s_instance->m_ctx2listeners[ctx].empty())
			s_instance->m_ctx2listeners.erase(ctx);
	}
	void event_service::unsubscribe_all(JSContext* const ctx)
	{
		auto const& it = s_instance->m_ctx2listeners.find(ctx);
		if (it == s_instance->m_ctx2listeners.end())
			return;
		for (auto const& pair : it->second)
		{
			s_instance->m_listeners[pair.first].erase(pair.second);
			if (s_instance->m_listeners[pair.first].empty())
				s_instance->m_listeners.erase(pair.first);
		}
		s_instance->m_ctx2listeners.erase(ctx);
	}



	event_service::event_service() :
		haul::parent<service<event_service>>("Event")
	{}
} // namespace djinn
