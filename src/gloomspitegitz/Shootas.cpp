/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Shootas.h>
#include <UnitFactory.h>
#include <iostream>

namespace GloomspiteGitz {
    static const int g_basesize = 25;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 20;
    static const int g_maxUnitSize = 60;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool Shootas::s_registered = false;

    Shootas::Shootas() :
            GloomspiteGitzBase("Shootas", 5, g_wounds, 4, 6, false),
            m_slitta(Weapon::Type::Melee, "Slitta", 1, 1, 5, 5, 0, 1),
            m_slittaBoss(Weapon::Type::Melee, "Slitta", 1, 1, 4, 5, 0, 1),
            m_moonclanBow(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 5, 5, 0, 1),
            m_moonclanBowBoss(Weapon::Type::Missile, "Moonclan Bow", 16, 1, 4, 5, 0, 1),
            m_barbedNet(Weapon::Type::Melee, "Barbed Net", 2, 3, 4, 5, 0, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, SHOOTAS};
        m_weapons = {&m_slitta, &m_slittaBoss, &m_moonclanBow, &m_moonclanBowBoss, &m_barbedNet};
        m_battleFieldRole = Battleline;
    }


    Unit *Shootas::Create(const ParameterList &parameters) {
        auto unit = new Shootas();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numBarbedNets = GetIntParam("Barbed Nets", parameters, 0);
        int numGongbashers = GetIntParam("Gong Bashers", parameters, 0);
        int numFlagbearers = GetIntParam("Flag Bearers", parameters, 0);
        int numIconbearers = GetIntParam("Icon Bearers", parameters, 0);

        bool ok = unit->configure(numModels, numBarbedNets, numGongbashers, numFlagbearers, numIconbearers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Shootas::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Shootas::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    Shootas::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Barbed Nets", 0, 0, 3 * g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Gong Bashers", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Flag Bearers", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Icon Bearers", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Shootas", factoryMethod);
        }
    }

    bool
    Shootas::configure(int numModels, int numBarbedNets, int numGongbashers, int numFlagbearers, int numIconbearers) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }

        if (numBarbedNets > 3 * numModels / g_minUnitSize) {
            return false;
        }
        if (numGongbashers > numModels / g_minUnitSize) {
            return false;
        }
        if (numFlagbearers + numIconbearers > numModels / g_minUnitSize) {
            return false;
        }

        m_numFlagbearers = numFlagbearers;
        m_numGongbashers = numGongbashers;
        m_numIconbearers = numIconbearers;

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        boss->addMeleeWeapon(&m_slittaBoss);
        boss->addMissileWeapon(&m_moonclanBowBoss);
        addModel(boss);

        // add the nets
        for (auto i = 1; i < numBarbedNets; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_barbedNet);
            addModel(model);
        }

        // and the rest
        auto currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_slitta);
            model->addMissileWeapon(&m_moonclanBow);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        m_ranks = 4;

        return true;
    }

    int Shootas::toWoundModifier(const Weapon *weapon, const Unit *unit) const {
        // Backstabbing Mob
        int modifier = GloomspiteGitzBase::toWoundModifier(weapon, unit);
        if (!weapon->isMissile()) {
            if (remainingModels() >= 30) { modifier += 2; }
            else if (remainingModels() >= 15) { modifier += 1; }
        }
        return modifier;
    }

    int Shootas::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        int modifier = GloomspiteGitzBase::toHitModifier(weapon, unit);
        // Moonclan Bows
        if (remainingModels() >= 15 && weapon->isMissile()) { modifier += 1; }
        return modifier;
    }

    int Shootas::runModifier() const {
        int modifier = GloomspiteGitzBase::runModifier();
        if (m_numGongbashers > 0) { modifier += 2; }
        return modifier;
    }

    int Shootas::braveryModifier() const {
        int modifier = GloomspiteGitzBase::braveryModifier();
        if (m_numFlagbearers > 0) { modifier += 1; }
        return modifier;
    }

    int Shootas::toSaveModifier(const Weapon *weapon) const {
        int modifier = GloomspiteGitzBase::toSaveModifier(weapon);
        if (m_numIconbearers > 0 && weapon->isMissile()) {
            modifier += 1;
        }
        return modifier;
    }

    int Shootas::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Netters
        if (distanceTo(attacker) <= 2.0) {
            bool hasNets = false;
            for (const auto &ip : m_models) {
                for (auto wip = ip->meleeWeaponBegin(); wip != ip->meleeWeaponEnd(); ++wip) {
                    const Weapon *w = *wip;

                    if (w->name() == m_barbedNet.name()) {
                        hasNets = true;
                        break;
                    }
                }
            }
            if (hasNets) { modifier -= 1; }
        }
        return modifier;
    }

    int Shootas::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace GloomspiteGitz