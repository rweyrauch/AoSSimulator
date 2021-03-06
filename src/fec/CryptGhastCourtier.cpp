/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptGhastCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 32;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 60;

    bool CryptGhastCourtier::s_registered = false;

    CryptGhastCourtier::CryptGhastCourtier(GrandCourt court, Delusion delusion, CommandTrait trait, Artefact artefact, bool isGeneral) :
            FleshEaterCourts(court, delusion, "Crypt Ghast Courtier", 6, g_wounds, 10, 5, false, g_pointsPerUnit),
            m_boneClub(Weapon::Type::Melee, "Bone Club", 1, 3, 3, 3, 0, 1),
            m_filthyClaws(Weapon::Type::Melee, "Filthy Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_GHAST_COURTIER};
        m_weapons = {&m_boneClub, &m_filthyClaws};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto infernal = new Model(g_basesize, wounds());
        infernal->addMeleeWeapon(&m_boneClub);
        infernal->addMeleeWeapon(&m_filthyClaws);
        addModel(infernal);
    }

    Unit *CryptGhastCourtier::Create(const ParameterList &parameters) {
        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_courtierCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_courtierArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new CryptGhastCourtier(court, delusion, trait, artefact, general);
    }

    void CryptGhastCourtier::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    ComputePoints,
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
            s_registered = UnitFactory::Register("Crypt Ghast Courtier", factoryMethod);
        }
    }

    int CryptGhastCourtier::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void CryptGhastCourtier::onStartHero(PlayerId playerId) {
        FleshEaterCourts::onStartHero(playerId);

        // Muster Serf
        auto courtiers = Board::Instance()->getUnitsWithKeyword(owningPlayer(), CRYPT_GHAST_COURTIER);
        Dice::RollResult rolls;
        for (auto c = 0u; c < courtiers.size(); c++) {
            Dice::RollD6(6, rolls);
            auto serfs = Board::Instance()->getUnitsWithKeyword(owningPlayer(), SERFS);
            for (auto serf : serfs) {
                if (Board::Instance()->getUnitWithKeyword(serf, owningPlayer(), CRYPT_GHAST_COURTIER, 10.0)) {
                    serf->returnModels(rolls.rollsGE(2));
                }
            }
        }
    }

} // namespace FleshEaterCourt
