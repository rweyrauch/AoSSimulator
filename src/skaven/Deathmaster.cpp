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
        auto unit = new Deathmaster();
        WeaponOption option = Weeping_Blades;

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_eshinClanCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_eshinArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Deathmaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
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

    Deathmaster::Deathmaster() :
            Skaventide("Deathmaster", 7, g_wounds, 5, 4, false),
            m_stars(Weapon::Type::Missile, "Eshin Throwing Stars", 12, 4, 4, 5, 0, 1),
            m_blades(Weapon::Type::Melee, "Weeping Blades", 1, 3, 3, 3, -1, RAND_D3),
            m_claws(Weapon::Type::Melee, "Fighting Claws", 1, 7, 3, 3, 0, 1) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_ESHIN, HERO, DEATHMASTER};
        m_weapons = {&m_stars, &m_blades, &m_claws};
        m_battleFieldRole = Leader;

        // Running Death
        m_runAndShoot = true;
    }

    bool Deathmaster::configure(WeaponOption option) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_stars);
        if (option == Weeping_Blades)
            model->addMeleeWeapon(&m_blades);
        else if (option == Fighting_Claws)
            model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
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
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int Deathmaster::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
