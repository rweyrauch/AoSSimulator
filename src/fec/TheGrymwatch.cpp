/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/TheGrymwatch.h>
#include <UnitFactory.h>

namespace FleshEaterCourt {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_pointsPerUnit = 0;

    bool TheGrymwatch::s_registered = false;

    TheGrymwatch::TheGrymwatch() :
            FleshEaterCourts("The Grymwatch", 6, g_wounds, 10, 6, false, g_pointsPerUnit),
            m_weaponsAndClaws(Weapon::Type::Melee, "Bone Weapons and Filthy Claws", 1, 2, 4, 4, 0, 1),
            m_fangs(Weapon::Type::Melee, "Fangs", 1, RAND_D6, 4, 4, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HOLLOWMOURNE, GRYMWATCH};
        m_weapons = {&m_weaponsAndClaws, &m_fangs};
    }

    void TheGrymwatch::configure() {

        for (auto i = 0; i < 2; i++) {
            auto harrier = new Model(g_basesize, wounds());
            harrier->addMeleeWeapon(&m_fangs);
            addModel(harrier);
        }

        for (auto i = 1; i < 4; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_weaponsAndClaws);
            addModel(model);
        }
    }

    Unit *TheGrymwatch::Create(const ParameterList &parameters) {
        auto unit = new TheGrymwatch();

        unit->setGrandCourt(GrandCourt::Hollowmourne);

        unit->configure();
        return unit;
    }

    void TheGrymwatch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheGrymwatch::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    TheGrymwatch::ComputePoints,
                    {
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("The Grymwatch", factoryMethod);
        }
    }

    int TheGrymwatch::ComputePoints(const ParameterList& parameters) {
        return g_pointsPerUnit;
    }

    Wounds TheGrymwatch::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Quest to Slay the Monster
        if (target->hasKeyword(MONSTER)) {
            return {weapon->damage() + 1, 0, Wounds::Source::Weapon_Melee};
        }
        return FleshEaterCourts::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} // namespace FleshEaterCourt
