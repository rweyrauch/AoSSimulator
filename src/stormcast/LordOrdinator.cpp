/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordOrdinator.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool LordOrdinator::s_registered = false;

    LordOrdinator::LordOrdinator() :
            StormcastEternal("Lord-Ordinator", 5, WOUNDS, 9, 4, false),
            m_astralHammers(Weapon::Type::Melee, "Astral Hammers", 1, 6, 4, 3, 0, 1),
            m_astralGrandhammer(Weapon::Type::Melee, "Astral Grandhammer", 1, 3, 3, 3, -1, 2) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, SACROSANCT, HERO, LORD_ORDINATOR};
        m_weapons = {&m_astralHammers, &m_astralGrandhammer};

        s_globalToHitMod.connect(this, &LordOrdinator::arcaneEngineer, &m_connection);
    }

    LordOrdinator::~LordOrdinator() {
        m_connection.disconnect();
    }

    bool LordOrdinator::configure(LordOrdinator::WeaponOption weaponOption) {
        m_weaponOption = weaponOption;

        auto model = new Model(BASESIZE, wounds());
        if (m_weaponOption == AstralHammers) {
            model->addMeleeWeapon(&m_astralHammers);
        } else {
            model->addMeleeWeapon(&m_astralGrandhammer);
        }
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *LordOrdinator::Create(const ParameterList &parameters) {
        auto unit = new LordOrdinator();
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapon", parameters, false);

        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        unit->setStormhost(stormhost);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void LordOrdinator::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {AstralHammers, AstralGrandhammer};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", AstralHammers, weapons),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Ordinator", factoryMethod);
        }
    }

    std::string LordOrdinator::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == AstralHammers) {
                return "Astral Hammers";
            } else if (parameter.intValue == AstralGrandhammer) {
                return "Astral Grandhammer";
            }
        }

        return StormcastEternal::ValueToString(parameter);
    }

    int LordOrdinator::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Astral Hammers") {
            return AstralHammers;
        } else if (enumString == "Astral Grandhammer") {
            return AstralGrandhammer;
        }

        return StormcastEternal::EnumStringToInt(enumString);
    }

    Wounds LordOrdinator::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Comet Strike
        if (hitRoll == 6 && weapon->name() == m_astralGrandhammer.name()) {
            return {0, 2};
        }
        if (hitRoll == 6 && weapon->name() == m_astralHammers.name()) {
            // remember this
            m_meteoricSlam.push_back(target);
        }
        return StormcastEternal::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void LordOrdinator::onStartCombat(PlayerId player) {
        StormcastEternal::onStartCombat(player);

        m_meteoricSlam.clear();
    }

    Wounds LordOrdinator::onEndCombat(PlayerId player) {
        auto wounds = StormcastEternal::onEndCombat(player);

        // Meteoric Slam
        if (m_meteoricSlam.size() > 1) {
            // TODO: assuming all targets are the same unit and that
            // the only unit the LO hit was the target unit
            if (m_meteoricSlam.front() == m_meleeTarget) {
                Wounds slamWounds = {0, Dice::rollD3()};
                m_meleeTarget->applyDamage(slamWounds);
                wounds += slamWounds;
            }
        }

        return wounds;
    }

    int LordOrdinator::arcaneEngineer(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *target) {
        if (target->hasKeyword(ORDER) && target->hasKeyword(WAR_MACHINE) &&
            (target->owningPlayer() == owningPlayer()) && (distanceTo(target) <= 9.0)) {
            return 1;
        }

        return 0;
    }

    int LordOrdinator::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace StormcastEternals