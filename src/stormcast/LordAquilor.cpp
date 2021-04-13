/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordAquilor.h>
#include <iostream>

#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 170;

    bool LordAquilor::s_registered = false;

    LordAquilor::LordAquilor(Stormhost stormhost, bool astralCompass, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Lord-Aquilor", 12, g_wounds, 9, 3, false),
            m_boltstormPistol(Weapon::Type::Missile, "Heavy Boltstorm Pistol", 9, 4, 3, 3, 0, 1),
            m_starboundBlade(Weapon::Type::Melee, "Starbound Blade", 1, 3, 3, 3, -1, 2),
            m_shockHandaxe(Weapon::Type::Melee, "Shock Handaxe", 1, 2, 3, 3, 0, 1),
            m_beakAndClaws(Weapon::Type::Melee, "Razor Beak and Claws", 1, 3, 3, 3, -2, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, GRYPH_CHARGER, STORMCAST_ETERNAL, HERO, LORD_AQUILOR};
        m_weapons = {&m_boltstormPistol, &m_starboundBlade, &m_shockHandaxe, &m_beakAndClaws};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_boltstormPistol);
        model->addMeleeWeapon(&m_starboundBlade);
        model->addMeleeWeapon(&m_shockHandaxe);
        model->addMeleeWeapon(&m_beakAndClaws);
        addModel(model);

        m_astralCompass = astralCompass;

        m_points = g_pointsPerUnit;
    }

    Unit *LordAquilor::Create(const ParameterList &parameters) {
        bool astralCompass = GetBoolParam("Astral Compass", parameters, false);
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordAquilor(stormhost, astralCompass, trait, artefact, general);
    }

    void LordAquilor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("Astral Compass"),
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Lord-Aquilor", factoryMethod);
        }
    }

    Wounds LordAquilor::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Aethereal Strike
        if ((hitRoll == 6) && (weapon->name() == m_beakAndClaws.name())) {
            return {0, 1};
        }
        return StormcastEternal::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int LordAquilor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals