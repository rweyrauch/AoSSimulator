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
    static const int BASESIZE = 130;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 340;

    struct TableEntry {
        int m_bileToWound;
        int m_flailToWound;
        int m_swordAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {2, 2, 4},
                    {3, 2, 5},
                    {3, 3, 5},
                    {4, 3, 6},
                    {4, 4, 6}
            };

    bool GreatUncleanOne::s_registered = false;

    GreatUncleanOne::GreatUncleanOne() :
            NurgleBase("Great Unclean One", 5, WOUNDS, 10, 4, false),
            m_bile(Weapon::Type::Missile, "Noxious Bile", 7, RAND_D6, 3, 2, -2, 1),
            m_flail(),
            m_bilesword(),
            m_bileblade(),
            m_doomsdayBell(),
            m_nurglings(Weapon::Type::Melee, "Host of Nurglings", 1, 3, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, NURGLE, MONSTER, HERO, WIZARD, GREAT_UNCLEAN_ONE, ROTIGUS};
        m_weapons = {&m_bile, &m_flail, &m_bilesword, &m_bileblade, &m_doomsdayBell, &m_nurglings};

        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    bool GreatUncleanOne::configure(WeaponOptionOne optionOne, WeaponOptionTwo optionTwo, Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_bile);
        if (optionOne == PlagueFlail)
            model->addMeleeWeapon(&m_flail);
        else if (optionOne == Bileblade)
            model->addMeleeWeapon(&m_bileblade);
        if (optionTwo == MassiveBilesword)
            model->addMeleeWeapon(&m_bilesword);
        else if (optionTwo == DoomsdayBell)
            model->addMeleeWeapon(&m_doomsdayBell);
        model->addMeleeWeapon(&m_nurglings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *GreatUncleanOne::Create(const ParameterList &parameters) {
        auto unit = new GreatUncleanOne();

        auto weaponOne = (WeaponOptionOne) GetEnumParam("Weapon One", parameters, PlagueFlail);
        auto weaponTwo = (WeaponOptionTwo) GetEnumParam("Weapon Two", parameters, DoomsdayBell);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        bool ok = unit->configure(weaponOne, weaponTwo, lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GreatUncleanOne::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons1 = {PlagueFlail, Bileblade};
            static const std::array<int, 2> weapons2 = {MassiveBilesword, DoomsdayBell};
            static FactoryMethod factoryMethod = {
                    GreatUncleanOne::Create,
                    GreatUncleanOne::ValueToString,
                    GreatUncleanOne::EnumStringToInt,
                    GreatUncleanOne::ComputePoints,
                    {
                            EnumParameter("Weapon One", PlagueFlail, weapons1),
                            EnumParameter("Weapon Two", MassiveBilesword, weapons2),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore)
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Great Unclean One", factoryMethod);
        }
    }

    void GreatUncleanOne::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_bile.setToWound(g_damageTable[damageIndex].m_bileToWound);
        m_flail.setToWound(g_damageTable[damageIndex].m_flailToWound);
        m_bilesword.setAttacks(g_damageTable[damageIndex].m_swordAttacks);
    }

    int GreatUncleanOne::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
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
                int roll = Dice::rollD6();
                if (roll >= 4) {
                    ip->applyDamage({Dice::rollD3(), 0});
                }
            }
        }
        Unit::onCharged();
    }

    std::string GreatUncleanOne::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon One") {
            if (parameter.intValue == PlagueFlail) return "Plague Flail";
            else if (parameter.intValue == Bileblade) return "Bileblade";
        } else if (std::string(parameter.name) == "Weapon Two") {
            if (parameter.intValue == MassiveBilesword) return "Massive Bilesword";
            else if (parameter.intValue == DoomsdayBell) return "Doomsday Bell";
        }
        return NurgleBase::ValueToString(parameter);
    }

    int GreatUncleanOne::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Plague Flail") return PlagueFlail;
        else if (enumString == "Bileblade") return Bileblade;
        else if (enumString == "Massive Bilesword") return MassiveBilesword;
        else if (enumString == "Doomsday Bell") return DoomsdayBell;
        return NurgleBase::EnumStringToInt(enumString);
    }

    Wounds GreatUncleanOne::applyWoundSave(const Wounds &wounds) {
        // Blubber and Bile
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        // TODO: on 6+ attacking unit takes a mortal wound.

        return totalWounds.clamp();
    }

    void GreatUncleanOne::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Corpulent Mass
            heal(Dice::rollD3());
        }
    }

    int GreatUncleanOne::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle

