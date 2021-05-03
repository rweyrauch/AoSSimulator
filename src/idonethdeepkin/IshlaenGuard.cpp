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

    AkhelianIshlaenGuard::AkhelianIshlaenGuard(Enclave enclave, int numModels, bool standardBearers, bool musicians, int points) :
            IdonethDeepkinBase(enclave, "Akhelian Ishlaen Guard", 14, g_wounds, 6, 4, true, points),
            m_helsabre(Weapon::Type::Melee, "Helsabre", 1, 3, 3, 3, 0, 1),
            m_helsabrePrince(Weapon::Type::Melee, "Helsabre", 1, 4, 3, 3, 0, 1),
            m_fangmoraFangedMaw(Weapon::Type::Melee, "Fangmora's Fanged Maw", 1, 1, 3, 3, 0, RAND_D3),
            m_fangmoraLashingTail(Weapon::Type::Melee, "Fangmora's Lashing Tail", 2, RAND_D3, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, AKHELIAN_GUARD, ISHLAEN_GUARD};
        m_weapons = {&m_helsabre, &m_helsabrePrince, &m_fangmoraFangedMaw, &m_fangmoraLashingTail};
        m_hasMount = true;
        m_fangmoraFangedMaw.setMount(true);
        m_fangmoraLashingTail.setMount(true);

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
                model->setName(Model::StandardBearer);
                standardBearers = false;
            } else if (musicians) {
                model->setName("Musicians");
                musicians = false;
            }
            addModel(model);
        }
    }

    Unit *AkhelianIshlaenGuard::Create(const ParameterList &parameters) {
        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standardBearers = GetBoolParam("Standard Bearers", parameters, true);
        bool musicians = GetBoolParam("Musicians", parameters, true);
        return new AkhelianIshlaenGuard(enclave, numModels, standardBearers, musicians, ComputePoints(parameters));
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
        if (isNamedModelAlive(Model::StandardBearer)) { return Rerolls::Failed; }
        return Unit::battleshockRerolls();
    }

    Rerolls AkhelianIshlaenGuard::chargeRerolls() const {
        if (isNamedModelAlive(Model::Musician)) { return Rerolls::Failed; }
        return Unit::chargeRerolls();
    }

    int AkhelianIshlaenGuard::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::toSaveModifier(weapon, attacker);

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

    int AkhelianIshlaenGuard::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace IdonethDeepkin