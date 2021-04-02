/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <stormcast/KnightVexillor.h>
#include <iostream>
#include "UnitFactory.h"
#include "StormcastEternalsPrivate.h"

namespace StormcastEternals {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool KnightVexillor::s_registered = false;

    KnightVexillor::KnightVexillor(Stormhost stormhost, CommandTrait trait, Artefact artefact, bool isGeneral) :
            StormcastEternal(stormhost, "Knight-Vexillor", 5, g_wounds, 8, 3, false),
            m_warhammer(Weapon::Type::Melee, "Warhammer", 1, 4, 4, 3, 0, 1) {
        m_keywords = {ORDER, CELESTIAL, HUMAN, STORMCAST_ETERNAL, HERO, KNIGHT_VEXILLOR};
        m_weapons = {&m_warhammer};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalChargeReroll.connect(this, &KnightVexillor::iconOfWarChargeReroll, &m_iconOfWarSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warhammer);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    KnightVexillor::~KnightVexillor() {
        m_iconOfWarSlot.disconnect();
    }

    Unit *KnightVexillor::Create(const ParameterList &parameters) {
        auto stormhost = (Stormhost) GetEnumParam("Stormhost", parameters, g_stormhost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsOfTheTempests[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new KnightVexillor(stormhost, trait, artefact, general);
     }

    void KnightVexillor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    StormcastEternal::ValueToString,
                    StormcastEternal::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Stormhost", g_stormhost[0], g_stormhost),
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            BoolParameter("General")
                    },
                    ORDER,
                    {STORMCAST_ETERNAL}
            };

            s_registered = UnitFactory::Register("Knight-Vexillor", factoryMethod);
        }
    }

    Rerolls KnightVexillor::iconOfWarChargeReroll(const Unit *unit) {
        if (isFriendly(unit) && unit->hasKeyword(STORMCAST_ETERNAL) && (distanceTo(unit) <= 18.0))
            return Rerolls::Failed;
        return Rerolls::None;
    }

    int KnightVexillor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace StormcastEternals