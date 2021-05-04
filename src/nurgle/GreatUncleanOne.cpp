/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/GreatUncleanOne.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include <array>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 130;
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_bileToWound;
        int m_flailToWound;
        int m_swordAttacks;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {2, 2, 4},
                    {3, 2, 5},
                    {3, 3, 5},
                    {4, 3, 6},
                    {4, 4, 6}
            };

    bool GreatUncleanOne::s_registered = false;

    GreatUncleanOne::GreatUncleanOne(PlagueLegion legion, WeaponOptionOne optionOne, WeaponOptionTwo optionTwo, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            NurgleBase(legion,"Great Unclean One", 5, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_bile(Weapon::Type::Missile, "Noxious Bile", 7, RAND_D6, 3, 2, -2, 1),
            m_flail(Weapon::Type::Melee, "Plague Flail", 2, 3, 3, 2, -1, 2),
            m_bilesword(Weapon::Type::Melee, "Massive Bilesword", 2, 3, 4, 3, -2, 3),
            m_bileblade(Weapon::Type::Melee, "Bileblade", 2, 3, 3, 3, -1, 1),
            m_doomsdayBell(Weapon::Type::Melee, "Doomsday Bell", 2, 4, 4, 3, -1, 1),
            m_nurglings(Weapon::Type::Melee, "Host of Nurglings", 1, 3, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, NURGLE, MONSTER, HERO, WIZARD, GREAT_UNCLEAN_ONE, ROTIGUS};
        m_weapons = {&m_bile, &m_flail, &m_bilesword, &m_bileblade, &m_doomsdayBell, &m_nurglings};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalUnbinds = 2;
        m_totalSpells = 2;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bile);
        if (optionOne == Plague_Flail)
            model->addMeleeWeapon(&m_flail);
        else if (optionOne == Bileblade)
            model->addMeleeWeapon(&m_bileblade);
        if (optionTwo == Massive_Bilesword)
            model->addMeleeWeapon(&m_bilesword);
        else if (optionTwo == Doomsday_Bell)
            model->addMeleeWeapon(&m_doomsdayBell);
        model->addMeleeWeapon(&m_nurglings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *GreatUncleanOne::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto weaponOne = (WeaponOptionOne) GetEnumParam("Weapon One", parameters, Plague_Flail);
        auto weaponTwo = (WeaponOptionTwo) GetEnumParam("Weapon Two", parameters, Doomsday_Bell);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new GreatUncleanOne(legion, weaponOne, weaponTwo, lore, trait, artefact, general);
    }

    void GreatUncleanOne::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons1 = {Plague_Flail, Bileblade};
            static const std::array<int, 2> weapons2 = {Massive_Bilesword, Doomsday_Bell};
            static FactoryMethod factoryMethod = {
                    GreatUncleanOne::Create,
                    GreatUncleanOne::ValueToString,
                    GreatUncleanOne::EnumStringToInt,
                    GreatUncleanOne::ComputePoints,
                    {
                            EnumParameter("Weapon One", Plague_Flail, weapons1),
                            EnumParameter("Weapon Two", Massive_Bilesword, weapons2),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Great Unclean One", factoryMethod);
        }
    }

    void GreatUncleanOne::onWounded() {
        NurgleBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_bile.setToWound(g_damageTable[damageIndex].m_bileToWound);
        m_flail.setToWound(g_damageTable[damageIndex].m_flailToWound);
        m_bilesword.setAttacks(g_damageTable[damageIndex].m_swordAttacks);
    }

    size_t GreatUncleanOne::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void GreatUncleanOne::onCharged() {
        // Mountain of Loathsome Flesh
        if (m_charged) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto ip : units) {
                int roll = Dice::RollD6();
                if (roll >= 4) {
                    ip->applyDamage({Dice::RollD3(), 0}, this);
                }
            }
        }
        NurgleBase::onCharged();
    }

    std::string GreatUncleanOne::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon One") {
            if (parameter.intValue == Plague_Flail) return "Plague Flail";
            else if (parameter.intValue == Bileblade) return "Bileblade";
        } else if (std::string(parameter.name) == "Weapon Two") {
            if (parameter.intValue == Massive_Bilesword) return "Massive Bilesword";
            else if (parameter.intValue == Doomsday_Bell) return "Doomsday Bell";
        }
        return NurgleBase::ValueToString(parameter);
    }

    int GreatUncleanOne::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Plague Flail") return Plague_Flail;
        else if (enumString == "Bileblade") return Bileblade;
        else if (enumString == "Massive Bilesword") return Massive_Bilesword;
        else if (enumString == "Doomsday Bell") return Doomsday_Bell;
        return NurgleBase::EnumStringToInt(enumString);
    }

    Wounds GreatUncleanOne::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto unsavedWounds = NurgleBase::applyWoundSave(wounds, attackingUnit);

        // Blubber and Bile
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(unsavedWounds.normal, woundSaves);
        Dice::RollD6(unsavedWounds.mortal, mortalSaves);

        Wounds totalWounds = unsavedWounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        // On each 6+, the attacking unit takes a mortal wound.
        auto numReturnedMortals = mortalSaves.rollsGE(6) + woundSaves.rollsGE(6);
        if (numReturnedMortals > 0)
            attackingUnit->applyDamage({0, numReturnedMortals}, this);

        return totalWounds.clamp();
    }

    void GreatUncleanOne::onStartHero(PlayerId player) {
        NurgleBase::onStartHero(player);

        if (owningPlayer() == player) {
            // Corpulent Mass
            heal(Dice::RollD3());
        }
    }

    int GreatUncleanOne::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nurgle

