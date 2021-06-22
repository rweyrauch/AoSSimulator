/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightVexillorApotheosis.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 110;

    bool KnightVexillorApotheosis::s_registered = false;

    KnightVexillorApotheosis::KnightVexillorApotheosis(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Vexillor with Banner of Apotheosis", 5, g_wounds, 8, 3, false, g_pointsPerUnit),
            m_warblade(Weapon::Type::Melee, "Warblade", 1, 4, 3, 3, -1, 12) {
        m_keywords = {ORDER, STORMCAST_ETERNAL, THUNDERSTRIKE, HERO, TOTEM, KNIGHT_VEXILLOR, KNIGHT_VEXILLOR_WITH_BANNER_OF_APOTHEOSIS};
        m_weapons = {&m_warblade};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warblade);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *KnightVexillorApotheosis::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsTotems[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightVexillorApotheosis(stormhost, trait, artefact, general);
     }

    void KnightVexillorApotheosis::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Artefact", g_artefactsTotems[0], g_artefactsTotems),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Vexillor with Banner of Apotheosis", factoryMethod);
        }
    }

    int KnightVexillorApotheosis::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals