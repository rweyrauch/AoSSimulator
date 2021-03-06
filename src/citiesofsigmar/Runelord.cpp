/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/Runelord.h>
#include <UnitFactory.h>
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool Runelord::s_registered = false;

    Runelord::Runelord(City city, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Runelord", 4, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_runeStaff(Weapon::Type::Melee, "Rune Staff", 1, 1, 4, 3, 0, RAND_D3),
            m_forgehammer(Weapon::Type::Melee, "Forgehammer", 1, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, PRIEST, RUNELORD};
        m_weapons = {&m_runeStaff, &m_forgehammer};
        m_battleFieldRole = Role::Leader;

        // Runes of Spellbreaking
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);
        setNarcotic(narcotic);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_runeStaff);
        model->addMeleeWeapon(&m_forgehammer);
        addModel(model);
    }

    Unit *Runelord::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Runelord(city, drug, trait, artefact, general);
    }

    void Runelord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Runelord::Create,
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    Runelord::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Runelord", factoryMethod);
        }
    }

    int Runelord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar