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
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);

        return new DarkoathWarqueen(legion, trait, artefact, general);
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

    DarkoathWarqueen::DarkoathWarqueen(DamnedLegion legion, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SlavesToDarknessBase("Darkoath Warqueen", 6, g_wounds, 8, 5, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EYE_OF_THE_GODS, HERO, DARKOATH_WARQUEEN};
        m_weapons = {&m_axe};
        m_battleFieldRole = Role::Leader;

        setDamnedLegion(legion);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        // Savage Duellist
        buffAbility(Ability::Fights_First, 1, {GamePhase::Combat, DurationRestOfGame, owningPlayer()});

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_axe);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    int DarkoathWarqueen::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    Wounds DarkoathWarqueen::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Savage Duellist
        if ((weapon->name() == m_axe.name()) && (target->hasKeyword(HERO) || target->hasKeyword(MONSTER))) {
            return {weapon->damage()+1, 0, Wounds::Source::Weapon_Melee};
        }
        return SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

} //namespace SlavesToDarkness
