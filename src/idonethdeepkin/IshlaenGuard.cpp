/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/IshlaenGuard.h>
#include <UnitFactory.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = 560;

    bool AkhelianIshlaenGuard::s_registered = false;

    AkhelianIshlaenGuard::AkhelianIshlaenGuard() :
            IdonethDeepkinBase("Akhelian Ishlaen Guard", 14, g_wounds, 6, 4, true),
            m_helsabre(Weapon::Type::Melee, "Helsabre", 1, 3, 3, 3, 0, 1),
            m_helsabrePrince(Weapon::Type::Melee, "Helsabre", 1, 4, 3, 3, 0, 1),
            m_fangmoraFangedMaw(Weapon::Type::Melee, "Fangmora's Fanged Maw", 1, 1, 3, 3, 0, RAND_D3),
            m_fangmoraLashingTail(Weapon::Type::Melee, "Fangmora's Lashing Tail", 2, RAND_D3, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, AKHELIAN_GUARD, ISHLAEN_GUARD};
        m_weapons = {&m_helsabre, &m_helsabrePrince, &m_fangmoraFangedMaw, &m_fangmoraLashingTail};
        m_hasMount = true;
    }

    bool AkhelianIshlaenGuard::configure(int numModels, bool standardBearers, bool musicians) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto prince = new Model(g_basesize, wounds());
        prince->addMeleeWeapon(&m_helsabrePrince);
        prince->addMeleeWeapon(&m_fangmoraFangedMaw);
        prince->addMeleeWeapon(&m_fangmoraLashingTail);
        addModel(prince);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_helsabre);
            model->addMeleeWeapon(&m_fangmoraFangedMaw);
            model->addMeleeWeapon(&m_fangmoraLashingTail);
            if (standardBearers) {
                model->setName("Standard Bearer");
                standardBearers = false;
            }
            else if (musicians) {
                model->setName("Musicians");
                musicians = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *AkhelianIshlaenGuard::Create(const ParameterList &parameters) {
        auto unit = new AkhelianIshlaenGuard();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, true);
        bool musicians = GetBoolParam("Musicians", parameters, true);

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        bool ok = unit->configure(numModels, standardBearers, musicians);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AkhelianIshlaenGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearers"),
                            BoolParameter("Musicians"),
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Akhelian Ishlaen Guard", factoryMethod);
        }
    }

    Rerolls AkhelianIshlaenGuard::battleshockRerolls() const {
        if (isNamedModelAlive("Standard Bearer")) { return Reroll_Failed; }
        return Unit::battleshockRerolls();
    }

    Rerolls AkhelianIshlaenGuard::chargeRerolls() const {
        if (isNamedModelAlive("Musician")) { return Reroll_Failed; }
        return Unit::chargeRerolls();
    }

    int AkhelianIshlaenGuard::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);

        // Biovoltaic Barrier
        if (m_charged) {
            modifier += 1;
        }

        // Biovoltaic Barrier - ignore rend by cancelling it out.
        if (weapon->rend() < 0) {
            modifier = -weapon->rend();
        }

        return modifier;
    }

    int AkhelianIshlaenGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace IdonethDeepkin