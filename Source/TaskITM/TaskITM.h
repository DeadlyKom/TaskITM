#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(TaskITMDebug, Log, All);

#if     WITH_EDITOR
#define Debug_LOG(Format, ...)          UE_LOG(TaskITMDebug, Log,       Format, ##__VA_ARGS__)
#define Debug_DISPLAY(Format, ...)      UE_LOG(TaskITMDebug, Display,   Format, ##__VA_ARGS__)
#define Debug_WARNING(Format, ...)      UE_LOG(TaskITMDebug, Warning,   Format, ##__VA_ARGS__)
#define Debug_ERROR(Format, ...)        UE_LOG(TaskITMDebug, Error,     Format, ##__VA_ARGS__)
#else
#define Debug_LOG(Format, ...)
#define Debug_DISPLAY(Format, ...)
#define Debug_WARNING(Format, ...)
#define Debug_ERROR(Format, ...)
#endif
