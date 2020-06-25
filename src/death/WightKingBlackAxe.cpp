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
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool WightKingWithBlackAxe::s_registered = false;

    WightKingWithBlackAxe::WightKingWithBlackAxe() :
            LegionOfNagashBase("Wight King with Black Axe", 4, WOUNDS, 10, 4, false),
            m_blackAxe(Weapon::Type::Melee, "Black Axe", 1, 4, 3, 3, -1, 1) {
        m_keywords = {DEATH, SKELETON, DEATHRATTLE, HERO, WIGHT_KING};
        m_weapons = {&m_blackAxe};
    }

    bool WightKingWithBlackAxe::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blackAxe);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
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

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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

    Wounds WightKingWithBlackAxe::applyWoundSave(const Wounds &wounds) {
        // Barrow Armour
        return {(wounds.normal + 1) / 2, wounds.mortal};
    }

    int WightKingWithBlackAxe::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void WightKingWithBlackAxe::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) deathlyInvocations(2, 6.0);
    }

} // namespace Death
