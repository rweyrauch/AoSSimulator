/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/DarkoathWarqueen.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 90;

    bool DarkoathWarqueen::s_registered = false;

    Unit *DarkoathWarqueen::Create(const ParameterList &parameters) {
        auto unit = new DarkoathWarqueen();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DarkoathWarqueen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DarkoathWarqueen::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    DarkoathWarqueen::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Darkoath Warqueen", factoryMethod);
        }
    }

    DarkoathWarqueen::DarkoathWarqueen() :
            SlavesToDarknessBase("Darkoath Warqueen", 6, g_wounds, 8, 5, false),
            m_axe(Weapon::Type::Melee, "Rune-etched Axe", 1, 6, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_WARQUEEN};
        m_weapons = {&m_axe};
        m_battleFieldRole = Role::Leader;
    }

    bool DarkoathWarqueen::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axe);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    int DarkoathWarqueen::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace SlavesToDarkness
