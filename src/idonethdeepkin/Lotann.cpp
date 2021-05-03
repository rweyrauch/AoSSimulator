/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "idonethdeepkin/Lotann.h"
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 70;

    bool Lotann::s_registered = false;

    Unit *Lotann::Create(const ParameterList &parameters) {
        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Lotann(enclave, general);
    }

    void Lotann::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Lotann", factoryMethod);
        }
    }

    Lotann::Lotann(Enclave enclave, bool isGeneral) :
            IdonethDeepkinBase(enclave, "Lotann", 6, g_wounds, 7, 6, false, g_pointsPerUnit),
            m_quill(Weapon::Type::Melee, "Bone Quill", 1, 1, 3, 5, 0, 1),
            m_cudgel(Weapon::Type::Melee, "Ochtar's Cudgel", 3, 1, 4, 3, -1, 2),
            m_blade(Weapon::Type::Melee, "Ochtar's Blade", 3, 1, 3, 4, -1, 1),
            m_tentacles(Weapon::Type::Melee, "Ochtar's Tentacles", 3, 6, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, ISHARANN, SOUL_WARDEN, HERO, LOTANN};
        m_weapons = {&m_quill, &m_cudgel, &m_blade, &m_tentacles};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &Lotann::catalogueOfSouls, &m_connection);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_quill);
        model->addMeleeWeapon(&m_cudgel);
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_tentacles);

        addModel(model);
    }

    Lotann::~Lotann() {
        m_connection.disconnect();
    }

    Wounds Lotann::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        return ignoreWounds(wounds, 5);
    }

    int Lotann::catalogueOfSouls(const Unit *target) {
        // Catalogue of Souls
        if (target->hasKeyword(IDONETH_DEEPKIN) && (target->owningPlayer() == owningPlayer()) &&
            (distanceTo(target) <= 12.0)) {
            return 1;
        }

        return 0;
    }

    int Lotann::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //IdonethDeepkin