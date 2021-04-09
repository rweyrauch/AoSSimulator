/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Warstomper.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 35;
    static const int g_pointsPerUnit = 490;

    struct TableEntry {
        int m_move;
        int m_clubExtraAttacks;
        int m_hurledBodyMod;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {12, 18, 24, 30, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 4, 2},
                    {9,  3, 1},
                    {8,  2, 0},
                    {7,  1, -1},
                    {6,  0, -2}
            };

    bool Warstomper::s_registered = false;

    Warstomper::Warstomper(CommandTrait trait, Artefact artefact, FierceLoathing loathing, bool isGeneral) :
            SonsOfBehematBase("Warstomper Mega-Gargant", 10, g_wounds, 7, 4, false) {
        m_weapons = {&m_grip, &m_jump, &m_club};
        m_battleFieldRole = Role::Behemoth;
        m_keywords = {DESTRUCTION, SONS_OF_BEHEMAT, GARGANT, MEGA_GARGANT, MONSTER, HERO, WARSTOMPER};

        setCommandTrait(trait);
        setArtefact(artefact);
        setFierceLoating(loathing);
        setGeneral(isGeneral);
        setTribe(Tribe::Stomper);

        s_globalBraveryMod.connect(this, &Warstomper::terror, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_grip);
        model->addMeleeWeapon(&m_jump);
        model->addMeleeWeapon(&m_club);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *Warstomper::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_stompersCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_stomperArtefact[0]);
        auto loathing = (FierceLoathing) GetEnumParam("Fierce Loathing", parameters, g_loathings[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Warstomper(trait, artefact, loathing, general);
    }

    void Warstomper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Warstomper::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Warstomper::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_stompersCommandTrait[0], g_stompersCommandTrait),
                            EnumParameter("Artefact", g_stomperArtefact[0], g_stomperArtefact),
                            EnumParameter("Fierce Loathing", g_loathings[0], g_loathings),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Warstomper Mega-Gargant", factoryMethod);
        }
    }

    int Warstomper::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    size_t Warstomper::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Warstomper::onWounded() {
        Unit::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Warstomper::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    int Warstomper::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

        attacks += target->remainingModels() + g_damageTable[getDamageTableIndex()].m_clubExtraAttacks;

        if (isGeneral() && (m_commandTrait == CommandTrait::Louder_Than_Words) &&
            (weapon->name() == m_club.name()))
            attacks++;

        return std::min(10, attacks);
    }

    Rerolls Warstomper::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Almighty Jump
        if ((weapon->name() == m_jump.name()) && (!target->hasKeyword(MONSTER)))
            return Rerolls::Ones_And_Twos;

        // Death Grip
        if ((weapon->name() == m_grip.name()) && (target - hasKeyword(MONSTER)))
            return Rerolls::Ones;

        return Unit::toHitRerolls(weapon, target);
    }

    void Warstomper::onCharged() {
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
        Unit::onCharged();
    }

    int Warstomper::terror(const Unit *unit) {
        // Terror
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    void Warstomper::onBeginRound(int battleRound) {
        SonsOfBehematBase::onBeginRound(battleRound);

        // Very Shouty
        if (isGeneral() && (m_commandTrait == CommandTrait::Very_Shouty) && (battleRound == 1)) {
            getRoster()->addCommandPoints(Dice::RollD3());
        }
    }

    Rerolls Warstomper::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (m_artefact == Artefact::Ironweld_Cestus) {
            return Rerolls::Failed;
        }
        return SonsOfBehematBase::toSaveRerolls(weapon, attacker);
    }

} // namespace SonsOfBehemat
