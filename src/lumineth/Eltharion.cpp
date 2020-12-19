/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Eltharion.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {
    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 220;

    bool TheLightOfEltharion::s_registered = false;

    TheLightOfEltharion::TheLightOfEltharion() :
            LuminethBase("The Light of Eltharion", 6, g_wounds, 10, 3, false),
            m_fangsword(Weapon::Type::Melee, "Fangsword of Eltharion", 1, 4, 2, 3, -3, RAND_D3),
            m_blade(Weapon::Type::Melee, "Celennari Blade", 1, 2, 2, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, HERO, LIGHT_OF_ELTHARION};
        m_weapons = {&m_fangsword, &m_blade};
        m_battleFieldRole = Role::Leader;
    }

    bool TheLightOfEltharion::configure() {

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_fangsword);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *TheLightOfEltharion::Create(const ParameterList &parameters) {
        auto unit = new TheLightOfEltharion();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto nation = (GreatNation)GetEnumParam("Nation", parameters, (int)GreatNation::None);
        unit->setNation(nation);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheLightOfEltharion::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("The Light of Eltharion", factoryMethod);
        }
    }

    int TheLightOfEltharion::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int TheLightOfEltharion::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);

        // Fangsword
        if (charged() && (weapon->name() == m_fangsword.name())) mod++;

        return mod;
    }

    Wounds TheLightOfEltharion::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);

        // Fangsword
        if (charged() && (weapon->name() == m_fangsword.name())) {
            if (woundRoll == 6) {
                damage.mortal++;
            }
        }

        // Celennari Blade
        if (m_celennariBladeTarget && (weapon->name() == m_blade.name())) {
            damage.normal++;
        }
        return damage;
    }

    void TheLightOfEltharion::onStartShooting(PlayerId player) {
        if (player == owningPlayer()) {
            // Searing Darts of Light
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && (distanceTo(unit) < 18.0f)) {
                auto roll = Dice::RollD6();
                if (roll >= 5) {
                    unit->applyDamage({0, Dice::RollD6()}, this);
                }
                else if (roll >= 2) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
    }

    int TheLightOfEltharion::targetSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        // Spirit Armour
        return 0;
    }

    Wounds TheLightOfEltharion::targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll, int woundRoll) const {
        // Spirit Armour
        return {(wounds.normal+1)/2, wounds.mortal};
    }

    int TheLightOfEltharion::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Supreme Swordmaster
        if (unmodifiedHitRoll == 6) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void TheLightOfEltharion::onStartCombat(PlayerId player) {
        if (meleeTarget() && meleeTarget()->hasKeyword(HERO) && distanceTo(meleeTarget()) <= 3.0f) {
            m_celennariBladeTarget = meleeTarget();
        }
        else {
            m_celennariBladeTarget = nullptr;
        }
    }

} // namespace LuminethRealmLords