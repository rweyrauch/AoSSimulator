/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <Board.h>
#include "ossiarch/OssiarchBonereaperBase.h"

#include "ossiarch/MortekGuard.h"
#include "ossiarch/NecropolisStalkers.h"
#include "ossiarch/Vokmortian.h"
#include "ossiarch/ImmortisGuard.h"
#include "ossiarch/KavalosDeathriders.h"
#include "ossiarch/GothizzarHarvester.h"
#include "ossiarch/MortekCrawler.h"
#include "ossiarch/Katakros.h"
#include "ossiarch/LiegeKavalos.h"
#include "ossiarch/KavalosZandtos.h"
#include "ossiarch/Boneshaper.h"
#include "ossiarch/Soulmason.h"
#include "ossiarch/Soulreaper.h"
#include "ossiarch/MorghastArchai.h"
#include "ossiarch/MorghastHarbingers.h"
#include "ossiarch/ArkhanTheBlack.h"

namespace OssiarchBonereapers {

    std::string OssiarchBonereaperBase::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Legion") {
            if (parameter.intValue == MortisPraetorians) return "Mortis Praetorians";
            else if (parameter.intValue == PetrifexElite) return "Petrifex Elite";
            else if (parameter.intValue == StalliarchLords) return "Stalliarch Lords";
            else if (parameter.intValue == IvoryHost) return "Ivory Host";
            else if (parameter.intValue == NullMyriad) return "Null Myriad";
            else if (parameter.intValue == Crematorians) return "Crematorians";
        }
        return ParameterValueToString(parameter);
    }

    int OssiarchBonereaperBase::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Mortis Praetorians") return MortisPraetorians;
        else if (enumString == "Petrifex Elite") return PetrifexElite;
        else if (enumString == "Stalliarch Lords") return StalliarchLords;
        else if (enumString == "Ivory Host") return IvoryHost;
        else if (enumString == "Null Myriad") return NullMyriad;
        else if (enumString == "Crematorians") return Crematorians;
        return 0;
    }

    void OssiarchBonereaperBase::setLegion(Legion legion) {
        removeKeyword(MORTIS_PRAETORIANS);
        removeKeyword(PETRIFEX_ELITE);
        removeKeyword(STALLIARCH_LORDS);
        removeKeyword(IVORY_HOST);
        removeKeyword(NULL_MYRIAD);
        removeKeyword(CREMATORIANS);

        m_legion = legion;
        switch (legion) {
            case MortisPraetorians:
                addKeyword(MORTIS_PRAETORIANS);
                break;
            case PetrifexElite:
                addKeyword(PETRIFEX_ELITE);
                break;
            case StalliarchLords:
                addKeyword(STALLIARCH_LORDS);
                break;
            case IvoryHost:
                addKeyword(IVORY_HOST);
                break;
            case NullMyriad:
                addKeyword(NULL_MYRIAD);
                break;
            case Crematorians:
                addKeyword(CREMATORIANS);
                break;
            default:
                break;
        }
    }

    Wounds OssiarchBonereaperBase::applyWoundSave(const Wounds &wounds) {
        // Deathless Warriors
        auto hekatos = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HEKATOS, 6.0);
        auto hero = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), HERO, 16.0);

        Wounds totalWounds = wounds;

        if (hasKeyword(HEKATOS) || hekatos || hero) {
            Dice::RollResult woundSaves, mortalSaves;
            Dice::rollD6(wounds.normal, woundSaves);
            Dice::rollD6(wounds.mortal, mortalSaves);

            totalWounds.normal -= woundSaves.rollsGE(6);
            totalWounds.normal = std::max(totalWounds.normal, 0);
            totalWounds.mortal -= mortalSaves.rollsGE(6);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);
        }
        return totalWounds;
    }

    void Init() {
        ArchKavalosZandtos::Init();
        ArkhanTheBlack::Init();
        GothizzarHarvester::Init();
        ImmortisGuard::Init();
        KavalosDeathriders::Init();
        LiegeKavalos::Init();
        MorghastArchai::Init();
        MorghastHarbingers::Init();
        MortekCrawler::Init();
        MortekGuard::Init();
        MortisanBoneshaper::Init();
        MortisanSoulmason::Init();
        MortisanSoulreaper::Init();
        NecropolisStalkers::Init();
        Katakros::Init();
        Vokmortian::Init();
    }

} // namespace OssiarchBonereapers
