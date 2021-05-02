/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/KrakenEater.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 35;
    static const int g_pointsPerUnit = 490;

    struct TableEntry {
        int m_move;
        int m_clubAttacks;
        int m_debrisRange;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {12, 18, 24, 30, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {11, 7, 24},
                    {10, 7, 21},
                    {9,  7, 18},
                    {8,  6, 15},
                    {7,  5, 12}
            };

    bool KrakenEater::s_registered = false;

    KrakenEater::KrakenEater(CommandTrait trait, Artefact artefact, FierceLoathing loathing, bool isGeneral) :
            SonsOfBehematBase("Kraken-Eater Mega-Gargant", 11, g_wounds, 7, 4, false, g_pointsPerUnit) {
        m_weapons = {&m_debris, &m_stomp, &m_grip, &m_warclub};
        m_battleFieldRole = Role::Behemoth;
        m_keywords = {DESTRUCTION, SONS_OF_BEHEMAT, GARGANT, MEGA_GARGANT, MONSTER, HERO, KRAKEN_EATER};

        setCommandTrait(trait);
        setArtefact(artefact);
        setFierceLoating(loathing);
        setGeneral(isGeneral);
        setTribe(Tribe::Taker);

        s_globalBraveryMod.connect(this, &KrakenEater::terror, &m_connection);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_debris);
        model->addMeleeWeapon(&m_stomp);
        model->addMeleeWeapon(&m_warclub);
        model->addMeleeWeapon(&m_grip);
        addModel(model);

        if (m_artefact == Artefact::Glowy_Lantern) {
            m_totalSpells = 1;
            m_totalUnbinds = 1;
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
            m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        }
        m_points = g_pointsPerUnit;
    }

    Unit *KrakenEater::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_takersCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_takerArtefact[0]);
        auto loathing = (FierceLoathing) GetEnumParam("Fierce Loathing", parameters, g_loathings[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new KrakenEater(trait, artefact, loathing, general);
    }

    void KrakenEater::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KrakenEater::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    KrakenEater::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_takersCommandTrait[0], g_takersCommandTrait),
                            EnumParameter("Artefact", g_takerArtefact[0], g_takerArtefact),
                            EnumParameter("Fierce Loathing", g_loathings[0], g_loathings),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Kraken-Eater Mega-Gargant", factoryMethod);
        }
    }

    int KrakenEater::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    size_t KrakenEater::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void KrakenEater::onRestore() {
        SonsOfBehematBase::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    void KrakenEater::onWounded() {
        SonsOfBehematBase::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_debris.setRange(g_damageTable[getDamageTableIndex()].m_debrisRange);
        m_warclub.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    Rerolls KrakenEater::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Almighty Stomp
        if ((weapon->name() == m_stomp.name()) && (!target->hasKeyword(MONSTER)))
            return Rerolls::Ones;

        // Death Grip
        if ((weapon->name() == m_grip.name()) && (target - hasKeyword(MONSTER)))
            return Rerolls::Ones;

        return SonsOfBehematBase::toHitRerolls(weapon, target);
    }

    void KrakenEater::onCharged() {
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

    int KrakenEater::terror(const Unit *unit) {
        // Terror
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    int KrakenEater::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = SonsOfBehematBase::extraAttacks(attackingModel, weapon, target);
        if (isGeneral() && (m_commandTrait == CommandTrait::Louder_Than_Words) &&
            (weapon->name() == m_warclub.name()))
            attacks++;
        return attacks;
    }

} // namespace SonsOfBehemat
