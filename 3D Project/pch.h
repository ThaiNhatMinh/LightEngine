#pragma once

// Precompile header file
// Thai Nhat Minh 
// Game 3D Project
// Day begin 31/5/2017

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <Windows.h>
#include <wchar.h>
#include <cassert>
#include <iomanip>
#include <set>
#include <iterator>
#include <algorithm>
#include <list>
#include <strstream>
#include <functional>
#include <memory>
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;
#pragma warning(disable: 4244) 
// OpenGL Include
// GLEW
#define GLEW_STATIC
#include <gl\glew.h>

// GLFW
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>
// TinyXML 2
#include <tinyxml2.h>

// Direct Input
#include <dinput.h>

// Bullet Physic 
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>


#include <FastDelegate\FastDelegate.h>
using fastdelegate::MakeDelegate;

// Lua Plus 5.1
//#include <LuaPlus.h>


// ImGui
#include <imgui\imgui.h>



// libary
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "lib/glfw3.lib")
#pragma comment(lib, "lib/Devil.lib")
#pragma comment(lib, "lib/tinyxml2.lib")
#pragma comment(lib, "lib/dinput8.lib")
#pragma comment(lib, "lib/dxguid.lib")
#pragma comment(lib, "opengl32.lib")

#ifdef BUILDING_MYDLL
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif

// Math
#include <glm\mat4x4.hpp>
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"
#include "glm\vec2.hpp"

#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\constants.hpp"

#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\compatibility.hpp"
#include <glm\gtx\euler_angles.hpp>
#include "glm\geometric.hpp"
#include "glm\detail\func_matrix.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;

#include "Math\AABB.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"
#include "Math\Ray.h"
#include "Math\Math.h"
// Interface
#include "interface.h"

#include "Interface\IPhysic.h"
#include "Interface\IRenderer.h"
#include "Interface\IContext.h"

#include "Utilities\Module.h"
#include "Core\Log.h"
#include "Utilities\Input.h"
#include "Core\DirectInput.h"
#include "Utilities\Shader.h"
#include "Utilities\Timer.h"
#include "Utilities\String.h"
#include "Utilities\Template.h"
#include "Core\Debug.h"
#include "Utilities\PhysicsUtilities.h"
#include "Core\Console.h"

#include "ResourceCache\ModelCache.h"
#include "ResourceCache\Texture.h"
#include "ResourceCache\ResourceLoader.h"


#include "VGUI\UI.h"

#include "Graphics3D\ICamera.h"
#include "Graphics3D\Material.h"
#include "Graphics3D\Mesh.h"
#include "Graphics3D\SkeMesh.h"
//#include "Graphics3D\AnimationDef.h"
//#include "Graphics3D\SkeAnimation.h"
#include "Graphics3D\Light.h"
#include "Graphics3D\TexShader.h"
#include "Graphics3D\PrimitiveShader.h"
#include "Graphics3D\SkeShader.h"
#include "Graphics3D\ImGuiShader.h"
#include "Graphics3D\DebugShader.h"

#include "Graphics3D\SpriteShader.h"
#include "Graphics3D\Particles.h"
#include "Graphics3D\Sprite.h"
#include "Graphics3D\SpriteAnim.h"
#include "Graphics3D\EffectSystem.h"
#include "Utilities\PrimitiveShape\Cube.h"
#include "Utilities\PrimitiveShape\Line.h"

// Lua Scripting
//#include "LuaScripting\LuaStateManager.h"
//#include "LuaScripting\ScriptEvents.h"
//#include "LuaScripting\ScriptExports.h"
//#include "EventManager\TestScriptEvent.h"

#include "Sound\SoundEngine.h"
#include "Sound\SoundListener.h"
#include "Sound\SoundSource3D.h"

#include "EventManager\EventManager.h"
#include "EventManager\AnimationEvent.h"
#include "EventManager\PhysicsEvents.h"
#include "EventManager\Events.h"

#include "GameObject\ActorComponent.h"
#include "GameObject\ColliderComponent.h"
#include "GameObject\TransformComponent.h"
#include "GameObject\MeshRenderComponent.h"
#include "GameObject\TerrainRenderComponent.h"
#include "GameObject\PhysicComponent.h"
#include "GameObject\RigidBodyComponent.h"
#include "GameObject\AnimationComponent.h"
#include "GameObject\CharacterControllerComponent.h"
#include "GameObject\CameraComponent.h"
#include "GameObject\LogicComponent.h"
#include "GameObject\ActorFactory.h"
#include "GameObject\Actor.h"
#include "GameObject\StaticObject.h"
#include "Physic\PhysicDebugDrawer.h"
#include "Physic\Physic.h"

#include "Graphics3D\Scene.h"
#include "Graphics3D\Camera.h"

#include "World\TerrainWorld.h"


#include "Core\SystemUI.h"
#include "Core\Windows.h"
#include "Core\Application.h"
#include "Core\Context.h"
#include "GameHeader.h"