/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarlockEngineer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <skaven/SkavenSpells.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool WarlockEngineer::s_registered = false;

    Unit *WarlockEngineer::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_skryreLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skryreCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_skryreArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WarlockEngineer(lore, trait, artefact, general);
    }

    void WarlockEngineer::Init() {
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

            s_registered = UnitFactory::Register("Warlock Engineer", factoryMethod);
        }
    }

    WarlockEngineer::WarlockEngineer(Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Warlock Engineer", 6, g_wounds, 5, 5, false, g_pointsPerUnit),
            m_pistol(Weapon::Type::Missile, "Warplock Pistol", 9, 1, 3, 3, -1, RAND_D3),
            m_blade(Weapon::Type::Melee, "Warp-energy Blade", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER};
        m_weapons = {&m_pistol, &m_blade};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pistol);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateWarpLightning(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void WarlockEngineer::onRestore() {
        Skaventide::onRestore();

        m_moreMoreFailed = false;
    }

    void WarlockEngineer::onEndCombat(PlayerId player) {
        Skaventide::onEndCombat(player);

        if (m_moreMoreFailed) {
            Wounds overloadWounds = {0, Dice::RollD6()};
            applyDamage(overloadWounds, this);
            m_moreMoreFailed = false;
        }
    }

    Wounds WarlockEngineer::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // More-more Warp-energy!
        if (weapon->name() == m_blade.name()) {
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
        return Skaventide::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int WarlockEngineer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
