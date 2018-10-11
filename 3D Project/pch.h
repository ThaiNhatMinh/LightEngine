#pragma once

// Precompile header file
// Thai Nhat Minh 
// Game 3D Project
// Day begin 31/5/2017

#ifdef _DEBUG
#define  _CRTDBG_MAP_ALLOC 1
#include <cstdlib>
#include <crtdbg.h>
#endif
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
//#define new DEBUG_NEW
#endif

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

// Lua Plus 5.1
//#include <LuaPlus.h>
// OpenGL Include
// GLEW

// ImGui
//#include <imgui.h>



// libary
//#pragma comment(lib, "glew32s.lib")
//#pragma comment(lib, "lib/glfw3.lib")
//#pragma comment(lib, "lib/Devil.lib")
//#pragma comment(lib, "lib/tinyxml2.lib")
//#pragma comment(lib, "lib/dinput8.lib")
//#pragma comment(lib, "lib/dxguid.lib")
//#pragma comment(lib, "opengl32.lib")



#include "mat4x4.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "vec2.hpp"

#include "gtc\quaternion.hpp"
#include "gtc\matrix_transform.hpp"
#include "gtc\type_ptr.hpp"
#include "gtc\constants.hpp"

#include "gtx\quaternion.hpp"
#include "gtx\compatibility.hpp"
#include "gtx\euler_angles.hpp"
#include "geometric.hpp"
#include "detail\func_matrix.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::quat;


// Interface
#include "interface.h"

#include "Core\Log.h"
//#include "EngineHeader.h"