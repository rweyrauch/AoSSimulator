/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/GhoulKingOnTerrorgheist.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "FleshEaterCourtsPrivate.h"
#include "SummonAbility.h"
#include "FeCSpells.h"

namespace FleshEaterCourt {

    class UnholyVitality : public Spell {
    public:
        explicit UnholyVitality(Unit *caster) :
                Spell(caster, "Unholy Vitality", 6, 24) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {FLESH_EATER_COURTS};
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            target->buffAbility(Ability::Ignore_All_Wounds_On_Value, 5, defaultDuration());
            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 130;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 420;

    struct TableEntry {
        int m_move;
        int m_deathShriek;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {4, 7, 10, 13, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 6, 4},
                    {12, 5, 4},
                    {10, 4, 3},
                    {8,  3, 3},
                    {6,  2, 2}
            };

    bool AbhorrantGhoulKingOnTerrorgheist::s_registered = false;

    AbhorrantGhoulKingOnTerrorgheist::AbhorrantGhoulKingOnTerrorgheist(GrandCourt court, Delusion delusion, Lore lore, MountTrait mountTrait, CommandTrait trait, Artefact artefact, bool isGeneral) :
            FleshEaterCourts(court, delusion, "Abhorrant Ghoul King on Terrorgheist", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_deathShriek(Weapon::Type::Missile, "Death Shriek", 10, 0, 0, 0, 0, 0),
            m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 5, 3, 3, -1, 1),
            m_skeletalClaws(Weapon::Type::Melee, "Skeletal Claws", 2, 4, 4, 3, -1, RAND_D3),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 3, 3, 4, 3, -2, RAND_D6) {
        m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, MONSTER, HERO,
                      WIZARD, ABHORRANT_GHOUL_KING};
        m_weapons = {&m_deathShriek, &m_goryTalonsAndFangs, &m_skeletalClaws, &m_fangedMaw};
        m_hasMount = true;
        m_skeletalClaws.setMount(true);
        m_fangedMaw.setMount(true);
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_deathShriek);
        model->addMeleeWeapon(&m_goryTalonsAndFangs);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_fangedMaw);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<UnholyVitality>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        SummonedUnitDesc unitDesc;
        unitDesc.push_back({"Crypt Flayers", 3});
        unitDesc.push_back({"Crypt Horrors", 3});
        m_commandAbilities.push_back(
                std::make_unique<SummonAbility>(this, getRoster(), "Summon Royal Guard", unitDesc));

        m_mountTrait = mountTrait;
        if (m_mountTrait == MountTrait::Deathly_Fast) {
            m_runAndShoot = true;
        }
    }

    Unit *AbhorrantGhoulKingOnTerrorgheist::Create(const ParameterList &parameters) {
        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_terrorgheistMountTraits[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_abhorrantCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_abhorrantArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AbhorrantGhoulKingOnTerrorgheist(court, delusion, lore, mount, trait, artefact, general);
    }

    void AbhorrantGhoulKingOnTerrorgheist::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AbhorrantGhoulKingOnTerrorgheist::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    AbhorrantGhoulKingOnTerrorgheist::ComputePoints,
                    {
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                            EnumParameter("Command Trait", g_abhorrantCommandTraits[0], g_abhorrantCommandTraits),
                            EnumParameter("Artefact", g_abhorrantArtefacts[0], g_abhorrantArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Mount Trait", g_terrorgheistMountTraits[0], g_terrorgheistMountTraits),
                            BoolParameter("General")
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Abhorrant Ghoul King on Terrorgheist", factoryMethod);
        }
    }

    Wounds AbhorrantGhoulKingOnTerrorgheist::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                          int woundRoll) const {
        // Gaping Maw
        if (weapon->name() == m_fangedMaw.name() && hitRoll == 6) {
            return {0, 6};
        }
        return FleshEaterCourts::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    void AbhorrantGhoulKingOnTerrorgheist::onStartHero(PlayerId player) {
        FleshEaterCourts::onStartHero(player);
        // Royal Blood
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                int woundsHealed = Dice::RollD3();
                if (m_mountTrait == MountTrait::Horribly_Resilient) {
                    woundsHealed = 3;
                }
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
    }

    size_t AbhorrantGhoulKingOnTerrorgheist::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void AbhorrantGhoulKingOnTerrorgheist::onWounded() {
        FleshEaterCourts::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_skeletalClaws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void AbhorrantGhoulKingOnTerrorgheist::onRestore() {
        FleshEaterCourts::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void AbhorrantGhoulKingOnTerrorgheist::onFriendlyUnitSlain(const Unit *attacker) {
        FleshEaterCourts::onFriendlyUnitSlain(nullptr);

        // Infested
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
        for (auto ip : units) {
            Wounds wounds = {0, Dice::RollD3()};
            if (m_mountTrait == MountTrait::Horribly_Infested) {
                wounds.mortal = 3;
            }
            ip->applyDamage(wounds, this);
        }
    }

    int AbhorrantGhoulKingOnTerrorgheist::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void AbhorrantGhoulKingOnTerrorgheist::onStartShooting(PlayerId player) {
        FleshEaterCourts::onStartShooting(player);

        // Death Shriek
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= m_deathShriek.range())) {
            auto roll = Dice::RollD6() + g_damageTable[getDamageTableIndex()].m_deathShriek;
            if (m_mountTrait == MountTrait::Devastating_Scream) {
                roll++;
            }
            if (roll > unit->bravery()) {
                unit->applyDamage({0, roll - unit->bravery()}, this);
            }
        }
    }

    Rerolls AbhorrantGhoulKingOnTerrorgheist::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if ((m_mountTrait == MountTrait::Gruesome_Bite) && (weapon->name() == m_fangedMaw.name())) {
            return Rerolls::Failed;
        }
        return FleshEaterCourts::toHitRerolls(weapon, target);
    }

    int AbhorrantGhoulKingOnTerrorgheist::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        auto rend = FleshEaterCourts::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);
        if (weapon->isMelee() && (m_mountTrait == MountTrait::Razor_Clawed)) {
            rend--;
        }
        return rend;
    }

} // namespace FleshEasterCourt
