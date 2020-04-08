/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/LegionOfNagash.h>
#include <Board.h>

#include "death/Nagash.h"
#include "death/BlackKnights.h"
#include "death/BloodKnights.h"
#include "death/DireWolves.h"
#include "death/GraveGuard.h"
#include "death/SkeletonWarriors.h"
#include "death/Vargheists.h"
#include "death/Zombies.h"
#include "death/WightKingTombBlade.h"
#include "death/WightKingBlackAxe.h"
#include "death/Necromancer.h"
#include "death/BatSwarms.h"
#include "death/BloodseekerPalangquin.h"
#include "death/CorpseCartBrazier.h"
#include "death/CorpseCartLodestone.h"
#include "death/CovenThrone.h"
#include "death/FellBats.h"
#include "death/Mannfred.h"
#include "death/MortisEngine.h"
#include "death/Neferata.h"
#include "death/PrinceVhordrai.h"
#include "death/VampireLord.h"
#include "death/VampireLordOnDragon.h"

namespace Death {

    std::string LegionOfNagashBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Legion") {
            if (parameter.intValue == GrandHostOfNagash) { return "Grand Host of Nagash"; }
            else if (parameter.intValue == LegionOfSacrament) { return "Legion of Sacrament"; }
            else if (parameter.intValue == LegionOfNight) { return "Legion of Night"; }
            else if (parameter.intValue == LegionOfBlood) { return "Legion of Blood"; }
        }
        return ParameterValueToString(parameter);
    }

    int LegionOfNagashBase::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Grand Host of Nagash") return GrandHostOfNagash;
        else if (enumString == "Legion of Sacrament") return LegionOfSacrament;
        else if (enumString == "Legion of Night") return LegionOfNight;
        else if (enumString == "Legion of Blood") return LegionOfBlood;
        return 0;
    }

    void LegionOfNagashBase::setLegion(LegionOfNagashBase::Legion legion) {
        removeKeyword(GRAND_HOST_OF_NAGASH);
        removeKeyword(LEGION_OF_SACRAMENT);
        removeKeyword(LEGION_OF_NIGHT);
        removeKeyword(LEGION_OF_BLOOD);

        m_legion = legion;
        switch (legion) {
            case GrandHostOfNagash:
                addKeyword(GRAND_HOST_OF_NAGASH);
                break;
            case LegionOfSacrament:
                addKeyword(LEGION_OF_SACRAMENT);
                break;
            case LegionOfNight:
                addKeyword(LEGION_OF_NIGHT);
                break;
            case LegionOfBlood:
                addKeyword(LEGION_OF_BLOOD);
                break;
        }
    }

    void LegionOfNagashBase::deathlyInvocations(int numUnits, double range) {

        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), range);
        int unitsHealed = 0;
        for (auto unit : units) {
            if (unit->hasKeyword(SUMMONABLE) && (unit->remainingWounds() < unit->initialWounds())) {
                const auto woundsRestored = Dice::rollD3();
                if (unit->numOfWoundedModels() > 0) {
                    unit->heal(woundsRestored);
                } else {
                    const auto numModels = woundsRestored / unit->wounds();
                    if (numModels)
                        unit->returnModels(numModels);
                }
                unitsHealed++;
            }

            if (unitsHealed >= numUnits) break;
        }
    }

    void Init() {
        Nagash::Init();
        BlackKnights::Init();
        BloodKnights::Init();
        DireWolves::Init();
        GraveGuard::Init();
        SkeletonWarriors::Init();
        Vargheists::Init();
        Zombies::Init();
        WightKingWithBlackAxe::Init();
        WightKingWithBalefulTombBlade::Init();
        Necromancer::Init();
        BatSwarms::Init();
        BloodseekerPalanquin::Init();
        CorpseCartWithBalefireBrazier::Init();
        CorpseCartWithUnholyLodestone::Init();
        CovenThrone::Init();
        FellBats::Init();
        MannfredMortarchOfNight::Init();
        MortisEngine::Init();
        NeferataMortarchOfBlood::Init();
        PrinceVhordrai::Init();
        VampireLord::Init();
        VampireLordOnZombieDragon::Init();
    }

} //namespace Seraphon
