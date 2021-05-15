/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WightKingOnSteed.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 0;

    bool WightKingOnSteed::s_registered = false;

    Unit *WightKingOnSteed::Create(const ParameterList &parameters) {
        auto bloodline = (CursedBloodline) GetEnumParam("Bloodline", parameters, g_bloodlines[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WightKingOnSteed(bloodline, trait, artefact, general);
    }

    int WightKingOnSteed::ComputePoints(const ParameterList &parameters) {
        return g_pointsPerUnit;
    }

    void WightKingOnSteed::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Bloodline", g_bloodlines[0], g_bloodlines),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {SOULBLIGHT_GRAVELORDS}
            };
            s_registered = UnitFactory::Register("Wight King on Skeletal Steed", factoryMethod);
        }
    }

    WightKingOnSteed::WightKingOnSteed(CursedBloodline bloodline, CommandTrait trait, Artefact artefact, bool isGeneral) :
        SoulblightBase(bloodline, "Wight King on Skeletal Steed", 12, g_wounds, 10, 3, true, g_pointsPerUnit) {

        m_keywords = {DEATH, SOULBLIGHT_GRAVELORDS, DEATHRATTLE, HERO, WIGHT_KING, WIGHT_KING_ON_SKELETAL_STEED};
        m_weapons = {&m_lance, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_lance);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);
    }

    Wounds WightKingOnSteed::applyWoundSave(const Wounds &wounds, Unit *attackingUnitk) {
        return SoulblightBase::applyWoundSave(wounds, attackingUnitk);
    }

    void WightKingOnSteed::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);
    }
} // namespace Soulblight
