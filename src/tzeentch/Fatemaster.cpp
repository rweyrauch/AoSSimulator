/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Fatemaster.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 120;

    bool Fatemaster::s_registered = false;

    Unit *Fatemaster::Create(const ParameterList &parameters) {
        auto unit = new Fatemaster();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, TzeentchBase::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Fatemaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Fatemaster::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    Fatemaster::ComputePoints,
                    {
                            {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None,
                             TzeentchBase::GuildOfSummoners, 1},
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Fatemaster", factoryMethod);
        }
    }

    Fatemaster::Fatemaster() :
            TzeentchBase("Fatemaster", 16, WOUNDS, 8, 4, true),
            m_glaive(Weapon::Type::Melee, "Fireglaive of Tzeentch", 2, 3, 3, 4, 0, RAND_D3),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, FATEMASTER};
        m_weapons = {&m_glaive, &m_teethAndHorns};
    }

    bool Fatemaster::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Fatemaster::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // Tzeentch