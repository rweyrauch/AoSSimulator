/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/Deathmaster.h>
#include <UnitFactory.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Deathmaster::s_registered = false;

    Unit *Deathmaster::Create(const ParameterList &parameters) {
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Weeping_Blades);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_eshinClanCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_eshinArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Deathmaster(option, trait, artefact, general);
    }

    void Deathmaster::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Weeping_Blades, Fighting_Claws};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Weeping_Blades, weapons),
                            EnumParameter("Command Trait", g_eshinClanCommandTraits[0], g_eshinClanCommandTraits),
                            EnumParameter("Artefact", g_eshinArtefacts[0], g_eshinArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Deathmaster", factoryMethod);
        }
    }

    Deathmaster::Deathmaster(WeaponOption option, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Deathmaster", 7, g_wounds, 5, 4, false, g_pointsPerUnit),
            m_stars(Weapon::Type::Missile, "Eshin Throwing Stars", 12, 4, 4, 5, 0, 1),
            m_blades(Weapon::Type::Melee, "Weeping Blades", 1, 3, 3, 3, -1, RAND_D3),
            m_claws(Weapon::Type::Melee, "Fighting Claws", 1, 7, 3, 3, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, HERO, DEATHMASTER};
        m_weapons = {&m_stars, &m_blades, &m_claws};
        m_battleFieldRole = Role::Leader;

        // Running Death
        m_runAndShoot = true;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_stars);
        if (option == Weeping_Blades)
            model->addMeleeWeapon(&m_blades);
        else if (option == Fighting_Claws)
            model->addMeleeWeapon(&m_claws);
        addModel(model);
    }

    std::string Deathmaster::ValueToString(const Parameter &parameter) {
        return Skaventide::ValueToString(parameter);
    }

    int Deathmaster::EnumStringToInt(const std::string &enumString) {
        return Skaventide::EnumStringToInt(enumString);
    }

    int Deathmaster::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Throwing Stars
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_stars.name())) {
            return 2;
        }
        return Skaventide::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int Deathmaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
