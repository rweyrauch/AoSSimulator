/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordWarpseer.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SkavenPrivate.h"

namespace Skaven {

    class DreadedWarpgale : public Spell {
    public:
        explicit DreadedWarpgale(Unit *caster) :
                Spell(caster, "Dreaded Warpgale", 8, 26) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            target->applyDamage({0, Dice::RollD6(), Wounds::Source::Spell}, m_caster);
            target->buffMovement(MovementRule::Halve_Charge_Roll, true, defaultDuration());
            target->buffMovement(MovementRule::Halve_Run_Roll, true, defaultDuration());
            if (target->fly()) {
                target->buffMovement(MovementRule::Can_Fly, false, defaultDuration());
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_move;
        int m_tailAttacks;
        int m_glaiveToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 4, 2},
                    {10, 3, 2},
                    {8,  2, 3},
                    {6,  1, 3},
                    {4,  0, 4}
            };


    bool VerminlordWarpseer::s_registered = false;

    Unit *VerminlordWarpseer::Create(const ParameterList &parameters) {
        auto unit = new VerminlordWarpseer();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterClanCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_masterClanArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void VerminlordWarpseer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_masterClanCommandTraits[0], g_masterClanCommandTraits),
                            EnumParameter("Artefact", g_masterClanArtefacts[0], g_masterClanArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Verminlord Warpseer", factoryMethod);
        }
    }

    VerminlordWarpseer::VerminlordWarpseer() :
            Skaventide("Verminlord Warpseer", 12, g_wounds, 10, 4, false),
            m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
            m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 2, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, MASTERCLAN, MONSTER, HERO, WIZARD,
                      VERMINLORD_WARPSEER};
        m_weapons = {&m_tails, &m_glaive};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &VerminlordWarpseer::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    VerminlordWarpseer::~VerminlordWarpseer() {
        m_connection.disconnect();
    }

    bool VerminlordWarpseer::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_glaive);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<DreadedWarpgale>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds VerminlordWarpseer::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    int VerminlordWarpseer::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    void VerminlordWarpseer::onStartHero(PlayerId player) {
        Skaventide::onStartHero(player);

        if ((player == owningPlayer()) && (remainingModels() >= 0) && m_roster) {
            auto roll = Dice::RollD6();
            if (roll == 6) m_roster->addCommandPoints(Dice::RollD3());
            else if (roll >= 3) m_roster->addCommandPoints(1);
        }
    }

    Rerolls VerminlordWarpseer::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        if (!m_usedOrb) {
            // Scry-orb
            return Rerolls::Failed;
        }

        return Skaventide::toSaveRerolls(weapon, attacker);
    }

    void VerminlordWarpseer::onStartShooting(PlayerId player) {
        Skaventide::onStartShooting(player);

        if (!m_usedOrb) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) <= 13.0) {
                unit->applyDamage({0, Dice::RollD6()}, this);
                m_usedOrb = true;
            }
        }
    }

    void VerminlordWarpseer::onRestore() {
        Skaventide::onRestore();

        m_usedOrb = false;

        // Restore table-driven attributes
        onWounded();
    }

    void VerminlordWarpseer::onWounded() {
        Skaventide::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_glaive.setToWound(g_damageTable[damageIndex].m_glaiveToWound);
    }

    size_t VerminlordWarpseer::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int VerminlordWarpseer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
