#pragma once

#include "IGamePlugin.h"

#define CREATEINTERFACE_PROCNAME	"CreateInterface"
typedef IGamePlugin* (*CreateInterfaceFn)();