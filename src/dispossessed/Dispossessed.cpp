/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Dispossessed.h>

#include "dispossessed/Hammerers.h"
#include "dispossessed/Ironbreakers.h"
#include "dispossessed/Irondrakes.h"
#include "dispossessed/Quarrellers.h"
#include "dispossessed/Longbeards.h"
#include "dispossessed/Warriors.h"
#include "dispossessed/Thunderers.h"
#include "dispossessed/Unforged.h"
#include "dispossessed/WardenKing.h"
#include "dispossessed/Runelord.h"

namespace Dispossessed {

    void Dispossessed::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        // Stubborn to the End
        if (roll <= 3) {
            numFled = 0;
            numAdded = 0;
        } else {
            Unit::computeBattleshockEffect(roll, numFled, numAdded);
        }
    }

    Rerolls Dispossessed::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        switch (m_grudge) {
            case StuckUp:
                if (target->hasKeyword(HERO)) {
                    return RerollOnes;
                }
                break;
            case SpeedMerchants:
                if (target->move() >= 10) {
                    return RerollOnes;
                }
                break;
            case MonstrousCheaters:
                if (target->hasKeyword(MONSTER)) {
                    return RerollOnes;
                }
                break;
            case CowardlyHorders:
                if (target->initialModels() >= 20) {
                    return RerollOnes;
                }
                break;
            case ShoddyCraftsmanship:
                if (target->save() >= 2 && target->save() <= 4) {
                    return RerollOnes;
                }
                break;
            case SneakyAmbushers:
                // TODO:
                break;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    void Dispossessed::setGrudge(Dispossessed::Grudge grudge) {
        m_grudge = grudge;
    }

    std::string Dispossessed::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Grudge") {
            if (parameter.intValue == StuckUp) { return "Stuck-up"; }
            else if (parameter.intValue == SpeedMerchants) { return "Speed Merchants"; }
            else if (parameter.intValue == MonstrousCheaters) { return "Monstrous Cheaters"; }
            else if (parameter.intValue == CowardlyHorders) { return "Cowardly Horders"; }
            else if (parameter.intValue == ShoddyCraftsmanship) { return "Shoddy Craftsmanship"; }
            else if (parameter.intValue == SneakyAmbushers) { return "Sneaky Ambushers"; }
        }
        return ParameterValueToString(parameter);
    }

    int Dispossessed::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Stuck-up") { return StuckUp; }
        else if (enumString == "Speed Merchants") { return SpeedMerchants; }
        else if (enumString == "Monstrous Cheaters") { return MonstrousCheaters; }
        else if (enumString == "Cowardly Horders") { return CowardlyHorders; }
        else if (enumString == "ShoddyCraftsmanship") { return ShoddyCraftsmanship; }
        else if (enumString == "Sneaky Ambushers") { return SneakyAmbushers; }
        return 0;
    }

    void Init() {
        Hammerers::Init();
        Ironbreakers::Init();
        Irondrakes::Init();
        Longbeards::Init();
        Quarrellers::Init();
        Thunderers::Init();
        Warriors::Init();
        Unforged::Init();
        Runelord::Init();
        WardenKing::Init();
    }

} //namespace Dispossessed