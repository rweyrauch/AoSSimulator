/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/MortisEngine.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 180;

    bool MortisEngine::s_registered = false;

    Unit *MortisEngine::Create(const ParameterList &parameters) {
        auto unit = new MortisEngine();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MortisEngine::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void MortisEngine::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DEATH,
                    {DEATHMAGES}
            };
            s_registered = UnitFactory::Register("Mortis Engine", factoryMethod);
        }
    }

    MortisEngine::MortisEngine() :
            LegionOfNagashBase("Mortis Engine", 14, WOUNDS, 10, 4, true),
            m_wail(Weapon::Type::Missile, "Wail of the Damned", 9, 1, 0, 0, 0, 0),
            m_staff(Weapon::Type::Melee, "Corpsemaster's Mortis Staff", 1, 1, 4, 3, -1, RAND_D3),
            m_etherealWeapons(Weapon::Type::Melee, "Spectral Host's Ethereal Weapons", 1, 12, 5, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, DEATHMAGES, MORTIS_ENGINE};
        m_weapons = {&m_wail, &m_staff, &m_etherealWeapons};
    }

    bool MortisEngine::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_wail);
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_etherealWeapons);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void MortisEngine::onWounded() {
        Unit::onWounded();
    }

    void MortisEngine::onRestore() {
        Unit::onRestore();
    }

    int MortisEngine::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
