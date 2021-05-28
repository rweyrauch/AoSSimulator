/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Gatebreaker.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 35;
    static const int g_pointsPerUnit = 490;

    struct TableEntry {
        int m_move;
        int m_flailAttacks;
        int m_boulderRange;
        int m_smashDown;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {12, 18, 24, 30, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 10, 18, 2},
                    {11, 9,  15, 3},
                    {10, 7,  12, 4},
                    {9,  6,  9,  5},
                    {8,  5,  6,  6}
            };

    bool Gatebreaker::s_registered = false;

    Gatebreaker::Gatebreaker(CommandTrait trait, Artefact artefact, FierceLoathing loathing, bool isGeneral) :
            SonsOfBehematBase("Gatebreaker Mega-Gargant", 12, g_wounds, 7, 4, false, g_pointsPerUnit) {
        m_weapons = {&m_boulder, &m_stomp, &m_grip, &m_flail};
        m_battleFieldRole = Role::Behemoth;
        m_keywords = {DESTRUCTION, SONS_OF_BEHEMAT, GARGANT, MEGA_GARGANT, MONSTER, HERO, GATEBREAKER};

        setCommandTrait(trait);
        setArtefact(artefact);
        setFierceLoating(loathing);
        setGeneral(isGeneral);
        setTribe(Tribe::Breaker);

        s_globalBraveryMod.connect(this, &Gatebreaker::terror, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_boulder);
        model->addMeleeWeapon(&m_stomp);
        model->addMeleeWeapon(&m_flail);
        model->addMeleeWeapon(&m_grip);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Gatebreaker::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_breakerCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_breakerArtefact[0]);
        auto loathing = (FierceLoathing) GetEnumParam("Fierce Loathing", parameters, g_loathings[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Gatebreaker(trait, artefact, loathing, general);
    }

    void Gatebreaker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gatebreaker::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Gatebreaker::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_breakerCommandTrait[0], g_breakerCommandTrait),
                            EnumParameter("Artefact", g_breakerArtefact[0], g_breakerArtefact),
                            EnumParameter("Fierce Loathing", g_loathings[0], g_loathings),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Gatebreaker Mega-Gargant", factoryMethod);
        }
    }

    int Gatebreaker::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    size_t Gatebreaker::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Gatebreaker::onWounded() {
        SonsOfBehematBase::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_boulder.setRange(g_damageTable[getDamageTableIndex()].m_boulderRange);
        m_flail.setAttacks(g_damageTable[getDamageTableIndex()].m_flailAttacks);
    }

    void Gatebreaker::onRestore() {
        SonsOfBehematBase::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    Rerolls Gatebreaker::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Almighty Stomp
        if ((weapon->name() == m_stomp.name()) && (!target->hasKeyword(MONSTER)))
            return Rerolls::Ones;

        // Death Grip
        if ((weapon->name() == m_grip.name()) && (target - hasKeyword(MONSTER)))
            return Rerolls::Ones;

        return SonsOfBehematBase::toHitRerolls(weapon, target);
    }

    void Gatebreaker::onCharged() {
        // Crushing Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0f);
        for (auto unit : units) {
            if (Dice::RollD6() >= 2) {
                auto mortal = Dice::RollD6();
                if (unit->hasKeyword(MONSTER)) {
                    mortal = Dice::RollD3();
                }
                unit->applyDamage({0, mortal}, this);
            }
        }
        SonsOfBehematBase::onCharged();
    }

    int Gatebreaker::terror(const Unit *unit) {
        // Terror
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    int Gatebreaker::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = SonsOfBehematBase::extraAttacks(attackingModel, weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Louder_Than_Words) &&
            (weapon->name() == m_flail.name()))
            attacks += 2;
        return attacks;
    }

    Wounds Gatebreaker::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SonsOfBehematBase::applyWoundSave(wounds, attackingUnit);
        if (m_artefact == Artefact::Enchanted_Portcullis) {
            int numSixes = 0;
            totalWounds = ignoreWounds(totalWounds, 6, numSixes);
        }
        return totalWounds;
    }

    Rerolls Gatebreaker::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_artefact == Artefact::Kingslaughter_Cowl) {
            if (target->isGeneral()) return Rerolls::Failed;
            else if (target->hasKeyword(HERO)) return Rerolls::Ones;
        }
        return SonsOfBehematBase::toWoundRerolls(weapon, target);
    }

    Wounds Gatebreaker::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = SonsOfBehematBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        if (m_artefact == Artefact::The_Great_Wrecka) {
            if ((hitRoll == 6) && (weapon->name() == m_flail.name())) {
                damage.mortal++;
            }
        }
        return damage;
    }

} // namespace SonsOfBehemat
