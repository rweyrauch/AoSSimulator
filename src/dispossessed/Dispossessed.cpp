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

#include <magic_enum.hpp>

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
            case Grudge::Stuck_Up:
                if (target->hasKeyword(HERO)) {
                    return RerollOnes;
                }
                break;
            case Grudge::Speed_Merchants:
                if (target->move() >= 10) {
                    return RerollOnes;
                }
                break;
            case Grudge::Monstrous_Cheaters:
                if (target->hasKeyword(MONSTER)) {
                    return RerollOnes;
                }
                break;
            case Grudge::Cowardly_Horders:
                if (target->initialModels() >= 20) {
                    return RerollOnes;
                }
                break;
            case Grudge::Shoddy_Craftsmanship:
                if (target->save() >= 2 && target->save() <= 4) {
                    return RerollOnes;
                }
                break;
            case Grudge::Sneaky_Ambushers:
                // TODO:
                break;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    void Dispossessed::setGrudge(Grudge grudge) {
        m_grudge = grudge;
    }

    std::string Dispossessed::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Grudge") {
            auto grudgeName = magic_enum::enum_name((Grudge)parameter.intValue);
            return std::string(grudgeName);
        }
        return ParameterValueToString(parameter);
    }

    int Dispossessed::EnumStringToInt(const std::string &enumString) {
        auto grudge = magic_enum::enum_cast<Grudge>(enumString);
        if (grudge.has_value()) return (int)grudge.value();
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