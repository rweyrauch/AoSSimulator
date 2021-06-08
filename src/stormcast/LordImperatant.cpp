/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/LordImperatant.h>
#include <iostream>
#include <Board.h>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {

    static const int g_basesize = 40;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 0;

    bool LordImperatant::s_registered = false;

    LordImperatant::LordImperatant(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Lord-Imperatant", 5, g_wounds, 9, 3, false, g_pointsPerUnit) {
        m_keywords = {ORDER, THUNDERSTRIKE, STORMCAST_ETERNAL, HERO, LORD_IMPERATANT};
        m_weapons = {&m_baton, &m_warhammer, &m_beakAndClaws};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_baton);
        model->addMeleeWeapon(&m_beakAndClaws);
        model->addMeleeWeapon(&m_warhammer);
        addModel(model);
    }

    Unit *LordImperatant::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LordImperatant(stormhost, trait, artefact, general);
    }

    void LordImperatant::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };
            s_registered = UnitFactory::Register("Lord-Imperatant", factoryMethod);
        }
    }

    int LordImperatant::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals