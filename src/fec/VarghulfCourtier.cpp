/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/VarghulfCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 160;

    bool VarghulfCourtier::s_registered = false;

    VarghulfCourtier::VarghulfCourtier(GrandCourt court, Delusion delusion, CommandTrait trait, Artefact artefact, bool isGeneral) :
            FleshEaterCourts(court, delusion, "Varghulf Courtier", 12, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_immenseClaws(Weapon::Type::Melee, "Immense Claws", 2, 4, 3, 3, -1, 2),
            m_daggerlikeFangs(Weapon::Type::Melee, "Dagger-like Fangs", 1, 1, 3, 2, -2, RAND_D3) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, COURTIER, HERO, VARGHULF_COURTIER};
        m_weapons = {&m_immenseClaws, &m_daggerlikeFangs};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto courtier = new Model(g_basesize, wounds());
        courtier->addMeleeWeapon(&m_immenseClaws);
        courtier->addMeleeWeapon(&m_daggerlikeFangs);
        addModel(courtier);
    }

    Unit *VarghulfCourtier::Create(const ParameterList &parameters) {
        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_courtierCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_courtierArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new VarghulfCourtier(court, delusion, trait, artefact, general);
    }

    void VarghulfCourtier::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    VarghulfCourtier::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    VarghulfCourtier::ComputePoints,
                    {
                            EnumParameter("Grand Court", g_grandCourt[0], g_grandCourt),
                            EnumParameter("Delusion", g_delusion[0], g_delusion),
                            EnumParameter("Command Trait", g_courtierCommandTraits[0], g_courtierCommandTraits),
                            EnumParameter("Artefact", g_courtierArtefacts[0], g_courtierArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Varghulf Courtier", factoryMethod);
        }
    }

    int VarghulfCourtier::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void VarghulfCourtier::onStartHero(PlayerId player) {
        FleshEaterCourts::onStartHero(player);

        // Muster Royal Household
        auto courtiers = Board::Instance()->getUnitsWithKeyword(owningPlayer(), VARGHULF_COURTIER);
        Dice::RollResult rolls;
        for (auto c = 0u; c < courtiers.size(); c++) {
            Dice::RollD6(6, rolls);
            auto serfs = Board::Instance()->getUnitsWithKeyword(owningPlayer(), SERFS);
            for (auto serf : serfs) {
                if (Board::Instance()->getUnitWithKeyword(serf, owningPlayer(), VARGHULF_COURTIER, 10.0)) {
                    serf->returnModels(rolls.rollsGE(2));
                }
            }
        }
    }

} // namespace FleshEaterCourt
