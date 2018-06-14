#pragma once

#include "Common.h"
#include "GameManager.h"
#include "ObjectManager.h"
#include "Input.h"
#include "Sound.h"
#include "Math.h"
#include "RenderTarget.h"
#include "RenderSpace.h"
#include "Renderer.h"
#include "Time.h"
#include "Vector.h"
#include "Color.h"
#include "Object.h"
#include "ObjectType.h"
#include "Polygon.h"
#include "Billboard.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Camera.h"
#include "Event.h"
#include "Shader.h"
#include "ParticleSystem.h"
#include "StaticModel.h"
#include "SkinnedModel.h"
#include "SoundPlayer.h"

#include <vector>
#include <list>
#include <memory>

#ifdef IMGUI
#include "../Imgui/ImGuiImpl.h"
#endif

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
