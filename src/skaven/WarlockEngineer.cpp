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
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool WarlockEngineer::s_registered = false;

    Unit *WarlockEngineer::Create(const ParameterList &parameters) {
        auto unit = new WarlockEngineer();

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

    WarlockEngineer::WarlockEngineer() :
            Skaventide("Warlock Engineer", 6, WOUNDS, 5, 5, false),
            m_pistol(Weapon::Type::Missile, "Warplock Pistol", 9, 1, 3, 3, -1, RAND_D3),
            m_blade(Weapon::Type::Melee, "Warp-energy Blade", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_SKRYRE, HERO, WIZARD, WARLOCK_ENGINEER};
        m_weapons = {&m_pistol, &m_blade};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool WarlockEngineer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_pistol);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateWarpLightning(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void WarlockEngineer::onRestore() {
        Unit::onRestore();

        m_moreMoreFailed = false;
    }

    Wounds WarlockEngineer::onEndCombat(PlayerId player) {
        auto wounds = Unit::onEndCombat(player);

        if (m_moreMoreFailed) {
            Wounds overloadWounds = {0, Dice::rollD6()};
            applyDamage(overloadWounds);
            wounds += overloadWounds;
            m_moreMoreFailed = false;
        }
        return wounds;
    }

    Wounds WarlockEngineer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
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
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int WarlockEngineer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Skaven
