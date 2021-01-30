/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019-20 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <magic_enum.hpp>
#include <slavestodarkness/SlavesToDarkness.h>
#include <Board.h>

#include "slavestodarkness/ChaosChosen.h"
#include "slavestodarkness/ChaosKnights.h"
#include "slavestodarkness/ChaosMarauders.h"
#include "slavestodarkness/ChaosWarriors.h"
#include "slavestodarkness/CorvusCabal.h"
#include "slavestodarkness/CypherLords.h"
#include "slavestodarkness/Furies.h"
#include "slavestodarkness/IronGolems.h"
#include "slavestodarkness/Raptoryx.h"
#include "slavestodarkness/Slambo.h"
#include "slavestodarkness/SplinteredFang.h"
#include "slavestodarkness/TheUnmade.h"
#include "slavestodarkness/UntamedBeasts.h"
#include "slavestodarkness/ChaosLordOnKarkadrak.h"
#include "slavestodarkness/ChaosLordOnManticore.h"
#include "slavestodarkness/ChaosWarshrine.h"
#include "slavestodarkness/ChaosSorcererOnManticore.h"
#include "slavestodarkness/ChaosSorcerer.h"
#include "slavestodarkness/DarkoathChieftain.h"
#include "slavestodarkness/DarkoathWarqueen.h"
#include "slavestodarkness/ExaltedHeroOfChaos.h"
#include "slavestodarkness/ChaosLordOnDaemonicMount.h"
#include "slavestodarkness/ChaosLord.h"
#include "slavestodarkness/ChaosChariots.h"
#include "slavestodarkness/GorebeastChariots.h"
#include "slavestodarkness/SpireTyrants.h"
#include "slavestodarkness/MindstealerSphiranx.h"
#include "slavestodarkness/FomoroidCrusher.h"
#include "slavestodarkness/OgroidMyrmidon.h"
#include "slavestodarkness/Belakor.h"
#include "slavestodarkness/DaemonPrince.h"
#include "slavestodarkness/GodswornHunt.h"
#include "slavestodarkness/Slaughterbrute.h"
#include "slavestodarkness/SoulGrinder.h"
#include "slavestodarkness/TheddraSkullScryer.h"
#include "slavestodarkness/VortexBeast.h"
#include "slavestodarkness/ScionsOfTheFlame.h"
#include "slavestodarkness/KhagrasRavagers.h"

#include "everchosen/Archaon.h"
#include "everchosen/Varanguard.h"


namespace SlavesToDarkness {

    std::string SlavesToDarknessBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Damned Legion") {
            auto legionName = magic_enum::enum_name((DamnedLegion)parameter.intValue);
            return std::string(legionName);
        } else if (std::string(parameter.name) == "Mark of Chaos") {
            auto markName = magic_enum::enum_name((MarkOfChaos)parameter.intValue);
            return std::string(markName);
        } else if (std::string(parameter.name) == "Lore") {
            auto loreName = magic_enum::enum_name((Lore)parameter.intValue);
            return std::string(loreName);
        }
        return ParameterValueToString(parameter);
    }

    int SlavesToDarknessBase::EnumStringToInt(const std::string &enumString) {
        auto legion = magic_enum::enum_cast<DamnedLegion>(enumString);
        if (legion.has_value()) return (int)legion.value();

        auto mark = magic_enum::enum_cast<MarkOfChaos>(enumString);
        if (mark.has_value()) return (int)mark.value();

        auto lore = magic_enum::enum_cast<Lore>(enumString);
        if (lore.has_value()) return (int)lore.value();

        return 0;
    }

    void SlavesToDarknessBase::setDamnedLegion(DamnedLegion legion) {
        removeKeyword(RAVAGERS);
        removeKeyword(CABALISTS);
        removeKeyword(DESPOILERS);
        removeKeyword(HOST_OF_THE_EVERCHOSEN);

        m_legion = legion;
        switch (legion) {
            case DamnedLegion::Ravagers:
                addKeyword(RAVAGERS);
                break;
            case DamnedLegion::Cabalists:
                addKeyword(CABALISTS);
                break;
            case DamnedLegion::Despoilers:
                addKeyword(DESPOILERS);
                break;
            case DamnedLegion::Host_Of_The_Everchosen:
                addKeyword(HOST_OF_THE_EVERCHOSEN);
                break;
            default:
                break;
        }
    }

    void SlavesToDarknessBase::setMarkOfChaos(MarkOfChaos mark) {
        removeKeyword(UNDIVIDED);
        removeKeyword(NURGLE);
        removeKeyword(KHORNE);
        removeKeyword(SLAANESH);
        removeKeyword(TZEENTCH);

        m_markOfChaos = mark;

        switch (mark) {
            case MarkOfChaos::Undivided:
                addKeyword(UNDIVIDED);
                break;
            case MarkOfChaos::Nurgle:
                addKeyword(NURGLE);
                break;
            case MarkOfChaos::Khorne:
                addKeyword(KHORNE);
                break;
            case MarkOfChaos::Slaanesh:
                addKeyword(SLAANESH);
                break;
            case MarkOfChaos::Tzeentch:
                addKeyword(TZEENTCH);
                break;
        }
    }

    Rerolls SlavesToDarknessBase::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Aura of Khorne
        if (hasKeyword(KHORNE)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(KHORNE)) {
                return Reroll_Ones;
            }
        }
        return Unit::toHitRerolls(weapon, target);
    }

    int SlavesToDarknessBase::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);

        // Aura of Khorne
        if (hasKeyword(KHORNE)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(KHORNE) && hero->isGeneral()) {
                mod++;
            }
        }

        return mod;
    }

    Rerolls SlavesToDarknessBase::toSaveRerolls(const Weapon *weapon) const {
        // Aura of Tzeentch
        if (hasKeyword(TZEENTCH)) {
            auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 12.0);
            if (hero && hero->hasKeyword(TZEENTCH)) {
                return Reroll_Ones;
            }
        }
        return Unit::toSaveRerolls(weapon);
    }

    void Init() {
        Archaon::Init();
        Varanguard::Init();

        ChaosChosen::Init();
        ChaosKnights::Init();
        ChaosMarauders::Init();
        ChaosWarriors::Init();
        CorvusCabal::Init();
        CypherLords::Init();
        Furies::Init();
        IronGolems::Init();
        Raptoryx::Init();
        Slambo::Init();
        SplinteredFang::Init();
        TheUnmade::Init();
        UntamedBeasts::Init();
        ChaosLordOnKarkadrak::Init();
        ChaosLordOnManticore::Init();
        ChaosWarshrine::Init();
        ChaosSorcererOnManticore::Init();
        ChaosSorcerer::Init();
        DarkoathChieftain::Init();
        DarkoathWarqueen::Init();
        ExaltedHeroOfChaos::Init();
        ChaosLordOnDaemonicMount::Init();
        ChaosLord::Init();
        ChaosChariots::Init();
        GorebeastChariots::Init();
        SpireTyrants::Init();
        FomoroidCrusher::Init();
        MindstealerSphiranx::Init();
        OgroidMyrmidon::Init();
        Belakor::Init();
        DaemonPrince::Init();
        GodswornHunt::Init();
        Slaughterbrute::Init();
        SoulGrinder::Init();
        TheddraSkullscryer::Init();
        MutalithVortexBeast::Init();
        ScionsOfTheFlame::Init();
        KhagrasRavagers::Init();
    }

} //namespace SlavesToDarkness
