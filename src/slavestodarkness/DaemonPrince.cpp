/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DaemonPrince.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 50;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 0;

    bool DaemonPrince::s_registered = false;

    Unit *DaemonPrince::Create(const ParameterList &parameters) {
        auto unit = new DaemonPrince();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, DaemonicAxe);

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

    std::string DaemonPrince::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            switch (parameter.intValue) {
                case DaemonicAxe:
                    return "Daemonic Axe";
                case HellforgedSword:
                    return "Hellforged Sword";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int DaemonPrince::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Daemonic Axe") return DaemonicAxe;
        else if (enumString == "Hellforged Sword") return HellforgedSword;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int DaemonPrince::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void DaemonPrince::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DaemonPrince::Create,
                    DaemonPrince::ValueToString,
                    DaemonPrince::EnumStringToInt,
                    DaemonPrince::ComputePoints,
                    {
                            {ParamType::Enum, "Weapon",
                             DaemonicAxe, DaemonicAxe, HellforgedSword, 1},
                            {ParamType::Enum, "Damned Legion", SlavesToDarknessBase::Ravagers,
                             SlavesToDarknessBase::Ravagers, SlavesToDarknessBase::HostOfTheEverchosen, 1},
                            {ParamType::Enum, "Mark of Chaos", SlavesToDarknessBase::Undivided,
                             SlavesToDarknessBase::Undivided, SlavesToDarknessBase::Tzeentch},
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Daemon Prince", factoryMethod);
        }
    }

    DaemonPrince::DaemonPrince() :
            SlavesToDarknessBase("Daemon Prince", 12, WOUNDS, 10, 3, true),
            m_axe(Weapon::Type::Melee, "Daemonic Axe", 1, 3, 3, 3, -2, 2),
            m_sword(Weapon::Type::Melee, "Hellforged Sword", 2, 4, 4, 3, -1, RAND_D3),
            m_talons(Weapon::Type::Melee, "Malefic Talons", 1, 3, 3, 3, 0, 2) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, HERO, MARK_OF_CHAOS, DAEMON_PRINCE};
        m_weapons = {&m_axe, &m_sword, &m_talons};
    }

    bool DaemonPrince::configure(WeaponOption option) {
        auto model = new Model(BASESIZE, wounds());

        if (option == DaemonicAxe)
            model->addMeleeWeapon(&m_axe);
        else if (option == HellforgedSword)
            model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace SlavesToDarkness


