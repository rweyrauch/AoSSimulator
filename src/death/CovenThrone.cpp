/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/CovenThrone.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 260;

    bool CovenThrone::s_registered = false;

    Unit *CovenThrone::Create(const ParameterList &parameters) {
        auto unit = new CovenThrone();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int CovenThrone::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void CovenThrone::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Coven Throne", factoryMethod);
        }
    }

    CovenThrone::CovenThrone() :
            LegionOfNagashBase("Coven Throne", 14, WOUNDS, 10, 4, true),
            m_bite(Weapon::Type::Melee, "Vampire Queen's Predatory Bite", 1, 1, 3, 4, 0, RAND_D3),
            m_stiletto(Weapon::Type::Melee, "Vampire Queen's Stiletto", 1, 4, 3, 3, -1, 1),
            m_poniards(Weapon::Type::Melee, "Handmaidens' Needle-sharp Poniards", 1, 8, 3, 3, 0, 1),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, MALIGNANT, HERO, WIZARD, COVEN_THRONE};
        m_weapons = {&m_bite, &m_stiletto, &m_poniards, &m_etherealWeapons};
    }

    bool CovenThrone::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bite);
        model->addMeleeWeapon(&m_stiletto);
        model->addMeleeWeapon(&m_poniards);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void CovenThrone::onWounded() {
        Unit::onWounded();
    }

    void CovenThrone::onRestore() {
        Unit::onRestore();
    }

    int CovenThrone::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
