#pragma once

#include "LabelInfo.h"
#include "MacroInfo.h"

namespace mc {
	extern LabelInfo* GlobalLabelStorage;
	extern MacroInfo* GlobalMacroStorage;
	extern int GlobalOpenFileCount;
	extern bool FlagU;
	extern bool FlagL;
	extern bool FlagD;
	extern bool NotFlagX;
	extern const unsigned int CharacterFlags[256];
}
