/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusOldblood.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon {
    static const int BASESIZE = 32;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 110;

    bool SaurusOldblood::s_registered = false;

    SaurusOldblood::SaurusOldblood() :
            SeraphonBase("Saurus Oldblood", 5, WOUNDS, 8, 3, false),
            m_maul(Weapon::Type::Melee, "Celestite Maul", 1, 3, 4, 3, -1, RAND_D3),
            m_warblade(Weapon::Type::Melee, "Celestite Warblade", 1, 5, 3, 3, 0, 1),
            m_warspear(Weapon::Type::Melee, "Celestite Warspear", 2, 4, 3, 3, 0, 1),
            m_greatblade(Weapon::Type::Melee, "Celestite Greatblade", 1, 3, 4, 3, -1, 2),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SAURUS, HERO, OLDBLOOD};
        m_weapons = {&m_maul, &m_warblade, &m_warspear, &m_greatblade, &m_jaws};
    }

    bool SaurusOldblood::configure(WeaponOption option) {
        auto model = new Model(BASESIZE, wounds());
        if (option == CelestiteMaul) {
            model->addMeleeWeapon(&m_maul);
        } else if (option == CelestiteWarblade) {
            model->addMeleeWeapon(&m_warblade);
        } else if (option == CelestiteWarspear) {
            model->addMeleeWeapon(&m_warspear);
        } else if (option == CelestiteGreatblade) {
            model->addMeleeWeapon(&m_greatblade);
        }
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SaurusOldblood::Create(const ParameterList &parameters) {
        auto unit = new SaurusOldblood();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, CelestiteWarblade);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, SeraphonBase::None);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SaurusOldblood::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Weapon", CelestiteWarblade, CelestiteMaul, CelestiteGreatblade, 1},
                            {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne,
                             SeraphonBase::Coalesced, 1},
                            {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None,
                             SeraphonBase::FangsOfSotek, 1}
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Oldblood", factoryMethod);
        }
    }

    std::string SaurusOldblood::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == CelestiteMaul) return "Celestite Maul";
            else if (parameter.intValue == CelestiteWarblade) return "Celestite Warblade";
            else if (parameter.intValue == CelestiteWarspear) return "Celestite War-spear";
            else if (parameter.intValue == CelestiteGreatblade) return "Celestite Greatblade";
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int SaurusOldblood::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestite Maul") return CelestiteMaul;
        else if (enumString == "Celestite Warblade") return CelestiteWarblade;
        else if (enumString == "Celestite War-spear") return CelestiteWarspear;
        else if (enumString == "Celestite Greatblade") return CelestiteGreatblade;
        return SeraphonBase::EnumStringToInt(enumString);
    }

    int SaurusOldblood::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_warblade.name()) ||
             (weapon->name() == m_warspear.name()) ||
             (weapon->name() == m_greatblade.name()) ||
             (weapon->name() == m_maul.name()))) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusOldblood::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
