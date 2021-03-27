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
    static const int g_pointsPerUnit = 90;

    bool Beastlord::s_registered = false;

    Beastlord::Beastlord() :
            BeastsOfChaosBase("Beastlord", 6, g_wounds, 7, 4, false) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, HERO, BEASTLORD};
        m_weapons.push_back(&m_pairedAxes);
        m_battleFieldRole = Role::Leader;
    }


    Beastlord::Beastlord(Greatfray fray, CommandTrait trait, Artefact artefact, bool general) :
            Beastlord() {
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

    Rerolls Beastlord::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Dual Axes
        if (weapon->name() == m_pairedAxes.name()) {
            return Rerolls::Ones;
        }
        return BeastsOfChaosBase::toHitRerolls(weapon, target);
    }

    Rerolls Beastlord::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Hatred of Heroes
        if (target->hasKeyword(HERO)) {
            return Rerolls::Failed;
        }
        return BeastsOfChaosBase::toWoundRerolls(weapon, target);
    }

    int Beastlord::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
