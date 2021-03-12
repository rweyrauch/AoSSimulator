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

    Warstomper::Warstomper() :
            SonsOfBehematBase("Warstomper Mega-Gargant", 10, g_wounds, 7, 4, false),
            m_grip(Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6),
            m_jump(Weapon::Type::Melee, "Jump Up and Down", 3, 4, 3, 3, -2, RAND_D3),
            m_club(Weapon::Type::Melee, "Titanic Boulderclub", 3, 0, 3, 3, -2, 2) {
        m_weapons = {&m_grip, &m_jump, &m_club};
        m_battleFieldRole = Role::Behemoth;
        m_keywords = {DESTRUCTION, SONS_OF_BEHEMAT, GARGANT, MEGA_GARGANT, MONSTER, HERO, WARSTOMPER};

        s_globalBraveryMod.connect(this, &Warstomper::terror, &m_connection);
    }

    bool Warstomper::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_grip);
        model->addMeleeWeapon(&m_jump);
        model->addMeleeWeapon(&m_club);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *Warstomper::Create(const ParameterList &parameters) {
        auto unit = new Warstomper();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_stompersCommandTrait[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_stomperArtefact[0]);
        unit->setArtefact(artefact);
        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->setTribe(Tribe::Stomper);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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

    int Warstomper::getDamageTableIndex() const {
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
