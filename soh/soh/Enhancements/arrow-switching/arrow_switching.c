#include "arrow_switching.h"

#include <global.h>


s32 func_808351D4(Player* this, PlayState* play); // Arrow nocked
s32 func_808353D8(Player* this, PlayState* play); // Aiming in first person

typedef struct {
    u8 asArrow;
    u8 asBowArrow;
} ArrowItems;

static ArrowItems arrowTypeToItem[] = {
    /* normal arrows */ { ITEM_BOW, ITEM_BOW },
    /* fire arrows */ { ITEM_ARROW_FIRE, ITEM_BOW_ARROW_FIRE },
    /* ice arrows */ { ITEM_ARROW_ICE, ITEM_BOW_ARROW_ICE },
    /* light arrows */ { ITEM_ARROW_LIGHT, ITEM_BOW_ARROW_LIGHT },
    /* unused arrow types are excluded from cycling */
};

// Returns true if the player is in a state where they can switch arrows.
// Specifically, the gArrowSwitching CVar is enabled, the player is holding the
// bow with normal, fire, ice, or light arrows, and they're either aiming or
// have an arrow notched.
bool ArrowSwitching_CanSwitch(Player* player) {
    if (!CVarGetInteger("gArrowSwitching", 0)) {
        return false;
    }

    if (player->heldItemAction < PLAYER_IA_BOW || player->heldItemAction > PLAYER_IA_BOW_LIGHT) {
        return false;
    }

    return player->func_82C == func_808351D4 || player->func_82C == func_808353D8;
}

bool ArrowSwitching_Next(u8 currentItemAction, u8* item, u8* itemAction) {
    const u8 arrowCount = ARRAY_COUNT(arrowTypeToItem);
    u8 heldArrowIA = currentItemAction - PLAYER_IA_BOW;
    u8 i;
    for (i = 1; i < arrowCount; i++) {
        u8 arrowIA = (heldArrowIA + i) % arrowCount;
        ArrowItems items = arrowTypeToItem[arrowIA];
        if (INV_CONTENT(items.asArrow) != ITEM_NONE) {
            *item = items.asBowArrow;
            *itemAction = PLAYER_IA_BOW + arrowIA;
            break;
        }
    }
    return i != arrowCount;
}
