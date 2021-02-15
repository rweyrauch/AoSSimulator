/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/Fiends.h>
#include <Weapon.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 75; // x42 oval
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 9;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize/g_minUnitSize) * g_pointsPerBlock;

    bool Fiends::s_registered = false;

    Fiends::Fiends() :
            SlaaneshBase("Fiends", 12, g_wounds, 10, 5, false),
            m_deadlyPincers(Weapon::Type::Melee, "Deadly Pincers", 1, 4, 3, 3, -1, 1),
            m_deadlyPincersBlissbringer(Weapon::Type::Melee, "Deadly Pincers", 1, 5, 3, 3, -1, 1),
            m_barbedStinger(Weapon::Type::Melee, "Barbed Stinger", 2, 1, 3, 3, -1, 1) {
        m_keywords = {CHAOS, DAEMON, SLAANESH, HEDONITE, FIENDS};
        m_weapons = {&m_deadlyPincers, &m_deadlyPincersBlissbringer, &m_barbedStinger};

        s_globalCastMod.connect(this, &Fiends::disruptiveSong, &m_connection);
    }

    Fiends::~Fiends() {
        m_connection.disconnect();
    }

    bool Fiends::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto blissbringer = new Model(g_basesize, wounds());
        blissbringer->addMeleeWeapon(&m_deadlyPincersBlissbringer);
        blissbringer->addMeleeWeapon(&m_barbedStinger);
        addModel(blissbringer);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_deadlyPincers);
            model->addMeleeWeapon(&m_barbedStinger);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Fiends::Create(const ParameterList &parameters) {
        auto unit = new Fiends();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Fiends::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Fiends", factoryMethod);
        }
    }

    int Fiends::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::targetHitModifier(weapon, attacker);

        // Soporific Musk
        if (!weapon->isMissile()) {
            modifier -= 1;
        }
        return modifier;
    }

    int Fiends::targetWoundModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::targetWoundModifier(weapon, attacker);

        // Soporific Musk
        if (!weapon->isMissile() && remainingModels() >= 4) {
            modifier -= 1;
        }
        return modifier;
    }

    Wounds Fiends::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Crushing Grip
        if ((woundRoll == 6) && (weapon->name() == m_deadlyPincers.name())) {
            return {RAND_D3, 0};
        }
        // Deadly Venom
        if (weapon->name() == m_barbedStinger.name()) {
            if (target->wounds() == 1)
                return {1, 0};
            else if (target->wounds() <= 3)
                return {RAND_D3, 0};
            else
                return {RAND_D6, 0};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Fiends::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int Fiends::disruptiveSong(const Unit *caster) {
        // Disruptive Song
        if (caster->hasKeyword(WIZARD) && (caster->owningPlayer() != owningPlayer()) && (distanceTo(caster) <= 12.0)) {
            return -1;
        }

        return 0;
    }

} // namespace Slaanesh