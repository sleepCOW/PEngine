#pragma once


/**
 * Low level includes.
 */

#include "CoreTypes.h"
#include "CoreMemory.h"
#include "CoreDefines.h"
#include "CoreGlobals.h"

/**
* Forward declarations
*/

class CObject;
class CComponent;
class CObjectManager;

/**
* Typedefs
*/

typedef union SDL_Event SDL_Event;

/**
* Commonly used headers
*/

#include "Core/Public/AssetManager.h"
#include "Core/Public/ReflectionManager.h"
#include <cassert>