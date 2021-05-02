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
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool AuralanWardens::s_registered = false;

    AuralanWardens::AuralanWardens(int points) :
            LuminethBase("Vanari Auralan Wardens", 6, g_wounds, 6, 4, false, points),
            m_championsBlade(Weapon::Type::Melee, "Champion's Blade", 1, 2, 3, 4, -1, 1),
            m_wardensPike(Weapon::Type::Melee, "Warden's Pike", 3, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, VANARI, AURALAN_WARDENS, Sunmetal_Weapons};
        m_weapons = {&m_championsBlade, &m_wardensPike};
        m_battleFieldRole = Role::Battleline;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool AuralanWardens::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto highWarden = new Model(g_basesize, wounds());
        highWarden->addMeleeWeapon(&m_championsBlade);
        highWarden->setName("High Warden");
        addModel(highWarden);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_wardensPike);
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<PowerOfHysh>(this));

        return true;
    }

    Unit *AuralanWardens::Create(const ParameterList &parameters) {
        auto unit = new AuralanWardens(ComputePoints(parameters));
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
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
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Vanari Auralan Wardens", factoryMethod);
        }
    }

    int AuralanWardens::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
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
                if (Dice::RollD6() >= 2) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
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

    Wounds AuralanWardens::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Sunmetal weapons
        auto hitRollThreshold = 6;
        if (m_powerOfHyshActive) hitRollThreshold = 5;

        if ((hitRoll >= hitRollThreshold) && (weapon->name() == m_wardensPike.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void AuralanWardens::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_powerOfHyshActive = false;
    }

    int AuralanWardens::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Wall of Blades
        if (target && target->charged() && (weapon->name() == m_wardensPike.name())) {
            return weapon->rend() - 1;
        }
        return Unit::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
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