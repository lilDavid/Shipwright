#ifndef MODS_HPP
#define MODS_HPP

#include <vector>
#include <functional>
#include <libultraship/libultraship.h>
#include "soh/UIWidgets.hpp"
#include "soh/Enhancements/game-interactor/GameInteractor.h"
#include "soh/Enhancements/cosmetics/CosmeticsEditor.h"

inline std::vector<std::function<void()>> modDrawFuncs = {};
inline std::vector<std::function<void()>> modRegisterFuncs = {};

inline void DrawModsMenu() {
    if (modDrawFuncs.empty()) {
        return;
    }

    if (ImGui::BeginMenu("Mods")) {
        for (auto& drawFunc : modDrawFuncs) {
            ImGui::PushID(&drawFunc);
            drawFunc();
            ImGui::PopID();
        }
        ImGui::EndMenu();
    }
}

inline void RegisterMods() {
    for (auto& regFunc : modRegisterFuncs) {
        regFunc();
    }
}

struct Mod {
    Mod(std::function<void()> drawFunc, std::function<void()> registerFunc) {
        modDrawFuncs.push_back(drawFunc);
        modRegisterFuncs.push_back(registerFunc);
    }
};

#endif //MODS_HPP
