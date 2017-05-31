#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>
#include <Windows.h>
// this file include basic I/O and basic data struct: string, vector
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

// OpenGL Include
// GLEW
#define GLEW_STATIC
#include "include/GL/glew.h"
// GLFW
#include "include/GLFW/glfw3.h"

#pragma comment(lib, "lib/glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "lib/glfw3.lib")

// Math
#include "Math\Vector.h"
#include "Math\Matrix.h"
#include "Math\Quaternion.h"
#include "Math\CMath.h"

#include "Math\AABB.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"

#include "Utilities\Module.h"
#include "Utilities\Log.h"
#include "Utilities\Input.h"
#include "Utilities\Shader.h"
#include "Utilities\Timer.h"

#include "Core\Camera.h"
#include "Core\Windows.h"