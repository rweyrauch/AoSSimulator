/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/SoulGrinder.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 160;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 210;

    bool SoulGrinder::s_registered = false;

    Unit *SoulGrinder::Create(const ParameterList &parameters) {
        auto unit = new SoulGrinder();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, WarpmetalBlade);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, SlavesToDarknessBase::Ravagers);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, Undivided);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(weapon);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SoulGrinder::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            switch (parameter.intValue) {
                case WarpmetalBlade:
                    return "Warpmetal Blade";
                case DaemonboneTalon:
                    return "Daemonbone Talon";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int SoulGrinder::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Warpmetal Blade") return WarpmetalBlade;
        else if (enumString == "Daemonbone Talon") return DaemonboneTalon;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int SoulGrinder::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void SoulGrinder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SoulGrinder::Create,
                    SoulGrinder::ValueToString,
                    SoulGrinder::EnumStringToInt,
                    SoulGrinder::ComputePoints,
                    {
                            {ParamType::Enum, "Weapon",
                             WarpmetalBlade, WarpmetalBlade, DaemonboneTalon, 1},
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Soul Grinder", factoryMethod);
        }
    }

    SoulGrinder::SoulGrinder() :
            SlavesToDarknessBase("Soul Grinder", 12, WOUNDS, 10, 4, false),
            m_cannon(Weapon::Type::Missile, "Harvester Cannon", 16, 6, 4, 3, -1, 1),
            m_phlegm(Weapon::Type::Missile, "Phlegm Bombardment", 20, 1, 4, 3, -2, 3),
            m_legs(Weapon::Type::Melee, "Piston-driven Legs", 1, 6, 4, 3, -1, 1),
            m_claw(Weapon::Type::Melee, "Hellforged Claw", 2, 1, 4, 3, -2, RAND_D6),
            m_blade(Weapon::Type::Melee, "Warpmetal Blade", 2, 2, 4, 3, -2, 3),
            m_talon(Weapon::Type::Melee, "Daemonbone Talon", 2, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, MONSTER, MARK_OF_CHAOS, SOUL_GRINDER};
        m_weapons = {&m_cannon, &m_phlegm, &m_legs, &m_claw, &m_blade, &m_talon};
    }

    bool SoulGrinder::configure(WeaponOption option) {
        auto model = new Model(BASESIZE, wounds());

        model->addMissileWeapon(&m_cannon);
        model->addMissileWeapon(&m_phlegm);
        model->addMeleeWeapon(&m_legs);
        model->addMeleeWeapon(&m_claw);
        if (option == WarpmetalBlade)
            model->addMeleeWeapon(&m_blade);
        else if (option == DaemonboneTalon)
            model->addMeleeWeapon(&m_talon);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace SlavesToDarkness


