/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <khorne/BloodWarriors.h>
#include <UnitFactory.h>
#include <iostream>
#include "KhornePrivate.h"

namespace Khorne {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 520;

    bool BloodWarriors::s_registered = false;

    BloodWarriors::BloodWarriors(SlaughterHost host, int numModels, bool pairedGoreax, int numGoreglaives, bool iconBearer, int points) :
            KhorneBase("Blood Warriors", 5, g_wounds, 6, 4, false, points) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, BLOOD_WARRIORS};
        m_weapons = {&m_goreaxe, &m_goreaxeChampion, &m_goreglaive};
        m_battleFieldRole = Role::Battleline;

        setSlaughterHost(host);

        m_pairedGoreaxe = pairedGoreax;

        // Add the Champion
        auto championModel = new Model(g_basesize, wounds());
        championModel->addMeleeWeapon(&m_goreaxeChampion);
        addModel(championModel);

        for (auto i = 0; i < numGoreglaives; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_goreglaive);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_goreaxe);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            }

            addModel(model);
        }
    }

    Rerolls BloodWarriors::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Goreaxes
        if (m_pairedGoreaxe && (weapon->name() == m_goreaxe.name())) {
            return Rerolls::Ones;
        }
        return KhorneBase::toHitRerolls(weapon, unit);
    }

    int BloodWarriors::braveryModifier() const {
        int modifier = KhorneBase::braveryModifier();

        // Icon Bearer
        if (isNamedModelAlive(Model::IconBearer)) {
            modifier++;
        }
        return modifier;
    }

    Unit *BloodWarriors::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool pairedGoreax = GetBoolParam("Paired Goreaxe", parameters, false);
        int numGoreglaives = GetIntParam("Goreglaives", parameters, 0);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);

        return new BloodWarriors(host, numModels, pairedGoreax, numGoreglaives, iconBearer, ComputePoints(parameters));
    }

    void BloodWarriors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BloodWarriors::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    BloodWarriors::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Paired Goreaxe"),
                            IntegerParameter("Goreglaives", 0, 0, g_maxUnitSize / 10, 1),
                            BoolParameter("Icon Bearer"),
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Blood Warriors", factoryMethod);
        }
    }

    Wounds BloodWarriors::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = KhorneBase::computeReturnedDamage(weapon, saveRoll);
        // Gorefists
        if (!m_pairedGoreaxe && (saveRoll == 6)) {
            wounds += {0, 1};
        }
        return wounds;
    }

    int BloodWarriors::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void BloodWarriors::onFriendlyModelSlain(int numSlain, Unit *attacker, Wounds::Source source) {
        KhorneBase::onFriendlyModelSlain(numSlain, attacker, source);

        // No Respite
        int numAdditionalSlain = 0;
        fight(1, m_meleeTarget, numAdditionalSlain);
    }

} // namespace Khorne