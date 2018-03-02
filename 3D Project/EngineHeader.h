#pragma once
// Math




#include "Math\AABB.h"
#include "Math\Plane.h"
#include "Math\Frustum.h"
#include "Math\Ray.h"
#include "Math\Math.h"


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
#include "EventManager\EffectEvent.h"

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
#include "Physic\HitBox.h"

#include "Graphics3D\Scene.h"
#include "Graphics3D\Camera.h"

#include "World\TerrainWorld.h"


#include "Core\Windows.h"
#include "Core\Application.h"
#include "Core\Context.h"
#include "Core\SystemUI.h"
