/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <soulblight/WightKingBlackAxe.h>
#include <UnitFactory.h>
#include "SoulblightGravelordsPrivate.h"

namespace Soulblight {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 115;

    bool WightKingWithBlackAxe::s_registered = false;

    WightKingWithBlackAxe::WightKingWithBlackAxe(Legion legion, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SoulblightBase(legion, "Wight King with Black Axe", 4, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_blackAxe(Weapon::Type::Melee, "Black Axe", 1, 4, 3, 3, -1, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
        m_weapons = {&m_blackAxe};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blackAxe);
        addModel(model);
    }

    Unit *WightKingWithBlackAxe::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new WightKingWithBlackAxe(legion, trait, artefact, general);
    }

    void WightKingWithBlackAxe::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SoulblightBase::ValueToString,
                    SoulblightBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legions[0], g_legions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {DEATHRATTLE}
            };
            s_registered = UnitFactory::Register("Wight King with Black Axe", factoryMethod);
        }
    }

    Wounds WightKingWithBlackAxe::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Barrow Armour
        return {(wounds.normal + 1) / 2, wounds.mortal};
    }

    int WightKingWithBlackAxe::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void WightKingWithBlackAxe::onStartHero(PlayerId player) {
        SoulblightBase::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Soulblight
