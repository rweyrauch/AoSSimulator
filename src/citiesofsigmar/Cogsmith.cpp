/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "citiesofsigmar/Cogsmith.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 60;

    bool Cogsmith::s_registered = false;

    Unit *Cogsmith::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Grudge_Raker);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Cogsmith(city, weapon, drug, trait, artefact, general);
    }

    std::string Cogsmith::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Grudge_Raker) {
                return "Grudge-raker";
            } else if (parameter.intValue == Cog_Axe) {
                return "Cog Axe";
            }
        }

        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Cogsmith::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Grudge-raker") {
            return Grudge_Raker;
        } else if (enumString == "Cog Axe") {
            return Cog_Axe;
        }
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Cogsmith::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapon = {Cogsmith::Grudge_Raker, Cogsmith::Cog_Axe};
            static FactoryMethod factoryMethod = {
                    Cogsmith::Create,
                    Cogsmith::ValueToString,
                    Cogsmith::EnumStringToInt,
                    Cogsmith::ComputePoints,
                    {
                            EnumParameter<2>("Weapon", Cogsmith::Grudge_Raker, weapon),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Cogsmith", factoryMethod);
        }
    }

    Cogsmith::Cogsmith(City city, WeaponOption weapon, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Cogsmith", 4, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_grudgeRaker(Weapon::Type::Missile, "Grudge-raker", 16, RAND_D3, 4, 3, -1, 1),
            m_pistols(Weapon::Type::Missile, "Duardin Pistols", 8, 2, 4, 3, -1, 1),
            m_cogAxe(Weapon::Type::Melee, "Cog Axe", 1, 4, 3, 4, 0, 1),
            m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, CITIES_OF_SIGMAR, IRONWELD_ARSENAL, HERO, ENGINEER, COGSMITH};
        m_weapons = {&m_grudgeRaker, &m_pistols, &m_cogAxe, &m_gunButt};
        m_battleFieldRole = Role::Leader;

        setNarcotic(narcotic);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_pistols);
        if (weapon == Grudge_Raker) {
            model->addMissileWeapon(&m_grudgeRaker);
        }

        model->addMeleeWeapon(&m_gunButt);
        if (weapon == Cog_Axe) {
            model->addMeleeWeapon(&m_cogAxe);
        }
        addModel(model);

        m_weaponOption = weapon;
    }

    int Cogsmith::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = CitizenOfSigmar::toHitModifier(weapon, target);

        // Free Arm
        if ((m_weaponOption == Grudge_Raker) && (weapon->name() == m_grudgeRaker.name())) mod++;
        if ((m_weaponOption == Cog_Axe) && (weapon->name() == m_cogAxe.name())) mod++;

        return mod;
    }

    void Cogsmith::onStartHero(PlayerId player) {
        CitizenOfSigmar::onStartHero(player);

        if (owningPlayer() == player) {
            // Master Engineer
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 3.0);
            for (auto unit : units) {
                if (unit->hasKeyword(IRONWELD_ARSENAL) && unit->hasKeyword(WAR_MACHINE) &&
                    (unit->remainingWounds() < unit->wounds())) {
                    // Heal one unit
                    unit->heal(Dice::RollD3());
                    break;
                }
            }
        }
    }

    int Cogsmith::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar