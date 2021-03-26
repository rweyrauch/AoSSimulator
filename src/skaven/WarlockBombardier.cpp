/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/WarlockBombardier.h>
#include <UnitFactory.h>
#include <skaven/SkavenSpells.h>
#include <spells/MysticShield.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool WarlockBombardier::s_registered = false;

    Unit *WarlockBombardier::Create(const ParameterList &parameters) {
        auto unit = new WarlockBombardier();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skryreCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_skryreArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_skryreLore[0]);

        unit->configure(lore);
        return unit;
    }

    void WarlockBombardier::Init() {
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

            s_registered = UnitFactory::Register("Warlock Bombardier", factoryMethod);
        }
    }

    WarlockBombardier::WarlockBombardier() :
            Skaventide("Warlock Bombardier", 6, g_wounds, 5, 5, false),
            m_doomrocket(Weapon::Type::Missile, "Doomrocket", 18, 1, 4, 3, -1, RAND_D6),
            m_pole(Weapon::Type::Melee, "Firing Pole", 1, 1, 5, 5, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER,
                      WARLOCK_BOMBARDIER};
        m_weapons = {&m_doomrocket, &m_pole};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool WarlockBombardier::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_doomrocket);
        model->addMeleeWeapon(&m_pole);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateWarpLightning(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void WarlockBombardier::onRestore() {
        Unit::onRestore();

        m_moreMoreFailed = false;
    }

    void WarlockBombardier::onEndShooting(PlayerId player) {
        Unit::onEndCombat(player);

        if (m_moreMoreFailed) {
            Wounds overloadWounds = {0, Dice::RollD6()};
            applyDamage(overloadWounds, this);
            m_moreMoreFailed = false;
        }
    }

    Wounds WarlockBombardier::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // More-more Doomrocket!
        if (weapon->name() == m_doomrocket.name()) {
            // Decide to overcharge (without using the hitRoll)
            if (moreMore()) {
                if (hitRoll == 1) {
                    m_moreMoreFailed = true;
                    return {0, 0};
                }

                m_moreMoreFailed = false;
                return {RAND_2D6, 0};
            }
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int WarlockBombardier::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }
} //namespace Skaven
