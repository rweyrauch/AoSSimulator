/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <UnitFactory.h>
#include <stormcast/StormsiresCursebreakers.h>
#include <iostream>
#include <spells/Empower.h>
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 3;
    static const int POINTS_PER_UNIT = 130;

    bool StormsiresCursebreakers::s_registered = false;

    StormsiresCursebreakers::StormsiresCursebreakers() :
            StormcastEternal("Stormsire's Cursebreakers", 5, WOUNDS, 8, 4, false),
            m_tempestBladeAndStave(Weapon::Type::Melee, "Tempest Blade and Stormstave", 1, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HAMMERS_OF_SIGMAR, SACROSANCT, CORPUSCANT,
                      WIZARD, EVOCATORS, STORMSIRES_CURSEBREAKERS};
        m_weapons = {&m_tempestBladeAndStave};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool StormsiresCursebreakers::configure(Lore lore) {
        auto ammis = new Model(BASESIZE, wounds());
        ammis->addMeleeWeapon(&m_tempestBladeAndStave);
        addModel(ammis);

        auto rastus = new Model(BASESIZE, wounds());
        rastus->addMeleeWeapon(&m_tempestBladeAndStave);
        addModel(rastus);

        m_knownSpells.push_back(std::make_unique<Empower>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Rerolls StormsiresCursebreakers::toSaveRerolls(const Weapon *weapon) const {
        // Celestial Lightning Arc
        if (weapon->isMissile()) {
            return RerollOnes;
        }

        return StormcastEternal::toSaveRerolls(weapon);
    }

    int StormsiresCursebreakers::generateMortalWounds(const Unit *unit) {
        auto mortalWounds = StormcastEternal::generateMortalWounds(unit);

        // Celestial Lightning Arc
        int roll = Dice::rollD6();
        if (roll >= 4) {
            mortalWounds++;
        }
        roll = Dice::rollD6();
        if (roll >= 4) {
            mortalWounds++;
        }

        return mortalWounds;
    }

    Unit *StormsiresCursebreakers::Create(const ParameterList &parameters) {
        auto *evos = new StormsiresCursebreakers();
        auto invigoration = (Lore) GetEnumParam("Lore of Invigoration", parameters, None);

        evos->setStormhost(Stormhost::Hammers_of_Sigmar);

        bool ok = evos->configure(invigoration);
        if (!ok) {
            delete evos;
            evos = nullptr;
        }
        return evos;
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
            return RerollOnes;
        }
        return StormcastEternal::toHitRerolls(weapon, target);
    }

    int StormsiresCursebreakers::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals
