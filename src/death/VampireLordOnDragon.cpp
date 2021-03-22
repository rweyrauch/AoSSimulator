/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/VampireLordOnDragon.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 130;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 440;

    struct TableEntry {
        int m_move;
        int m_breathToWound;
        int m_clawAttacks;
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

    bool VampireLordOnZombieDragon::s_registered = false;

    Unit *VampireLordOnZombieDragon::Create(const ParameterList &parameters) {
        auto unit = new VampireLordOnZombieDragon();

        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Deathlance);
        bool shield = GetBoolParam("Ancient Shield", parameters, true);
        bool chalice = GetBoolParam("Chalice of Blood", parameters, true);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_vampireLore[0]);

        bool ok = unit->configure(weapon, shield, chalice, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int VampireLordOnZombieDragon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void VampireLordOnZombieDragon::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Deathlance, Vampiric_Sword};
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Deathlance, weapons),
                            BoolParameter("Ancient Shield"),
                            BoolParameter("Chalice of Blood"),
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_vampireLore[0], g_vampireLore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Vampire Lord on Zombie Dragon", factoryMethod);
        }
    }

    VampireLordOnZombieDragon::VampireLordOnZombieDragon() :
            LegionOfNagashBase("Vampire Lord on Zombie Dragon", 14, g_wounds, 10, 4, true),
            m_breath(Weapon::Type::Missile, "Pestilential Breath", 9, 1, 3, 2, -3, RAND_D6),
            m_deathlance(Weapon::Type::Melee, "Deathlance", 1, 3, 3, 3, -1, 2),
            m_sword(Weapon::Type::Melee, "Vampiric Sword", 1, 4, 3, 3, -1, RAND_D3),
            m_maw(Weapon::Type::Melee, "Zombie Dragon's Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Zombie Dragon's Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, SOULBLIGHT, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, VAMPIRE_LORD, VAMPIRE};
        m_weapons = {&m_breath, &m_deathlance, &m_maw, &m_claws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_maw.setMount(true);
        m_claws.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool VampireLordOnZombieDragon::configure(WeaponOption option, bool shield, bool chalice, Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_breath);

        if (option == Deathlance) {
            model->addMeleeWeapon(&m_deathlance);
        } else if (option == Vampiric_Sword) {
            model->addMeleeWeapon(&m_sword);
        }
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        // Ancient Shield
        if (shield) {
            m_save = 3;
        }

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_haveChaliceOfBlood = chalice;

        m_points = g_pointsPerUnit;

        return true;
    }

    void VampireLordOnZombieDragon::onWounded() {
        LegionOfNagashBase::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_breath.setToWound(g_damageTable[damageIndex].m_breathToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void VampireLordOnZombieDragon::onRestore() {
        LegionOfNagashBase::onRestore();

        m_usedChaliceOfBlood = false;

        // Restore table-driven attributes
        onWounded();
    }

    int VampireLordOnZombieDragon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void VampireLordOnZombieDragon::onStartHero(PlayerId player) {
        LegionOfNagashBase::onStartHero(player);

        if (owningPlayer() == player) {

            deathlyInvocations(3, 12.0);

            // Chalice of Blood
            if (m_haveChaliceOfBlood && !m_usedChaliceOfBlood && remainingWounds() < wounds()) {
                heal(Dice::RollD6());
                m_usedChaliceOfBlood = true;
            }
        }
    }

    void VampireLordOnZombieDragon::onEndCombat(PlayerId player) {
        // The Hunger
        if (m_currentRecord.m_enemyModelsSlain > 0) heal(1);

        LegionOfNagashBase::onEndCombat(player);
    }

    Wounds VampireLordOnZombieDragon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                   int woundRoll) const {
        // Deathlance Charge
        if (m_charged && (weapon->name() == m_deathlance.name())) return {3, 0};
        return LegionOfNagashBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int VampireLordOnZombieDragon::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = LegionOfNagashBase::toHitModifier(weapon, target);

        // Pestilential Breath
        if ((weapon->name() == m_breath.name())) {
            if (Dice::RollD6() <= target->remainingModels()) {
                // Auto hits
                mod += 6;
            }
        }
        return mod;
    }
} // namespace Death
