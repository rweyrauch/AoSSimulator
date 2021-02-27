/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fec/GhoulKingOnZombieDragon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {

    class MaleficHunger : public Spell {
    public:
        explicit MaleficHunger(Unit* caster) :
                Spell(caster, "Malefic Hunger", 6, 16) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords = {FLESH_EATER_COURTS};
        }

    protected:
        Result apply(int castingRoll, int unmodifiedCastingRoll, Unit* target) override {
            auto units = Board::Instance()->getUnitsWithin(m_caster, m_caster->owningPlayer(), m_range);
            for (auto unit : units) {
                if (unit->hasKeyword(FLESH_EATER_COURTS)) {
                    unit->buffReroll(To_Wound_Melee, Reroll_Failed, defaultDuration());
                }
            }
            return Spell::Result::Success;
        }

        Result apply(int castingValue, int unmodifiedCastingValue, double x, double y) override { return Result::Failed; }
    };

    static const int g_basesize = 130;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 440;

    struct TableEntry {
        int m_move;
        int m_breathToWound;
        int m_clawsAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 2, 7},
                    {12, 3, 6},
                    {10, 4, 5},
                    {8,  5, 4},
                    {6,  6, 3}
            };

    bool AbhorrantGhoulKingOnZombieDragon::s_registered = false;

    AbhorrantGhoulKingOnZombieDragon::AbhorrantGhoulKingOnZombieDragon() :
            FleshEaterCourts("Abhorrant Ghoul King on Zombie Dragon", 14, g_wounds, 10, 4, true),
            m_pestilentialBreath(Weapon::Type::Missile, "Pestilential Breath", 9, 1, 3, 2, -3, RAND_D6),
            m_goryTalonsAndFangs(Weapon::Type::Melee, "Gory Talons and Fangs", 1, 5, 3, 3, -1, 1),
            m_snappingMaw(Weapon::Type::Melee, "Snapping Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_swordlikeClaws(Weapon::Type::Melee, "Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, FLESH_EATER_COURTS, ABHORRANT, MONSTER, HERO,
                      WIZARD, ABHORRANT_GHOUL_KING};
        m_weapons = {&m_pestilentialBreath, &m_goryTalonsAndFangs, &m_snappingMaw, &m_swordlikeClaws};
        m_hasMount = true;
        m_snappingMaw.setMount(true);
        m_swordlikeClaws.setMount(true);
        m_battleFieldRole = Leader_Behemoth;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool AbhorrantGhoulKingOnZombieDragon::configure(Lore lore, MountTrait trait) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pestilentialBreath);
        model->addMeleeWeapon(&m_goryTalonsAndFangs);
        model->addMeleeWeapon(&m_snappingMaw);
        model->addMeleeWeapon(&m_swordlikeClaws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<MaleficHunger>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *AbhorrantGhoulKingOnZombieDragon::Create(const ParameterList &parameters) {
        auto unit = new AbhorrantGhoulKingOnZombieDragon();

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);

        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == GrandCourt::None)
            unit->setCourtsOfDelusion(delusion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_abhorrantCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_abhorrantArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_dragonMountTraits[0]);

        bool ok = unit->configure(lore, mount);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AbhorrantGhoulKingOnZombieDragon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AbhorrantGhoulKingOnZombieDragon::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    AbhorrantGhoulKingOnZombieDragon::ComputePoints,
                    {
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                            EnumParameter("Command Trait", g_abhorrantCommandTraits[0], g_abhorrantCommandTraits),
                            EnumParameter("Artefact", g_abhorrantArtefacts[0], g_abhorrantArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Mount Trait", g_dragonMountTraits[0], g_dragonMountTraits),
                            BoolParameter("General")
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Abhorrant Ghoul King on Zombie Dragon", factoryMethod);
        }
    }

    Wounds AbhorrantGhoulKingOnZombieDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                          int woundRoll) const {
        return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void AbhorrantGhoulKingOnZombieDragon::onStartHero(PlayerId player) {
        // Royal Blood
        if (player == owningPlayer()) {
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                int woundsHealed = Dice::RollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
    }

    int AbhorrantGhoulKingOnZombieDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void AbhorrantGhoulKingOnZombieDragon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_pestilentialBreath.setToWound(g_damageTable[damageIndex].m_breathToWound);
        m_swordlikeClaws.setAttacks(g_damageTable[damageIndex].m_clawsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void AbhorrantGhoulKingOnZombieDragon::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    int AbhorrantGhoulKingOnZombieDragon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int AbhorrantGhoulKingOnZombieDragon::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Pestilential Breath
        if ((weapon->name() == m_pestilentialBreath.name())) {
            if (Dice::RollD6() <= target->remainingModels()) {
                // Auto hits
                mod += 6;
            }
        }
        return mod;
    }

} // namespace FleshEasterCourt
