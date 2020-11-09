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

    CryptGhastCourtier::CryptGhastCourtier() :
            FleshEaterCourts("Crypt Ghast Courtier", 6, g_wounds, 10, 5, false),
            m_boneClub(Weapon::Type::Melee, "Bone Club", 1, 3, 3, 3, 0, 1),
            m_filthyClaws(Weapon::Type::Melee, "Filthy Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_GHAST_COURTIER};
        m_weapons = {&m_boneClub, &m_filthyClaws};
        m_battleFieldRole = Leader;
    }

    bool CryptGhastCourtier::configure() {
        auto infernal = new Model(g_basesize, wounds());
        infernal->addMeleeWeapon(&m_boneClub);
        infernal->addMeleeWeapon(&m_filthyClaws);
        addModel(infernal);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *CryptGhastCourtier::Create(const ParameterList &parameters) {
        auto unit = new CryptGhastCourtier();

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, g_grandCourt[0]);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, g_delusion[0]);
        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == GrandCourt::None)
            unit->setCourtsOfDelusion(delusion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_courtierCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_courtierArtefacts[0]);
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

    int CryptGhastCourtier::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace FleshEaterCourt
