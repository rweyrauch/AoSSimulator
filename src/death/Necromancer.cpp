/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Necromancer.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 130;

    bool Necromancer::s_registered = false;

    Necromancer::Necromancer() :
            LegionOfNagashBase("Necromancer", 5, WOUNDS, 10, 6, false),
            m_staff(Weapon::Type::Melee, "Necromancer's Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {DEATH, NECROMANCER, DEATHMAGES, HERO, WIZARD};
        m_weapons = {&m_staff};
    }

    bool Necromancer::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Necromancer::Create(const ParameterList &parameters) {
        auto unit = new Necromancer();

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Necromancer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Legion", Legion::GrandHostOfNagash, Legion ::GrandHostOfNagash, Legion::LegionOfBlood, 1},
                    },
                    DEATH,
                    {DEATHMAGES}
            };
            s_registered = UnitFactory::Register("Necromancer", factoryMethod);
        }
    }

    Wounds Necromancer::applyWoundSave(const Wounds &wounds) {
        return Unit::applyWoundSave(wounds);
    }

    int Necromancer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void Necromancer::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations();
    }

} // namespace Death
