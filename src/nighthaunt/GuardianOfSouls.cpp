/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nighthaunt/GuardianOfSouls.h"

namespace Nighthaunt {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool GuardianOfSouls::s_registered = false;

    Unit *GuardianOfSouls::Create(const ParameterList &parameters) {
        auto unit = new GuardianOfSouls();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GuardianOfSouls::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GuardianOfSouls::Create,
                    nullptr,
                    nullptr,
                    GuardianOfSouls::ComputePoints,
                    {
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Guardian of Souls with Nightmare Lantern", factoryMethod);
        }
    }

    GuardianOfSouls::GuardianOfSouls() :
            Nighthaunt("Guardian of Souls with Nightmare Lantern", 6, WOUNDS, 10, 4, true),
            m_blade(Weapon::Type::Melee, "Chill Blade", 1, 3, 3, 3, -1, 1),
            m_maul(Weapon::Type::Melee, "Maul of Judgement", 1, 2, 3, 3, 0, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, GUARDIAN_OF_SOULS};
        m_weapons = {&m_blade, &m_maul};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool GuardianOfSouls::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_maul);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    int GuardianOfSouls::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
