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

#include <GamePlugin/IGamePlugin.h>
#include <EngineHeader.h>

#include "WeaponResource.h"
#include "CharacterResource.h"

#include "Creature.h"
#include "Player.h"

#include "ZombieController.h"
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