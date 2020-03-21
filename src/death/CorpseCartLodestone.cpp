/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/CorpseCartLodestone.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 80;

    bool CorpseCartWithUnholyLodestone::s_registered = false;

    Unit *CorpseCartWithUnholyLodestone::Create(const ParameterList &parameters) {
        auto unit = new CorpseCartWithUnholyLodestone();

        auto legion = (Legion)GetEnumParam("Legion", parameters, GrandHostOfNagash);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int CorpseCartWithUnholyLodestone::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void CorpseCartWithUnholyLodestone::Init() {
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
                    {DEADWALKERS}
            };
            s_registered = UnitFactory::Register("Corpse Cart with Unholy Lodestone", factoryMethod);
        }
    }

    CorpseCartWithUnholyLodestone::CorpseCartWithUnholyLodestone() :
            LegionOfNagashBase("Corpse Cart with Unholy Lodestone", 4, WOUNDS, 10, 6, false),
            m_goad(Weapon::Type::Melee, "Corpsemaster's Goad", 2, 2, 4, 4, 0, 1),
            m_lash(Weapon::Type::Melee, "Corpsemaster's Lash", 1, 3, 4, 4, 0, 1),
            m_blades(Weapon::Type::Melee, "Zombies' Rusty Blades", 1, RAND_2D6, 5, 5, 0, 1) {
        m_keywords = {DEATH, ZOMBIE, DEADWALKERS, CORPSE_CART};
        m_weapons = {&m_goad, &m_lash, &m_blades};
    }

    bool CorpseCartWithUnholyLodestone::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_goad);
        model->addMeleeWeapon(&m_lash);
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void CorpseCartWithUnholyLodestone::onWounded() {
        Unit::onWounded();
    }

    void CorpseCartWithUnholyLodestone::onRestore() {
        Unit::onRestore();
    }

    int CorpseCartWithUnholyLodestone::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
