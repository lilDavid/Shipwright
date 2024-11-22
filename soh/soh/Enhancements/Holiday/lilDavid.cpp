#include "Holiday.hpp"

extern "C" {
#include "macros.h"
#include "functions.h"
#include "variables.h"
extern PlayState* gPlayState;
}

#include "src/overlays/actors/ovl_En_Arrow/z_en_arrow.h"
#include "src/overlays/actors/ovl_En_Bom/z_en_bom.h"

extern "C" {
    void func_809B45E0(EnArrow*, PlayState*);
    void func_809B4640(EnArrow*, PlayState*);
}

#define AUTHOR "lilDavid"
#define CVAR(v) "gHoliday." AUTHOR "." v

static void OnConfigurationChanged() {
    COND_ID_HOOK(OnActorInit, ACTOR_EN_ARROW, CVarGetInteger(CVAR("BombArrows"), 0), [](void* actorRef) {
        EnArrow* arrow = (EnArrow*) actorRef;
        if (arrow->actor.params != ARROW_NORMAL)
            return;

        if (AMMO(ITEM_BOMB) == 0)
            return;

        EnBom* bomb = (EnBom*) Actor_SpawnAsChild(&gPlayState->actorCtx, &arrow->actor, gPlayState, ACTOR_EN_BOM,
                arrow->actor.world.pos.x, arrow->actor.world.pos.y, arrow->actor.world.pos.z,
                0, 0, 0, BOMB_BODY);
        if (bomb == nullptr)
            return;

        Actor_SetScale(&bomb->actor, 0.003f);
        bomb->timer = 65;
    });

    COND_ID_HOOK(OnActorUpdate, ACTOR_EN_ARROW, CVarGetInteger(CVAR("BombArrows"), 0), [](void* actorRef) {
        EnArrow* arrow = (EnArrow*) actorRef;
        if (!arrow->actor.child || arrow->actor.child->id != ACTOR_EN_BOM)
            return;

        EnBom* bomb = (EnBom*) arrow->actor.child;
        bomb->actor.world.pos = arrow->actor.world.pos;
        f32 r = 8.0f;
        f32 xrot = arrow->actor.world.rot.x;
        f32 yrot = arrow->actor.world.rot.y;
        bomb->actor.world.pos.x += r * Math_CosS(xrot) * Math_SinS(yrot);
        bomb->actor.world.pos.y -= r * Math_SinS(xrot) + 2.0f;
        bomb->actor.world.pos.z += r * Math_CosS(xrot) * Math_CosS(yrot);

        if (arrow->actor.parent == nullptr) {
            if (bomb->timer > 60) {
                Inventory_ChangeAmmo(ITEM_BOMB, -1);
            }
            bomb->timer = 52;
        } else {
            bomb->timer = 62;
        }

        if (arrow->actionFunc == func_809B45E0 ||
            arrow->actionFunc == func_809B4640 ||
            arrow->actor.params == ARROW_NORMAL_LIT)
        {
            arrow->actor.child = nullptr;
            bomb->actor.parent = nullptr;
            bomb->timer = 2;
            Actor_Kill(&arrow->actor);
        }
    });

    COND_ID_HOOK(OnActorKill, ACTOR_EN_ARROW, CVarGetInteger(CVAR("BombArrows"), 0), [](void* actorRef) {
        EnArrow* arrow = (EnArrow*) actorRef;
        if (!arrow->actor.child || arrow->actor.child->id != ACTOR_EN_BOM)
            return;
        Actor_Kill(arrow->actor.child);
    });

    COND_ID_HOOK(OnActorUpdate, ACTOR_EN_BOM, CVarGetInteger(CVAR("BombArrows"), 0), [](void* actorRef) {
        EnBom* bomb = (EnBom*) actorRef;
        if (!bomb->actor.parent || bomb->actor.parent->id != ACTOR_EN_ARROW)
            return;

        if (bomb->timer > 55 && bomb->timer < 60)
            bomb->timer += 4;
        if (bomb->timer > 45 && bomb->timer < 50)
            bomb->timer += 4;
    });
}

static void DrawMenu() {
    ImGui::SeparatorText(AUTHOR);
    if (UIWidgets::EnhancementCheckbox("Bomb Arrows", CVAR("BombArrows"))) {
        OnConfigurationChanged();
    }
}

static void RegisterMod() {
    // #region Leave this alone unless you know what you are doing
    OnConfigurationChanged();
    // #endregion
}

static Holiday holiday(DrawMenu, RegisterMod);
