/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/SquigGobba.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 160;

    bool SquigGobba::s_registered = false;

    SquigGobba::SquigGobba() :
            GloomspiteGitzBase("Squig Gobba", 4, g_wounds, 4, 5, false),
            m_spitSquigs(Weapon::Type::Missile, "Spit-squigs", 30, 6, 4, 3, 0, RAND_D3),
            m_bashinSticks(Weapon::Type::Melee, "Bashin' Sticks", 1, 3, 5, 5, 0, 1),
            m_cavernousMaw(Weapon::Type::Melee, "Cavernous Maw", 2, 3, 3, 3, -2, RAND_D3) {
        m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, SQUIG, MOONCLAN, MONSTER, SQUIG_GOBBA};
        m_weapons = {&m_spitSquigs, &m_bashinSticks, &m_cavernousMaw};
        m_battleFieldRole = Role::Artillery;
    }

    bool SquigGobba::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_spitSquigs);
        model->addMeleeWeapon(&m_bashinSticks);
        model->addMeleeWeapon(&m_cavernousMaw);

        m_points = g_pointsPerUnit;

        addModel(model);

        return true;
    }

    Unit *SquigGobba::Create(const ParameterList &parameters) {
        auto unit = new SquigGobba();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SquigGobba::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SquigGobba::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SquigGobba::ComputePoints,
                    {
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Squig Gobba", factoryMethod);
        }
    }

    int SquigGobba::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = GloomspiteGitzBase::toHitModifier(weapon, target);

        // Arcing Spit
        if (weapon->name() == m_spitSquigs.name() && target->remainingModels() >= 10)
            modifier += 1;

        return modifier;
    }

    int SquigGobba::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
