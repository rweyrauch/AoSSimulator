/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "mawtribes/Hrothgorn.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool Hrothgorn::s_registered = false;

    Unit *Hrothgorn::Create(const ParameterList &parameters) {
        auto unit = new Hrothgorn();

        unit->setMawtribe(Mawtribe::Winterbite);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Hrothgorn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Hrothgorn::Create,
                    Hrothgorn::ValueToString,
                    Hrothgorn::EnumStringToInt,
                    Hrothgorn::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Hrothgorn", factoryMethod);
        }
    }

    Hrothgorn::Hrothgorn() :
            MawtribesBase("Hrothgorn", 6, g_wounds, 7, 5, false),
            m_trapLauncher(Weapon::Type::Missile, "Trap Launcher", 12, 1, 4, 3, 0, RAND_D3),
            m_knife(Weapon::Type::Melee, "Hunting Knife", 1, 4, 3, 3, 0, 2),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, WINTERBITE, HERO, ICEBROW_HUNTER,
                      HROTHGORN};
        m_weapons = {&m_trapLauncher, &m_knife, &m_bite};
        m_battleFieldRole = Leader;
    }

    bool Hrothgorn::configure() {
        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_trapLauncher);
        model->addMeleeWeapon(&m_knife);
        model->addMeleeWeapon(&m_bite);

        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    std::string Hrothgorn::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int Hrothgorn::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    int Hrothgorn::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
