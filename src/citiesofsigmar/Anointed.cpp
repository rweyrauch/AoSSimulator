/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "citiesofsigmar/Anointed.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Anointed::s_registered = false;

    Unit *Anointed::Create(const ParameterList &parameters) {
        auto unit = new Anointed();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Anointed::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Anointed::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Anointed::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Anointed::Create,
                    Anointed::ValueToString,
                    Anointed::EnumStringToInt,
                    Anointed::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Anointed", factoryMethod);
        }
    }

    Anointed::Anointed() :
            CitizenOfSigmar("Anointed", 6, g_wounds, 7, 4, false),
            m_halberd(Weapon::Type::Melee, "Great Phoenix Halberd", 2, 4, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, PHOENIX_TEMPLE, HERO, ANOINTED};
        m_weapons = {&m_halberd};
        m_battleFieldRole = Leader;
        // Blessing of the Ur-Phoenix
        m_totalUnbinds = 1;
    }

    bool Anointed::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_halberd);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds Anointed::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Witness to Destiny
        Dice::RollResult woundSaves, mortalSaves;
        Dice::RollD6(wounds.normal, woundSaves);
        Dice::RollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(4);
        totalWounds.normal = std::max(totalWounds.normal, 0);
        totalWounds.mortal -= mortalSaves.rollsGE(4);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int Anointed::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar