/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/KeeperOfSecrets.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int BASESIZE = 100;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 380;

    struct TableEntry {
        int m_move;
        int m_greatbladeAttacks;
        int m_clawDamage;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {14, 4, 5},
                    {12, 3, 4},
                    {10, 3, 3},
                    {8,  2, 3},
                    {6,  2, 2}
            };

    bool KeeperOfSecrets::s_registered = false;

    KeeperOfSecrets::KeeperOfSecrets() :
            SlaaneshBase("Keeper of Secrets", 14, WOUNDS, 10, 4, false),
            m_livingWhip(Weapon::Type::Missile, "Living Whip", 6, 1, 3, 3, -1, 1),
            m_ritualKnifeOrHand(Weapon::Type::Melee, "Ritual Knife or Sinistrous Hand", 1, 1, 2, 3, -1, 1),
            m_greatblade(Weapon::Type::Melee, "Elegant Greatblade", 2, 4, 3, 3, -1, 2),
            m_impalingClaws(Weapon::Type::Melee, "Impaling Claws", 3, 2, 3, 3, -2, 5) {
        m_keywords = {CHAOS, DAEMON, GREATER_DAEMON, SLAANESH, HEDONITE, MONSTER, HERO, WIZARD, KEEPER_OF_SECRETS};
        m_weapons = {&m_livingWhip, &m_ritualKnifeOrHand, &m_greatblade, &m_impalingClaws};
        m_battleFieldRole = LeaderBehemoth;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool KeeperOfSecrets::configure(WeaponOption weapon, Lore lore) {
        auto model = new Model(BASESIZE, wounds());

        m_weapon = weapon;

        if (weapon == LivingWhip) {
            model->addMissileWeapon(&m_livingWhip);
        }
        if (weapon == RitualKnife || weapon == SinistrousHand) {
            model->addMeleeWeapon(&m_ritualKnifeOrHand);
        }
        model->addMeleeWeapon(&m_greatblade);
        model->addMeleeWeapon(&m_impalingClaws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *KeeperOfSecrets::Create(const ParameterList &parameters) {
        auto unit = new KeeperOfSecrets();
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, RitualKnife);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greaterDaemonLore[0]);

        bool ok = unit->configure(weapon, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KeeperOfSecrets::Init() {
        if (!s_registered) {
            static const std::array<int, 4> weapons = {RitualKnife, SinistrousHand, LivingWhip, ShiningAegis};
            static FactoryMethod factoryMethod = {
                    KeeperOfSecrets::Create,
                    KeeperOfSecrets::ValueToString,
                    KeeperOfSecrets::EnumStringToInt,
                    KeeperOfSecrets::ComputePoints,
                    {
                            EnumParameter("Weapon", RitualKnife, weapons),
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_greaterDaemonLore[0], g_greaterDaemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Keeper of Secrets", factoryMethod);
        }
    }

    void KeeperOfSecrets::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_greatblade.setAttacks(g_damageTable[damageIndex].m_greatbladeAttacks);
        m_impalingClaws.setDamage(g_damageTable[damageIndex].m_clawDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        Unit::onWounded();
    }

    int KeeperOfSecrets::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void KeeperOfSecrets::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Wounds KeeperOfSecrets::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        auto target = m_meleeTarget;
        if (target == nullptr) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            if (!units.empty()) {
                target = units.front();
            }
        }

        // Ritual Knife
        if ((m_weapon == RitualKnife) && target) {
            Wounds knifeWounds = {0, 0};
            int roll = Dice::rollD6();
            if (roll == 6) {
                knifeWounds = {0, Dice::rollD3()};
            } else if (roll >= 2) {
                knifeWounds = {0, 1};
            }
            target->applyDamage(knifeWounds);
            wounds += knifeWounds;
        }

        // Sinistrous Hand
        if ((m_weapon == SinistrousHand) && m_currentRecord.m_enemyModelsSlain) {
            // TODO: keep track of heroes slain to heal D6
            heal(Dice::rollD3());
        }
        return wounds;
    }

    Wounds KeeperOfSecrets::applyWoundSave(const Wounds &wounds) {
        if (m_weapon == ShiningAegis) {
            // Shining Aegis
            Dice::RollResult woundSaves, mortalSaves;
            Dice::rollD6(wounds.normal, woundSaves);
            Dice::rollD6(wounds.mortal, mortalSaves);

            Wounds totalWounds = wounds;
            totalWounds.normal -= woundSaves.rollsGE(6);
            totalWounds.normal = std::max(totalWounds.normal, 0);
            totalWounds.mortal -= mortalSaves.rollsGE(6);
            totalWounds.mortal = std::max(totalWounds.mortal, 0);

            return totalWounds;
        }
        return Unit::applyWoundSave(wounds);
    }

    void KeeperOfSecrets::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Dark Temptations
        auto hero = Board::Instance()->getUnitWithKeyword(this, GetEnemyId(owningPlayer()), HERO, 3.0);
        if (hero != nullptr) {
            // Heros _always_ refuse right now and take D3
            // TODO: give hero the choice
            hero->applyDamage({0, Dice::rollD3()});
        }
    }

    std::string KeeperOfSecrets::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == RitualKnife) return "Ritual Knife";
            else if (parameter.intValue == SinistrousHand) return "Sinistrous Hand";
            else if (parameter.intValue == LivingWhip) return "LivingWhip";
            else if (parameter.intValue == ShiningAegis) return "Shining Aegis";
        }
        return SlaaneshBase::ValueToString(parameter);
    }

    Wounds KeeperOfSecrets::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Delicate Precision
        if (!weapon->isMissile()) {
            if (woundRoll == 6) {
                return {0, weapon->damage()};
            }
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int KeeperOfSecrets::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ritual Knife") return RitualKnife;
        else if (enumString == "Sinistrous Hand") return SinistrousHand;
        else if (enumString == "Living Whip") return LivingWhip;
        else if (enumString == "Shining Aegis") return ShiningAegis;
        return SlaaneshBase::EnumStringToInt(enumString);
    }

    int KeeperOfSecrets::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Slannesh
