/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/MegabossOnMawKrusha.h>
#include <UnitFactory.h>
#include <Board.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 160;
    static const int g_wounds = 15;
    static const int g_pointsPerUnit = 460;

    bool MegabossOnMawKrusha::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_fistsAttacks;
        int m_bulkDice;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 10, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 8, 8},
                    {10, 7, 7},
                    {8,  6, 6},
                    {6,  5, 5},
                    {4,  4, 4}
            };

    MegabossOnMawKrusha::MegabossOnMawKrusha() :
            Ironjawz("Megaboss on Maw-Krusha", 12, g_wounds, 8, 3, true),
            m_bellow(Weapon::Type::Missile, "Innard-bursting Bellow", 8, RAND_D6, 2, 3, -1, 1),
            m_hackaAndChoppa(Weapon::Type::Melee, "Boss Gore-hacka and Choppa", 2, 8, 3, 3, -1, 2),
            m_ripToofFist(Weapon::Type::Melee, "Boss Choppa and Rip-toof Fist", 1, 6, 3, 3, -1, 2),
            m_fistsAndTail(Weapon::Type::Melee, "Mighty Fists and Tail", 1, 8, 3, 3, -2, 2) {
        m_keywords = {DESTRUCTION, ORRUK, MAW_KRUSHA, IRONJAWZ, MONSTER, HERO, MEGABOSS, GORDRAKK};
        m_weapons = {&m_bellow, &m_hackaAndChoppa, &m_ripToofFist, &m_fistsAndTail};
        m_hasMount = true;
        m_fistsAndTail.setMount(true);
        m_battleFieldRole = Leader_Behemoth;
    }

    bool MegabossOnMawKrusha::configure(WeaponOption weapons) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bellow);

        if (weapons == Hacka_And_Choppa) {
            model->addMeleeWeapon(&m_hackaAndChoppa);
        } else if (weapons == Choppa_And_Riptoof_Fist) {
            model->addMeleeWeapon(&m_ripToofFist);
        }
        model->addMeleeWeapon(&m_fistsAndTail);

        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BuffModifierCommandAbility>(this, "Go on Ladz, Get Stuck In!", 18, 18, Phase::Combat,
                                                                                  To_Hit_Melee, 1, Abilities::Target::SelfAndFriendly, std::vector<Keyword>{IRONJAWZ}));

        m_weaponOption = weapons;

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *MegabossOnMawKrusha::Create(const ParameterList &parameters) {
        auto unit = new MegabossOnMawKrusha();
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Hacka_And_Choppa);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_bossCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_bossArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto mount = (MountTrait) GetEnumParam("Mount Trait", parameters, g_mountTrait[0]);
        unit->setMountTrait(mount);

        bool ok = unit->configure(weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MegabossOnMawKrusha::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Hacka_And_Choppa, Choppa_And_Riptoof_Fist};
            static FactoryMethod factoryMethod = {
                    MegabossOnMawKrusha::Create,
                    MegabossOnMawKrusha::ValueToString,
                    MegabossOnMawKrusha::EnumStringToInt,
                    MegabossOnMawKrusha::ComputePoints,
                    {
                            EnumParameter("Weapons", Hacka_And_Choppa, weapons),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_bossCommandTraits[0], g_bossCommandTraits),
                            EnumParameter("Artefact", g_bossArtefacts[0], g_bossArtefacts),
                            EnumParameter("Mount Trait", g_mountTrait[0], g_mountTrait),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Megaboss on Maw-Krusha", factoryMethod);
        }
    }

    void MegabossOnMawKrusha::onRestore() {
        Ironjawz::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void MegabossOnMawKrusha::onWounded() {
        Ironjawz::onWounded();
        const int damageIndex = getDamageTableIndex();
        m_fistsAndTail.setAttacks(g_damageTable[damageIndex].m_fistsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int MegabossOnMawKrusha::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void MegabossOnMawKrusha::onCharged() {
        Ironjawz::onCharged();

        // Destructive Bulk
        const int threshold = (m_mountTrait == MountTrait::Heavy_Un) ? 4 : 5;
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        if (!units.empty()) {
            auto unit = units.front();
            Dice::RollResult result;
            Dice::RollD6(g_damageTable[getDamageTableIndex()].m_bulkDice, result);
            Wounds bulkWounds = {0, result.rollsGE(threshold)};
            unit->applyDamage(bulkWounds, this);
        }
    }

    void MegabossOnMawKrusha::onStartCombat(PlayerId player) {
        m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

        Ironjawz::onStartCombat(player);
    }

    void MegabossOnMawKrusha::onEndCombat(PlayerId player) {
        // Strength from Victory
        if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat) {
            heal(1);
            m_hackaAndChoppa.setAttacks(m_hackaAndChoppa.attacks() + 1);
            m_ripToofFist.setAttacks(m_ripToofFist.attacks() + 1);
        }
        Ironjawz::onEndCombat(player);
    }

    Wounds MegabossOnMawKrusha::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = Ironjawz::computeReturnedDamage(weapon, saveRoll);
        // Rip-toof Fist
        if ((saveRoll == 6) && (m_weaponOption == Choppa_And_Riptoof_Fist)) {
            wounds += {0, 1};
        }
        return wounds;
    }

    std::string MegabossOnMawKrusha::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Hacka_And_Choppa) {
                return "Boss Gore-hacka and Choppa";
            } else if (parameter.intValue == Choppa_And_Riptoof_Fist) {
                return "Boss Choppa and Rip-toof Fist";
            }
        }

        return Ironjawz::ValueToString(parameter);
    }

    int MegabossOnMawKrusha::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Boss Gore-hacka and Choppa") {
            return Hacka_And_Choppa;
        } else if (enumString == "Boss Choppa and Rip-toof Fist") {
            return Choppa_And_Riptoof_Fist;
        }
        return Ironjawz::EnumStringToInt(enumString);
    }

    int MegabossOnMawKrusha::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Wounds
    MegabossOnMawKrusha::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Ironjawz::weaponDamage(weapon, target, hitRoll, woundRoll);
        if (m_mountTrait == MountTrait::Mean_Un) {
            if (weapon->name() == m_fistsAndTail.name()) {
                damage.normal++;
            }
        }
        return damage;
    }

} //namespace Ironjawz