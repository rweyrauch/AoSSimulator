/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/ArchWarlock.h>
#include <UnitFactory.h>
#include <Board.h>
#include <skaven/SkavenSpells.h>
#include <spells/MysticShield.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 160;

    bool ArchWarlock::s_registered = false;

    Unit *ArchWarlock::Create(const ParameterList &parameters) {
        auto unit = new ArchWarlock();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skryreCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_skryreArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_skryreLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ArchWarlock::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                        EnumParameter("Command Trait", g_skryreCommandTraits[0], g_skryreCommandTraits),
                        EnumParameter("Artefact", g_skryreArtefacts[0], g_skryreArtefacts),
                        EnumParameter("Lore", g_skryreLore[0], g_skryreLore),
                        BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Arch-Warlock", factoryMethod);
        }
    }

    ArchWarlock::ArchWarlock() :
            Skaventide("Arch-Warlock", 6, g_wounds, 6, 3, false),
            m_halberd(Weapon::Type::Melee, "Stormcage Halberd", 2, 1, 3, 3, -2, RAND_D3),
            m_claw(Weapon::Type::Melee, "Piston Claw", 1, 1, 4, 3, -2, 3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER, ARCH_WARLOCK};
        m_weapons = {&m_halberd, &m_claw};
        m_battleFieldRole = Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 1;
    }

    bool ArchWarlock::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_halberd);
        model->addMeleeWeapon(&m_claw);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateWarpLightningStorm(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds ArchWarlock::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        if (m_moreMoreFailed) {
            Wounds overloadWounds = {0, Dice::RollD6()};
            applyDamage(overloadWounds);
            wounds += overloadWounds;
            m_moreMoreFailed = false;
        }
        return wounds;
    }

    Wounds ArchWarlock::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // More-more Stormcage!
        if (weapon->name() == m_halberd.name()) {
            // Decide to overcharge (without using the hitRoll)
            if (moreMore()) {
                if (hitRoll == 1) {
                    m_moreMoreFailed = true;
                    return {0, 0};
                }

                m_moreMoreFailed = false;
                return {RAND_D6, 0};
            }
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void ArchWarlock::onRestore() {
        Unit::onRestore();

        m_moreMoreFailed = false;
        m_usedGauntlet = false;
    }

    void ArchWarlock::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        // Warpfire Gauntlet
        if ((owningPlayer() == player) && !m_usedGauntlet) {
            auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
            if (unit && distanceTo(unit) < 8.0) {
                if (Dice::RollD6() >= 2) unit->applyDamage({0, Dice::RollD3()});
                m_usedGauntlet = true;
            }
        }
    }

    int ArchWarlock::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
