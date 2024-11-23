#ifndef ENHANCEMENTS_ARROW_SWITCHING_H
#define ENHANCEMENTS_ARROW_SWITCHING_H

#include <z64.h>

bool ArrowSwitching_CanSwitch(Player*);
bool ArrowSwitching_Next(u8 currentItemAction, u8* item, u8* itemAction);

#endif
