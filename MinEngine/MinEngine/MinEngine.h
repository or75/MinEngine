#pragma once

#include <Windows.h>
#include <intrin.h>
#include <winternl.h>

#include "Api/Api.h"
#include "Memory/Memory.h"
#include "String/String.h"

PVOID GetReturnAddress();