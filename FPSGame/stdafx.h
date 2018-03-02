// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

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
#include <set>
#include <iterator>
#include <algorithm>
#include <list>
#include <strstream>
#include <functional>
#include <memory>

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>
#include <glm\vec4.hpp>
#include <glm\vec2.hpp>

#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\constants.hpp>

#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\compatibility.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\geometric.hpp>
#include <glm\detail\func_matrix.hpp>

#include <GamePlugin/IGamePlugin.h>
#include <EngineHeader.h>

#include "WeaponResource.h"
#include "CharacterResource.h"

#include "Creature.h"
#include "Player.h"


#include "LocalPlayerComponent.h"

#include "Weapon.h"

#include "EvtData.h"
#include "Zombie.h"
#include "AI_Explosive.h"
#include "PlayerView.h"
#include "GunPlayerView.h"
#include "Game.h"

// TODO: reference additional headers your program requires here

extern "C" __declspec(dllexport) IGamePlugin* CreateInterface();