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
//#pragma comment(lib, "glew32s.lib")
//#pragma comment(lib, "lib/glfw3.lib")
//#pragma comment(lib, "lib/Devil.lib")
//#pragma comment(lib, "lib/tinyxml2.lib")
//#pragma comment(lib, "lib/dinput8.lib")
//#pragma comment(lib, "lib/dxguid.lib")
//#pragma comment(lib, "opengl32.lib")

#ifdef BUILDING_MYDLL
#define MYDLL_API __declspec(dllexport)
#else
#define MYDLL_API __declspec(dllimport)
#endif


#include "glm\mat4x4.hpp"
#include "glm\vec3.hpp"
#include "glm\vec4.hpp"
#include "glm\vec2.hpp"

#include "glm\gtc\quaternion.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\constants.hpp"

#include "glm\gtx\quaternion.hpp"
#include "glm\gtx\compatibility.hpp"
#include "glm\gtx\euler_angles.hpp"
#include "glm\geometric.hpp"
#include "glm\detail\func_matrix.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;


// Interface
#include "interface.h"


#include "EngineHeader.h"