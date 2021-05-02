/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Dawnriders.h>
#include <UnitFactory.h>
#include <iostream>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int g_basesize = 65; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 520;

    bool Dawnriders::s_registered = false;

    Dawnriders::Dawnriders(int points) :
            LuminethBase("Vanari Dawnriders", 14, g_wounds, 7, 4, false, points),
            m_guardiansSword(Weapon::Type::Melee, "Guardian's Sword", 1, 2, 3, 4, -1, 1),
            m_lance(Weapon::Type::Melee, "Sunmetal Lance", 2, 1, 3, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Dashing Hooves", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, DAWNRIDERS, Sunmetal_Weapons};
        m_weapons = {&m_guardiansSword, &m_lance, &m_hooves};
        m_battleFieldRole = Role::Battleline;
        m_hasMount = true;
        m_hooves.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Dawnriders::configure(int numModels, bool standardBearer) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto master = new Model(g_basesize, wounds());
        master->addMeleeWeapon(&m_guardiansSword);
        master->addMeleeWeapon(&m_hooves);
        master->setName("Steedmaster");
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_hooves);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            }
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<PowerOfHysh>(this));

        return true;
    }

    Unit *Dawnriders::Create(const ParameterList &parameters) {
        auto unit = new Dawnriders(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto standard = GetBoolParam("Standard Bearer", parameters, true);

        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure(numModels, standard);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Dawnriders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Dawnriders", factoryMethod);
        }
    }

    int Dawnriders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Dawnriders::onRestore() {
        LuminethBase::onRestore();

        m_powerOfHyshActive = false;
    }

    void Dawnriders::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        if (spell->name() == "Power of Hysh") {
            m_powerOfHyshActive = true;
        }
    }

    Wounds Dawnriders::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sunmetal weapons
        auto hitRollThreshold = 6;
        if (m_powerOfHyshActive) hitRollThreshold = 5;

        if ((hitRoll >= hitRollThreshold) && (weapon->name() == m_lance.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void Dawnriders::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_powerOfHyshActive = false;
    }

    int Dawnriders::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lances of Dawn
        if (charged() && (weapon->name() == m_lance.name())) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int Dawnriders::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        // Lances of Dawn
        if (charged() && (weapon->name() == m_lance.name())) {
            mod++;
        }
        return mod;
    }

    Rerolls Dawnriders::battleshockRerolls() const {
        if (isNamedModelAlive(Model::StandardBearer)) return Rerolls::Failed;
        return Unit::battleshockRerolls();
    }

    int Dawnriders::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

        // Deathly Furrows
        if (!target->hasMount()) {
            if (target->wounds() == 2) attacks += 1;
            else if (target->wounds() == 1) attacks += 2;
        }

        return attacks;
    }

} // namespace LuminethRealmLords