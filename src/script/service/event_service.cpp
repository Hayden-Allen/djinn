#include "pch.h"
#include "event_service.h"
#include "script/js.h"

namespace std
{
	bool operator==(JSValue const& a, JSValue const& b)
	{
		return JS_VALUE_GET_PTR(a) == JS_VALUE_GET_PTR(b);
	}
} // namespace std



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
		ASSERT(argc == 2 || argc == 3);
		std::string const& event = js::extract_string(ctx, argv[0]);
		JSValue const& fn = argv[1];
		JSValue const& user = argc == 3 ? argv[2] : JS_UNDEFINED;
		::djinn::event_service::subscribe(event, ctx, fn, user);
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
		super::register_function(ctx, "subscribe", 3, js::event_service::subscribe);
		super::register_function(ctx, "unsubscribe", 2, js::event_service::unsubscribe);
	}
	void event_service::dispatch(std::string const& event, JSValue payload)
	{
		auto const& it = s_instance->m_listeners.find(event);
		if (it == s_instance->m_listeners.end())
			return;
		for (listener const* const l : it->second)
		{
			JSValue args[3] = { js::create_string(l->ctx, event), payload, l->user };
			JS_Call(l->ctx, l->fn, JS_UNDEFINED, 3, args);
		}
	}
	void event_service::subscribe(std::string const& event, JSContext* const ctx, JSValue const& fn, JSValue const& user)
	{
		ASSERT(JS_IsFunction(ctx, fn));
		void* const ptr = JS_VALUE_GET_PTR(fn);

		listener* const l = new listener(ctx, fn, user);
		auto const& it = s_instance->m_fn2listener.find(ptr);
		// remove old listener if it exists
		if (it != s_instance->m_fn2listener.end())
		{
			listener* const existing = it->second;
			ASSERT(s_instance->m_listeners.contains(event));
			ASSERT(s_instance->m_listeners.at(event).contains(existing));

			s_instance->m_listeners.at(event).erase(existing);
			s_instance->m_fn2listener.erase(ptr);
			s_instance->m_ctx2listeners.at(ctx).erase(existing);
			delete existing;
		}
		// insert new listener
		s_instance->m_listeners[event].insert(l);
		s_instance->m_fn2listener.insert({ ptr, l });
		s_instance->m_ctx2listeners[ctx].insert({ l, event });
	}
	void event_service::unsubscribe(std::string const& event, JSContext* const ctx, JSValue const& fn)
	{
		ASSERT(JS_IsFunction(ctx, fn));
		void* const ptr = JS_VALUE_GET_PTR(fn);

		ASSERT(s_instance->m_fn2listener.contains(ptr));
		listener* const l = s_instance->m_fn2listener.at(ptr);
		s_instance->m_fn2listener.erase(ptr);

		ASSERT(s_instance->m_listeners.contains(event));
		ASSERT(s_instance->m_listeners.at(event).contains(l));
		s_instance->m_listeners.at(event).erase(l);
		if (s_instance->m_listeners.at(event).empty())
			s_instance->m_listeners.erase(event);

		ASSERT(s_instance->m_ctx2listeners.contains(ctx));
		ASSERT(s_instance->m_ctx2listeners.at(ctx).contains(l));
		s_instance->m_ctx2listeners.at(ctx).erase(l);
		if (s_instance->m_ctx2listeners.at(ctx).empty())
			s_instance->m_ctx2listeners.erase(ctx);

		delete l;
	}
	void event_service::unsubscribe_all(JSContext* const ctx)
	{
		auto const& it = s_instance->m_ctx2listeners.find(ctx);
		if (it == s_instance->m_ctx2listeners.end())
			return;
		for (auto const& pair : it->second)
		{
			s_instance->m_listeners[pair.second].erase(pair.first);
			if (s_instance->m_listeners[pair.second].empty())
				s_instance->m_listeners.erase(pair.second);
			s_instance->m_fn2listener.erase(JS_VALUE_GET_PTR(pair.first->fn));
			delete pair.first;
		}
		s_instance->m_ctx2listeners.erase(ctx);
	}



	event_service::event_service() :
		haul::parent<service<event_service>>("Event")
	{}
} // namespace djinn
