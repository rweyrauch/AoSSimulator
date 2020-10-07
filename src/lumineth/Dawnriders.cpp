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
    static const int BASESIZE = 65; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 130;
    static const int POINTS_MAX_UNIT_SIZE = 520;

    bool Dawnriders::s_registered = false;

    Dawnriders::Dawnriders() :
            LuminethBase("Vanari Dawnriders", 14, WOUNDS, 7, 4, false),
            m_guardiansSword(Weapon::Type::Melee, "Guardian's Sword", 1, 2, 3, 4, -1, 1),
            m_lance(Weapon::Type::Melee, "Sunmetal Lance", 2, 1, 3, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Dashing Hooves", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, DAWNRIDERS};
        m_weapons = {&m_guardiansSword, &m_lance, &m_hooves};
        m_battleFieldRole = Role::Battleline;
        m_hasMount = true;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Dawnriders::configure(int numModels, bool standardBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto master = new Model(BASESIZE, wounds());
        master->addMeleeWeapon(&m_guardiansSword);
        master->addMeleeWeapon(&m_hooves);
        master->setName("Steedmaster");
        addModel(master);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_lance);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<PowerOfHysh>(this));

        m_standardBearer = standardBearer;
        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Dawnriders::Create(const ParameterList &parameters) {
        auto unit = new Dawnriders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto standard = GetBoolParam("Standard Bearer", parameters, true);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
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
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Standard Bearer"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Dawnriders", factoryMethod);
        }
    }

    int Dawnriders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
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

    Wounds Dawnriders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sunmetal weapons
        auto hitRollThreshold = 6;
        if (m_powerOfHyshActive) hitRollThreshold = 5;

        if ((hitRoll >= hitRollThreshold) && (weapon->name() == m_lance.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Dawnriders::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_powerOfHyshActive = false;
    }

    int Dawnriders::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Lances of Dawn
        if (charged() && (weapon->name() == m_lance.name())) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
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
        if (m_standardBearer) return RerollFailed;
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