/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/TerradonChief.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 50;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 70;

    bool TerradonChief::s_registered = false;

    TerradonChief::TerradonChief() :
            SeraphonBase("Terradon Chief", 16, WOUNDS, 6, 6, true),
            m_skyblade(Weapon::Type::Melee, "Skyblade", 1, 3, 3, 4, -1, 1),
            m_jaws(Weapon::Type::Melee, "Razor-sharp Jaws", 1, 4, 4, 4, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, TERRADON, HERO, TERRADON_CHIEF};
        m_weapons = {&m_skyblade, &m_jaws};
        m_hasMount = true;
    }

    bool TerradonChief::configure() {
        // Add the Alpha
        auto alpha = new Model(BASESIZE, wounds());
        alpha->addMeleeWeapon(&m_skyblade);
        alpha->addMeleeWeapon(&m_jaws);
        addModel(alpha);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *TerradonChief::Create(const ParameterList &parameters) {
        auto unit = new TerradonChief();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TerradonChief::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_skinkCommandTrait[0], g_skinkCommandTrait),
                            EnumParameter("Artefact", g_vestmentsOfThePriesthood[0], g_vestmentsOfThePriesthood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Terradon Chief", factoryMethod);
        }
    }

    int TerradonChief::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
