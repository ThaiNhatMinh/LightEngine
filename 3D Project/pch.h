#pragma once

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
#pragma comment(lib, "lib/glfw3.lib")
#pragma comment(lib, "lib/Devil.lib")
#pragma comment(lib, "opengl32.lib")

// Math
#include "Math\Vector.h"
#include "Math\Quaternion.h"
#include "Math\Matrix.h"
#include "Math\EulerAngles.h"
#include "Math\CMath.h"

#include "Math\AABB.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"
#include "Math\Sphere.h"

#include "Utilities\Module.h"
#include "Utilities\Log.h"
#include "Utilities\Input.h"
#include "Utilities\Shader.h"
#include "Utilities\Timer.h"

#include "ResourceCache\ModelCache.h"
#include "ResourceCache\Texture.h"
#include "ResourceCache\ResourceLoader.h"

#include "Graphics3D\Material.h"
#include "Graphics3D\Mesh.h"
#include "Graphics3D\SkeMesh.h"
#include "Graphics3D\AnimationDef.h"
#include "Graphics3D\SkeAnimation.h"

#include "Core\Camera.h"
#include "Core\Windows.h"
#include "Core\CoreApplication.h"