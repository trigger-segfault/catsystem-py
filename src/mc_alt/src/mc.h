#pragma once

#include "LabelInfo.h"
#include "MacroInfo.h"

namespace mc {
	extern LabelInfo* GlobalLabelStorage;
	extern MacroInfo* GlobalMacroStorage;
	extern int GlobalOpenFileCount;
	extern bool OnlyUpdateIfNewer;
	extern bool IncludeLineNumbers;
	extern bool FlagD;
	extern bool EnableCompression;
	extern const unsigned int CharacterFlags[256];
}
