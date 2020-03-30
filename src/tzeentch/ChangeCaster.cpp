/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Changecaster.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 0;

    bool ChangecasterHeraldOfTzeentch::s_registered = false;

    Unit *ChangecasterHeraldOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new ChangecasterHeraldOfTzeentch();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, TzeentchBase::None);
        unit->setChangeCoven(coven);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ChangecasterHeraldOfTzeentch::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void ChangecasterHeraldOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChangecasterHeraldOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    ChangecasterHeraldOfTzeentch::ComputePoints,
                    {
                            {ParamType::Enum, "Change Coven", TzeentchBase::None, TzeentchBase::None,
                             TzeentchBase::GuildOfSummoners, 1},
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Changecaster Herald of Tzeentch", factoryMethod);
        }
    }

    ChangecasterHeraldOfTzeentch::ChangecasterHeraldOfTzeentch() :
            TzeentchBase("Changecaster Herald of Tzeentch", 5, WOUNDS, 10, 5, false),
            m_flames(Weapon::Type::Missile, "Magical Flames", 18, 3, 4, 4, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Change", 2, 1, 4, 3, -1, RAND_D3),
            m_dagger(Weapon::Type::Melee, "Ritual Dagger", 1, 2, 4, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, CHANGECASTER};
        m_weapons = {&m_flames, &m_staff, &m_dagger};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ChangecasterHeraldOfTzeentch::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_flames);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_dagger);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace Tzeentch


