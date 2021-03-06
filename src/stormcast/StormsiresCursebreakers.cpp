/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <stormcast/StormsiresCursebreakers.h>
#include <iostream>
#include <stormcast/Empower.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 3;
    static const int g_pointsPerUnit = 130;

    bool StormsiresCursebreakers::s_registered = false;

    StormsiresCursebreakers::StormsiresCursebreakers(Lore lore) :
            StormcastEternal(Stormhost::Hammers_Of_Sigmar, "Stormsire's Cursebreakers", 5, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, CORPUSCANT,
                      WIZARD, EVOCATORS, STORMSIRES_CURSEBREAKERS};
        m_weapons = {&m_tempestBladeAndStave};

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        auto ammis = new Model(g_basesize, wounds());
        ammis->addMeleeWeapon(&m_tempestBladeAndStave);
        addModel(ammis);

        auto rastus = new Model(g_basesize, wounds());
        rastus->addMeleeWeapon(&m_tempestBladeAndStave);
        addModel(rastus);

        m_knownSpells.push_back(std::make_unique<Empower>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = g_pointsPerUnit;
    }

    Rerolls StormsiresCursebreakers::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Celestial Lightning Arc
        if (weapon->isMissile()) {
            return Rerolls::Ones;
        }

        return StormcastEternal::toSaveRerolls(weapon, attacker);
    }

    int StormsiresCursebreakers::generateMortalWounds(const Unit *unit) {
        auto mortalWounds = StormcastEternal::generateMortalWounds(unit);

        // Celestial Lightning Arc
        int roll = Dice::RollD6();
        if (roll >= 4) {
            mortalWounds++;
        }
        roll = Dice::RollD6();
        if (roll >= 4) {
            mortalWounds++;
        }

        return mortalWounds;
    }

    Unit *StormsiresCursebreakers::Create(const ParameterList &parameters) {
        auto invigoration = (Lore) GetEnumParam("Lore of Invigoration", parameters, g_loreOfInvigoration[0]);
        return new StormsiresCursebreakers(invigoration);
    }

    void StormsiresCursebreakers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore of Invigoration", g_loreOfInvigoration[0], g_loreOfInvigoration)
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Stormsire's Cursebreakers", factoryMethod);
        }
    }

    std::string StormsiresCursebreakers::ValueToString(const Parameter &parameter) {
        return StormcastEternal::ValueToString(parameter);
    }

    int StormsiresCursebreakers::EnumStringToInt(const std::string &enumString) {
        return StormcastEternal::EnumStringToInt(enumString);
    }

    Rerolls StormsiresCursebreakers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Blessed Banishment
        if (target->hasKeyword(DEATH) || target->hasKeyword(CHAOS)) {
            return Rerolls::Ones;
        }
        return StormcastEternal::toHitRerolls(weapon, target);
    }

    int StormsiresCursebreakers::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals
