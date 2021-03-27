/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/Slambo.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Slambo::s_registered = false;

    Unit *Slambo::Create(const ParameterList &parameters) {
        auto unit = new Slambo();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        unit->configure();
        return unit;
    }

    void Slambo::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Slambo::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Slambo::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Slambo", factoryMethod);
        }
    }

    Slambo::Slambo() :
            SlavesToDarknessBase("Slambo", 5, g_wounds, 8, 4, false),
            m_hurledAxe(Weapon::Type::Missile, "Hurled Chaos Axe", 8, 1, 3, 3, -1, RAND_D3),
            m_chaosAxes(Weapon::Type::Melee, "Chaos Axes", 1, RAND_D6, 4, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, EXALTED_HERO_OF_CHAOS, SLAMBO};
        m_weapons = {&m_hurledAxe, &m_chaosAxes};
        m_battleFieldRole = Role::Leader;
    }

    void Slambo::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_hurledAxe);
        model->addMeleeWeapon(&m_chaosAxes);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    int Slambo::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //SlavesToDarkness