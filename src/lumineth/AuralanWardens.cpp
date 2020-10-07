/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/AuralanWardens.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    bool AuralanWardens::s_registered = false;

    AuralanWardens::AuralanWardens() :
            LuminethBase("Vanari Auralan Wardens", 6, WOUNDS, 6, 4, false),
            m_championsBlade(Weapon::Type::Melee, "Champion's Blade", 1, 2, 3, 4, -1, 1),
            m_wardensPike(Weapon::Type::Melee, "Warden's Pike", 3, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, AURALAN_WARDENS};
        m_weapons = {&m_championsBlade, &m_wardensPike};
        m_battleFieldRole = Role::Battleline;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool AuralanWardens::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto highWarden = new Model(BASESIZE, wounds());
        highWarden->addMeleeWeapon(&m_championsBlade);
        highWarden->setName("High Warden");
        addModel(highWarden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_wardensPike);
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<PowerOfHysh>(this));

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *AuralanWardens::Create(const ParameterList &parameters) {
        auto unit = new AuralanWardens();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuralanWardens::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Auralan Wardens", factoryMethod);
        }
    }

    int AuralanWardens::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    void AuralanWardens::onRestore() {
        LuminethBase::onRestore();

        m_usedMoonfireFlask = false;
        m_powerOfHyshActive = false;
    }

    void AuralanWardens::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Moonfire Flask
        if (!m_usedMoonfireFlask) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) <= 3.0f)) {
                m_usedMoonfireFlask = true;
                if (Dice::rollD6() >= 2) {
                    unit->applyDamage({0, Dice::rollD3()});
                }
            }
        }
    }

    void AuralanWardens::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        if (spell->name() == "Power of Hysh") {
            m_powerOfHyshActive = true;
        }
    }

    Wounds AuralanWardens::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sunmetal weapons
        auto hitRollThreshold = 6;
        if (m_powerOfHyshActive) hitRollThreshold = 5;

        if ((hitRoll >= hitRollThreshold) && (weapon->name() == m_wardensPike.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void AuralanWardens::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_powerOfHyshActive = false;
    }

    int AuralanWardens::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Wall of Blades
        if (target && target->charged() && (weapon->name() == m_wardensPike.name())) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    int AuralanWardens::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);
        // Wall of Blades
        if (target && target->charged() && (weapon->name() == m_wardensPike.name())) {
            mod++;
        }
        return mod;
    }

} // namespace LuminethRealmLords