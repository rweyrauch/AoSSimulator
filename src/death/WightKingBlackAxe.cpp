/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/WightKingBlackAxe.h>
#include <UnitFactory.h>
#include "LegionOfNagashPrivate.h"

namespace Death {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool WightKingWithBlackAxe::s_registered = false;

    WightKingWithBlackAxe::WightKingWithBlackAxe() :
            LegionOfNagashBase("Wight King with Black Axe", 4, g_wounds, 10, 4, false),
            m_blackAxe(Weapon::Type::Melee, "Black Axe", 1, 4, 3, 3, -1, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
        m_weapons = {&m_blackAxe};
        m_battleFieldRole = Role::Leader;
    }

    void WightKingWithBlackAxe::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blackAxe);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *WightKingWithBlackAxe::Create(const ParameterList &parameters) {
        auto unit = new WightKingWithBlackAxe();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legions[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    void WightKingWithBlackAxe::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
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

    int WightKingWithBlackAxe::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void WightKingWithBlackAxe::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Death
