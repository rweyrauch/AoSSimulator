/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordCorruptor.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SkavenPrivate.h"

namespace Skaven {

    class DreadedPlague : public Spell {
    public:
        explicit DreadedPlague(Unit *caster) :
                Spell(caster, "Dreaded Plague", 7, 13) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            Dice::RollResult rolls;
            Dice::RollD6(target->remainingModels(), rolls);
            target->applyDamage({0, rolls.rollsGE(4), Wounds::Source::Spell}, m_caster);
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 280;

    struct TableEntry {
        int m_move;
        int m_tailAttacks;
        int m_reaperAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 4, 10},
                    {10, 3, 9},
                    {8,  2, 8},
                    {6,  1, 7},
                    {4,  0, 6}
            };


    bool VerminlordCorruptor::s_registered = false;

    Unit *VerminlordCorruptor::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_pestilensCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_pestilensArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new VerminlordCorruptor(trait, artefact, general);
    }

    void VerminlordCorruptor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_pestilensCommandTraits[0], g_pestilensCommandTraits),
                            EnumParameter("Artefact", g_pestilensArtefacts[0], g_pestilensArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Verminlord Corruptor", factoryMethod);
        }
    }

    VerminlordCorruptor::VerminlordCorruptor(CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Verminlord Corruptor", 12, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
            m_plaguereapers(Weapon::Type::Melee, "Plaguereapers", 1, 10, 3, 3, 0, 1) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, NURGLE, CLANS_PESTILENS, MONSTER, HERO, WIZARD, VERMINLORD_CORRUPTOR};
        m_weapons = {&m_tails, &m_plaguereapers};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &VerminlordCorruptor::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_plaguereapers);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<DreadedPlague>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    VerminlordCorruptor::~VerminlordCorruptor() {
        m_connection.disconnect();
    }

    Wounds VerminlordCorruptor::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    int VerminlordCorruptor::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    Wounds
    VerminlordCorruptor::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Plaguereapers
        if ((weapon->name() == m_plaguereapers.name()) && (hitRoll == 6)) {
            return {0, 1};
        }
        return Skaventide::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void VerminlordCorruptor::onEndCombat(PlayerId player) {
        Skaventide::onEndCombat(player);

        // Plaguemaster
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 4) {
                Wounds plague = {0, Dice::RollD3()};
                unit->applyDamage(plague, this);
            }
        }
    }

    void VerminlordCorruptor::onWounded() {
        Skaventide::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_plaguereapers.setAttacks(g_damageTable[damageIndex].m_reaperAttacks);
    }

    void VerminlordCorruptor::onRestore() {
        Skaventide::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t VerminlordCorruptor::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int VerminlordCorruptor::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
