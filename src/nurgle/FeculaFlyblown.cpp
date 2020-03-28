/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nurgle/FeculaFlyblown.h"

namespace Nurgle {
    static const int BASESIZE = 40;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 100;

    bool FeculaFlyblown::s_registered = false;

    Unit *Nurgle::FeculaFlyblown::Create(const ParameterList &parameters) {
        auto unit = new FeculaFlyblown();
        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Nurgle::FeculaFlyblown::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FeculaFlyblown::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    FeculaFlyblown::ComputePoints,
                    {
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Fecula Flyblown", factoryMethod);
        }
    }

    Nurgle::FeculaFlyblown::FeculaFlyblown() :
            NurgleBase("Fecula Flyblown", 4, WOUNDS, 8, 4, false),
            m_staff(Weapon::Type::Melee, "Rotwood Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, BLESSED_SONS, HERO, WIZARD, SORCERER, FECULA_FLYBLOWN};
        m_weapons = {&m_staff};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Nurgle::FeculaFlyblown::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int FeculaFlyblown::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }


} // namespace Nurgle
