/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Beastlord.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 95;

    bool Beastlord::s_registered = false;

    Beastlord::Beastlord(Greatfray fray, CommandTrait trait, Artefact artefact, bool general) :
            BeastsOfChaosBase("Beastlord", 6, g_wounds, 7, 4, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, HERO, BEASTLORD};
        m_weapons.push_back(&m_pairedAxes);
        m_battleFieldRole = Role::Leader;

        setGreatfray(fray);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(general);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_pairedAxes);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    bool Beastlord::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *Beastlord::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_brayherdCommandTrait[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_brayherdArtefact[0]);
            auto general = GetBoolParam("General", parameters, false);
            return new Beastlord(fray, trait, artefact, general);
        }
        return nullptr;
    }

    void Beastlord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                            EnumParameter("Command Trait", g_brayherdCommandTrait[0], g_brayherdCommandTrait),
                            EnumParameter("Artefact", g_brayherdArtefact[0], g_brayherdArtefact),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Beastlord", factoryMethod);
        }
    }

    int Beastlord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
