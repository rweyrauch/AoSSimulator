/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeerScreamingBell.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include <skaven/SkavenSpells.h>
#include "SkavenPrivate.h"

namespace Skaven {

    class CracksCall : public Spell {
    public:
        explicit CracksCall(Unit *caster) :
                Spell(caster, "Cracks Call", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            if (!target->fly()) {
                target->applyDamage({0, target->move() - Dice::Roll2D6(), Wounds::Source::Spell}, m_caster);
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 13;
    static const int g_pointsPerUnit = 240;

    struct TableEntry {
        int m_move;
        int m_spikesToHit;
        double m_pealRange;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 5, 8, 10, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {6, 2, 26},
                    {6, 3, 22},
                    {4, 4, 18},
                    {4, 4, 14},
                    {3, 5, 10}
            };


    bool GreySeerOnScreamingBell::s_registered = false;

    Unit *GreySeerOnScreamingBell::Create(const ParameterList &parameters) {
        auto unit = new GreySeerOnScreamingBell();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterClanCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_masterClanArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greySeerLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GreySeerOnScreamingBell::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_masterClanCommandTraits[0], g_masterClanCommandTraits),
                            EnumParameter("Artefact", g_masterClanArtefacts[0], g_masterClanArtefacts),
                            EnumParameter("Lore", g_greySeerLore[0], g_greySeerLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Grey Seer on Screaming Bell", factoryMethod);
        }
    }

    GreySeerOnScreamingBell::GreySeerOnScreamingBell() :
            Skaventide("Grey Seer on Screaming Bell", 6, g_wounds, 6, 4, false),
            m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 3, 4, 4, -1, 1),
            m_clawsAndFangs(Weapon::Type::Melee, "Tearing Claws and Fangs", 1, 4, 4, 3, -1, 2),
            m_spikes(Weapon::Type::Melee, "Rusty Spikes", 1, RAND_D6, 2, 3, -1, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, WAR_MACHINE, HERO, WIZARD, SCREAMING_BELL,
                      GREY_SEER};
        m_weapons = {&m_staff, &m_clawsAndFangs, &m_spikes};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_clawsAndFangs.setMount(true);
        s_globalBraveryMod.connect(this, &GreySeerOnScreamingBell::altarOfTheHornedRat, &m_connection);
        s_globalToHitMod.connect(this, &GreySeerOnScreamingBell::wallOfUnholySound, &m_unholySoundConnection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    GreySeerOnScreamingBell::~GreySeerOnScreamingBell() {
        m_connection.disconnect();
        m_unholySoundConnection.disconnect();
    }

    bool GreySeerOnScreamingBell::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_clawsAndFangs);
        model->addMeleeWeapon(&m_spikes);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<CracksCall>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds GreySeerOnScreamingBell::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    void GreySeerOnScreamingBell::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_spikes.setToHit(g_damageTable[damageIndex].m_spikesToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void GreySeerOnScreamingBell::onRestore() {
        Unit::onRestore();

        m_unholySoundActive = false;

        // Restore table-driven attributes
        onWounded();
    }

    int GreySeerOnScreamingBell::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int
    GreySeerOnScreamingBell::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);

        // Pushed into Battle
        if ((weapon->name() == m_spikes.name()) && m_charged) {
            extra += Dice::RollD6();
        }
        return extra;
    }

    int GreySeerOnScreamingBell::altarOfTheHornedRat(const Unit *unit) {
        if (unit->hasKeyword(SKAVENTIDE) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 13.0)) {
            // Make unit battleshock immune
            return 13;
        }
        return 0;
    }

    void GreySeerOnScreamingBell::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_unholySoundActive = false;

        // Peal of Doom
        if (owningPlayer() == player) {
            int roll = Dice::Roll2D6();
            if (roll == 2) {
                // Magical Backlash
                auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0);
                for (auto unit : units) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            } else if (roll <= 4) {
                // Unholy Clamour
                buffModifier(Attribute::Move_Distance, Dice::RollD6(),
                             {Phase::Hero, m_battleRound + 1, owningPlayer()});
            } else if (roll <= 6) {
                // Deafening Peals
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                               g_damageTable[getDamageTableIndex()].m_pealRange);
                for (auto unit : units) {
                    if (Dice::RollD6() >= 4) {
                        unit->applyDamage({0, 1}, this);
                    }
                }
            } else if (roll == 7) {
                // Avalanche of Energy
                auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(),
                                                               g_damageTable[getDamageTableIndex()].m_pealRange);
                for (auto unit : units) {
                    unit->buffModifier(Attribute::Casting_Roll, 1,
                                       {Phase::Hero, m_battleRound + 1, owningPlayer()});
                }
            } else if (roll <= 9) {
                // Apocalyptic Doom
                auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()),
                                                               g_damageTable[getDamageTableIndex()].m_pealRange);
                for (auto unit : units) {
                    if (Dice::RollD6() >= 4) {
                        unit->applyDamage({0, Dice::RollD3()}, this);
                    }
                }
            } else if (roll <= 11) {
                // Wall of Unholy Sound
                m_unholySoundActive = true;
            } else // roll == 12
            {
                // A Stirring Beyond
                // Summon any unit with the VERMINLORD keyword
                static const std::string VerminlordUnitNames[6] = {
                        "Lord Skreech Verminking",
                        "Verminlord Corruptor",
                        "Verminlord Deceiver",
                        "Verminlord Warbringer",
                        "Verminlord Warpseer",
                        "Verminlord Corruptor",
                };

                const int which = Dice::RollD6() - 1;

                auto factory = UnitFactory::LookupUnit(VerminlordUnitNames[which]);
                if (factory) {
                    if (m_roster) {
                        auto unit = std::shared_ptr<Unit>(
                                UnitFactory::Create(VerminlordUnitNames[which], factory->m_parameters));
                        unit->deploy(position(), m_orientation);
                        m_roster->addUnit(unit);
                    }
                }
            }
        }
    }

    int
    GreySeerOnScreamingBell::wallOfUnholySound(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *unit) {
        // Wall of Unholy Sound
        if (m_unholySoundActive && (unit->owningPlayer() != owningPlayer()) &&
            (distanceTo(unit) <= g_damageTable[getDamageTableIndex()].m_pealRange)) {
            return -1;
        }
        return 0;
    }

    int GreySeerOnScreamingBell::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
