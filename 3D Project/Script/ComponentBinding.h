#pragma once


#include "..\GameComponents\TransformComponent.h"
#include "..\GameComponents\MeshRenderComponent.h"
#include "..\GameComponents\AnimatorComponent.h"
#include "..\GameComponents\CameraComponent.h"
#include "..\Core\Events.h"
#include "..\Interface\IInput.h"
#include "..\Graphics3D\OpenGL\DataMap.h"
#include <sol.hpp>

using namespace Light;


#include <glm\detail\type_vec3.hpp>

sol::table GLMBinding(sol::this_state s)
{

	sol::state_view lua(s);
	sol::table module = lua.create_table();
	auto r = glm::vec3(2) + glm::vec3(1);
	module.new_usertype<glm::vec3>("vec3",
		sol::constructors < glm::vec3(), glm::vec3(float, float, float), glm::vec3(float)>(),
		"x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z,
		sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+),
		sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-),
		sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, float)>(glm::operator*));

	module.new_usertype<glm::vec4>("vec4",
		sol::constructors < glm::vec4(), glm::vec4(float, float, float,float), glm::vec4(float)>(),
		"x", &glm::vec4::x, "y", &glm::vec4::y, "z", &glm::vec4::z, "w", &glm::vec4::w);

	module.new_usertype<glm::quat>("quat",
		sol::constructors < glm::quat(), glm::quat(float, float, float,float)>(),
		"x", &glm::quat::x, "y", &glm::quat::y, "z", &glm::quat::z, "w", &glm::quat::w);

	/*module.new_usertype<glm::mat4>("mat4",
		sol::constructors < glm::mat4(), glm::mat4(float, float, float, float)>());*/

	module["length"] = glm::length<float, glm::packed_highp, glm::tvec3>;
	module["angleAxis"] = glm::angleAxis<float, glm::packed_highp>;
	module["normalize"] = glm::normalize<float, glm::packed_highp>;
	return module;

}



sol::table InputBinding(sol::this_state s)
{
	sol::state_view lua(s);
	sol::table module = lua.create_table();

	std::vector<const char*> keymap =
	{
		stringify(A),
		stringify(B),
		stringify(C),
		stringify(D),
		stringify(E),
		stringify(F),
		stringify(G),
		stringify(H),
		stringify(I),
		stringify(J),
		stringify(K),
		stringify(L),
		stringify(M),
		stringify(N),
		stringify(O),
		stringify(P),
		stringify(Q),
		stringify(R),
		stringify(S),
		stringify(T),
		stringify(U),
		stringify(V),
		stringify(W),
		stringify(X),
		stringify(Y),
		stringify(Z),
		stringify(Num0),
		stringify(Num1),
		stringify(Num2),
		stringify(Num3),
		stringify(Num4),
		stringify(Num5),
		stringify(Num6),
		stringify(Num7),
		stringify(Num8),
		stringify(Num9),
		stringify(Escape),
		stringify(LControl),
		stringify(LShift),
		stringify(LAlt),
		stringify(LSystem),
		stringify(RControl),
		stringify(RShift),
		stringify(RAlt),
		stringify(RSystem),
		stringify(Menu),
		stringify(LBracket),
		stringify(RBracket),
		stringify(Semicolon),
		stringify(Comma),
		stringify(Period),
		stringify(Quote),
		stringify(Slash),
		stringify(Backslash),
		stringify(GraveAccent),
		stringify(Equal),
		stringify(Hyphen),
		stringify(Space),
		stringify(Enter),
		stringify(Backspace),
		stringify(Tab),
		stringify(PageUp),
		stringify(PageDown),
		stringify(End),
		stringify(Home),
		stringify(Insert),
		stringify(Delete),
		stringify(Add),
		stringify(Subtract),
		stringify(Multiply),
		stringify(Divide),
		stringify(LeftArrow),
		stringify(RightArrow),
		stringify(UpArrow),
		stringify(DownArrow),
		stringify(Numpad0),
		stringify(Numpad1),
		stringify(Numpad2),
		stringify(Numpad3),
		stringify(Numpad4),
		stringify(Numpad5),
		stringify(Numpad6),
		stringify(Numpad7),
		stringify(Numpad8),
		stringify(Numpad9),
		stringify(F1),
		stringify(F2),
		stringify(F3),
		stringify(F4),
		stringify(F5),
		stringify(F6),
		stringify(F7),
		stringify(F8),
		stringify(F9),
		stringify(F10),
		stringify(F11),
		stringify(F12),
		stringify(Pause),
	};

	sol::table Key = module.create("Key");

	for (int i = 0; i < keymap.size(); i++)
	{
		Key[keymap[i]] = toglfw3Key[i];
	}

	std::vector<const char*> moousemap = {
		stringify(Left),
		stringify(Right),
		stringify(Middle),
		stringify(XButton1),
		stringify(XButton2) 
	};

	sol::table MouseButton = module.create("MouseButton");

	for (int i = 0; i < moousemap.size(); i++)
	{
		MouseButton[moousemap[i]] = i;
	}

	sol::table KeyAction = module.create("KeyAction");
	KeyAction["Press"] = Press;
	KeyAction["Release"] = Release;
	KeyAction["Repeat"] = Repeat;

	return module;

}

using namespace Light;
sol::table EventBinding(sol::this_state s)
{
	sol::state_view lua(s);
	sol::table module = lua.create_table();

	sol::table evType = module.create("EventType");
	evType["EvtNewActor"] = events::EvtNewActor::StaticType;
	evType["EvtDestroyActor"] = events::EvtDestroyActor::StaticType;
	evType["EvtMoveActor"] = events::EvtMoveActor::StaticType;
	//evType["EvtCameraCreate"] = events::EvtCameraCreate::StaticType;
	//evType["EvtSceneCreate"] = events::EvtSceneCreate::StaticType;
	evType["EvtKeyEvent"] = events::EvtKeyEvent::StaticType;
	evType["EvtMouseMove"] = events::EvtMouseMove::StaticType;

	module.new_usertype<events::EvtNewActor>("EvtNewActor","GetID",&events::EvtNewActor::GetID);
	module.new_usertype<events::EvtDestroyActor>("EvtDestroyActor", "GetID", &events::EvtDestroyActor::GetActor);
	module.new_usertype<events::EvtMoveActor>("EvtMoveActor", "GetID", &events::EvtMoveActor::GetId, "GetMatrix",&events::EvtMoveActor::GetMatrix);
	//lua.new_usertype<events::EvtCameraCreate>("EvtCameraCreate", "GetID", &events::EvtCameraCreate::GetID);
	//lua.new_usertype<events::EvtSceneCreate>("EvtSceneCreate", "GetID", &events::EvtNewActor::GetID);
	module.new_usertype<events::EvtKeyEvent>("EvtKeyEvent", "key", &events::EvtKeyEvent::key, "action", &events::EvtKeyEvent::action, "dt", &events::EvtKeyEvent::dt);
	module.new_usertype<events::EvtMouseMove>("EvtMouseMove", "x", &events::EvtMouseMove::x, "y", &events::EvtMouseMove::y,"dx", &events::EvtMouseMove::dx, "dy", &events::EvtMouseMove::dy);


	return module;

}