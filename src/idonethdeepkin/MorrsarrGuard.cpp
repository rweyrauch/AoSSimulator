/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <idonethdeepkin/MorrsarrGuard.h>
#include <UnitFactory.h>
#include "IdonethDeepkinPrivate.h"

namespace IdonethDeepkin {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 170;
    static const int g_pointsMaxUnitSize = 170 * 4;

    bool AkhelianMorrsarrGuard::s_registered = false;

    AkhelianMorrsarrGuard::AkhelianMorrsarrGuard() :
            IdonethDeepkinBase("Akhelian Morrsarr Guard", 14, g_wounds, 6, 4, true),
            m_voltspear(Weapon::Type::Melee, "Voltspear", 2, 2, 3, 3, 0, 1),
            m_voltspearPrince(Weapon::Type::Melee, "Voltspear", 2, 3, 3, 3, 0, 1),
            m_fangmoraFangedMaw(Weapon::Type::Melee, "Fangmora's Fanged Maw", 1, 1, 3, 3, 0, RAND_D3),
            m_fangmoraLashingTail(Weapon::Type::Melee, "Fangmora's Lashing Tail", 2, RAND_D3, 3, 3, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, AKHELIAN, AKHELIAN_GUARD, MORRSARR_GUARD};
        m_weapons = {&m_voltspear, &m_voltspearPrince, &m_fangmoraFangedMaw, &m_fangmoraLashingTail};
        m_hasMount = true;
        m_fangmoraLashingTail.setMount(true);
        m_fangmoraFangedMaw.setMount(true);
    }

    bool AkhelianMorrsarrGuard::configure(int numModels, bool standardBearers, bool musicians) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto prince = new Model(g_basesize, wounds());
        prince->addMeleeWeapon(&m_voltspearPrince);
        prince->addMeleeWeapon(&m_fangmoraFangedMaw);
        prince->addMeleeWeapon(&m_fangmoraLashingTail);
        addModel(prince);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_voltspear);
            model->addMeleeWeapon(&m_fangmoraFangedMaw);
            model->addMeleeWeapon(&m_fangmoraLashingTail);
            if (standardBearers) {
                model->setName(Model::StandardBearer);
                standardBearers = false;
            } else if (musicians) {
                model->setName(Model::Musician);
                musicians = false;
            }
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *AkhelianMorrsarrGuard::Create(const ParameterList &parameters) {
        auto unit = new AkhelianMorrsarrGuard();
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

    void AkhelianMorrsarrGuard::Init() {
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

            s_registered = UnitFactory::Register("Akhelian Morrsarr Guard", factoryMethod);
        }
    }

    Rerolls AkhelianMorrsarrGuard::battleshockRerolls() const {
        if (isNamedModelAlive(Model::StandardBearer)) { return Rerolls::Failed; }
        return Unit::battleshockRerolls();
    }

    Rerolls AkhelianMorrsarrGuard::chargeRerolls() const {
        if (isNamedModelAlive(Model::Musician)) { return Rerolls::Failed; }
        return Unit::chargeRerolls();
    }

    void AkhelianMorrsarrGuard::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        if (player != owningPlayer()) { return; }

        // Biovoltaic Blast
        if (m_meleeTarget && !m_usedBiovoltaicBlast) {
            Dice::RollResult roll;
            Dice::RollD6(m_meleeTarget->remainingModels(), roll);
            int totalMortalWounds = 0;
            int num6s = roll.numUnmodified6s();
            for (int i = 0; i < roll.numUnmodified6s(); i++) {
                totalMortalWounds += Dice::RollD3();
            }
            int num3plus = roll.rollsGE(3);
            totalMortalWounds += (num3plus - num6s);

            m_meleeTarget->applyDamage({0, totalMortalWounds}, this);

            m_usedBiovoltaicBlast = true;
        }

    }

    int AkhelianMorrsarrGuard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds
    AkhelianMorrsarrGuard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (m_charged && (weapon->name() == m_voltspear.name())) return {weapon->damage() + 1, 0};
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int AkhelianMorrsarrGuard::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (m_charged && (weapon->name() == m_voltspear.name())) return -2;
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

} // namespace IdonethDeepkin