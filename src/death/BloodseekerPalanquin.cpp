/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/BloodseekerPalangquin.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 300;

    bool BloodseekerPalanquin::s_registered = false;

    Unit *BloodseekerPalanquin::Create(const ParameterList &parameters) {
        auto unit = new BloodseekerPalanquin();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int BloodseekerPalanquin::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void BloodseekerPalanquin::Init() {
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
            s_registered = UnitFactory::Register("Bloodseeker Palanquin", factoryMethod);
        }
    }

    BloodseekerPalanquin::BloodseekerPalanquin() :
            LegionOfNagashBase("Bloodseeker Palanquin", 14, WOUNDS, 10, 4, true),
            m_wail(Weapon::Type::Missile, "Wail of the Damned", 9, 1, 0, 0, 0, 0),
            m_blade(Weapon::Type::Melee, "Sanguinarch's Bloodletting Blade", 1, 4, 3, 3, -1, RAND_D3),
            m_etherialWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, MALIGNANT, HERO, WIZARD, BLOODSEEKER_PALANQUIN};
        m_weapons = {&m_wail, &m_blade, &m_etherialWeapons};
    }

    bool BloodseekerPalanquin::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_etherialWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void BloodseekerPalanquin::onWounded() {
        Unit::onWounded();
    }

    void BloodseekerPalanquin::onRestore() {
        Unit::onRestore();
    }

    int BloodseekerPalanquin::getDamageTableIndex() const {
        return 0;
    }

} // namespace Death
