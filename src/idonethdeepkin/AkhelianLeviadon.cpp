/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <idonethdeepkin/AkhelianLeviadon.h>
#include <UnitFactory.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 310;

    bool AkhelianLeviadon::s_registered = false;

    AkhelianLeviadon::AkhelianLeviadon() :
            IdonethDeepkinBase("Akhelian Leviadon", 12, WOUNDS, 7, 3, true),
            m_harpoonLauncher(Weapon::Type::Missile, "Harpoon Launchers", 24, 6, 3, 3, 0, 1),
            m_crushingJaws(Weapon::Type::Melee, "Leviadon's Crushing Jaws", 1, 1, 2, 2, -2, RAND_D6),
            m_scythedFins(Weapon::Type::Melee, "Leviadon's Massive Scythed Fins", 2, 4, 3, 3, -1, 3),
            m_twinProngedSpear(Weapon::Type::Melee, "Twin-pronged Spear", 1, 2, 3, 3, 0, 2),
            m_razorshellHarpoons(Weapon::Type::Melee, "Razorshell Harpoons", 1, 4, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, MONSTER, AKHELIAN, LEVIADON};
        m_weapons = {&m_harpoonLauncher, &m_crushingJaws, &m_scythedFins, &m_twinProngedSpear, &m_razorshellHarpoons};
        m_battleFieldRole = Behemoth;
        m_hasMount = true;
    }

    bool AkhelianLeviadon::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_harpoonLauncher);
        model->addMeleeWeapon(&m_crushingJaws);
        model->addMeleeWeapon(&m_scythedFins);
        model->addMeleeWeapon(&m_twinProngedSpear);
        model->addMeleeWeapon(&m_razorshellHarpoons);

        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *AkhelianLeviadon::Create(const ParameterList &parameters) {
        auto unit = new AkhelianLeviadon();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AkhelianLeviadon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Akhelian Leviadon", factoryMethod);
        }
    }

    Wounds AkhelianLeviadon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Jaws of Death
        if ((hitRoll >= 6) && (weapon->name() == m_crushingJaws.name())) {
            return {0, 6};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int AkhelianLeviadon::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace IdonethDeepkin
