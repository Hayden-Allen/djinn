#include "pch.h"
#include "imgui_service.h"
#include "script/js.h"
#include "core/util.h"
#include "core/constants.h"

namespace djinn
{
	namespace js::imgui_service
	{
		JSValue text(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 1);
			std::string const& msg = js::extract_string(ctx, argv[0]);
			ImGui::Text("%s", msg.c_str());
			return JS_UNDEFINED;
		}
		JSValue color_picker4(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 2);
			std::string const& label = js::extract_string(ctx, argv[0]);
			std::vector<f32> colors = js::extract_f32_array(ctx, argv[1]);
			ASSERT(colors.size() == 4);
			ImGui::ColorPicker4(label.c_str(), colors.data());
			return js::create_f32_array(ctx, 4, colors.data());
		}
	} // namespace js::imgui_service


	void imgui_service::init(sptr<mgl::context> ctx)
	{
		ASSERT(!s_instance);
		s_instance = new imgui_service(ctx);
	}
	void imgui_service::shutdown()
	{
		super::shutdown();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void imgui_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "text", 1, js::imgui_service::text);
		super::register_function(ctx, "colorPicker4", 2, js::imgui_service::color_picker4);
	}
	void imgui_service::begin_frame()
	{
		f32 x_scale = -1.0f, y_scale = -1.0f;
		glfwGetWindowContentScale(s_instance->m_context->window, &x_scale, &y_scale);
		if (x_scale != s_instance->m_prev_x_scale || y_scale != s_instance->m_prev_y_scale)
		{
			ImGui_ImplOpenGL3_DestroyFontsTexture();
			ImGui::GetIO().Fonts->Clear();
			s_instance->m_font_size = 20.0f * std::max(x_scale, y_scale);
			ImGui::GetIO().Fonts->AddFontFromFileTTF(u::to_absolute(c::base_dir::font, "Jost-Regular.ttf").c_str(), s_instance->m_font_size);
			ImGui::GetIO().Fonts->Build();
			ImGui_ImplOpenGL3_CreateFontsTexture();
			s_instance->m_prev_x_scale = x_scale;
			s_instance->m_prev_y_scale = y_scale;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	void imgui_service::end_frame()
	{
		ImGui::Render();

		/*glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glEnable(GL_BLEND);*/
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		/*glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glDisable(GL_BLEND);*/
	}



	imgui_service::imgui_service(sptr<mgl::context> ctx) :
		haul::parent<service<imgui_service>>("ImGui"),
		m_context(ctx),
		m_prev_x_scale(-1),
		m_prev_y_scale(-1),
		m_font_size(20)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		// ImGuiIO& io = ImGui::GetIO();
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		// io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		ImGui::StyleColorsDark();
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui_ImplGlfw_InitForOpenGL(ctx->window, false);
		ImGui_ImplGlfw_InstallCallbacks(ctx->window);
		ImGui_ImplOpenGL3_Init("#version 410 core");

		// See also: https://github.com/TheCherno/Hazel/blob/master/Hazel/src/Hazel/ImGui/ImGuiLayer.cpp
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.48f, 0.4805f, 0.481f, 1.0f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.48f, 0.4805f, 0.481f, 1.0f };
		colors[ImGuiCol_SliderGrabActive] = ImVec4{ 0.48f, 0.4805f, 0.481f, 1.0f };
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.25f, 0.255f, 0.26f, 1.0f };
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	}
} // namespace djinn
