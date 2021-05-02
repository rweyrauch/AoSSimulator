/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptHaunterCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 50;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool CryptHaunterCourtier::s_registered = false;

    CryptHaunterCourtier::CryptHaunterCourtier() :
            FleshEaterCourts("Crypt Haunter Courtier", 12, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_massiveBoneClub(Weapon::Type::Melee, "Massive Bone Clube", 1, 3, 4, 3, 0, 3),
            m_rancidTalons(Weapon::Type::Melee, "Rancid Talons", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_HAUNTER_COURTIER};
        m_weapons = {&m_massiveBoneClub, &m_rancidTalons};
        m_battleFieldRole = Role::Leader;
    }

    void CryptHaunterCourtier::configure() {
        auto infernal = new Model(g_basesize, wounds());
        infernal->addMeleeWeapon(&m_massiveBoneClub);
        infernal->addMeleeWeapon(&m_rancidTalons);
        addModel(infernal);

        m_points = g_pointsPerUnit;
    }

    Unit *CryptHaunterCourtier::Create(const ParameterList &parameters) {
        auto unit = new CryptHaunterCourtier();

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

        unit->configure();
        return unit;
    }

    void CryptHaunterCourtier::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CryptHaunterCourtier::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    CryptHaunterCourtier::ComputePoints,
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
            s_registered = UnitFactory::Register("Crypt Haunter Courtier", factoryMethod);
        }
    }

    void CryptHaunterCourtier::onStartHero(PlayerId player) {
        FleshEaterCourts::onStartHero(player);

        // Noble Blood
        if (owningPlayer() == player) heal(1);

        // Muster King's Chosen
        auto courtiers = Board::Instance()->getUnitsWithKeyword(owningPlayer(), CRYPT_HAUNTER_COURTIER);
        Dice::RollResult rolls;
        for (auto c = 0u; c < courtiers.size(); c++) {
            Dice::RollD6(6, rolls);
            auto horrors = Board::Instance()->getUnitsWithKeyword(owningPlayer(), CRYPT_HORRORS);
            for (auto horror : horrors) {
                if (Board::Instance()->getUnitWithKeyword(horror, owningPlayer(), CRYPT_HAUNTER_COURTIER, 10.0)) {
                    horror->returnModels(rolls.rollsGE(5));
                }
            }
        }
    }

    Rerolls CryptHaunterCourtier::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Chosen of the King
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
        for (auto unit : units) {
            if (unit->hasKeyword(ABHORRANT)) return Rerolls::Failed;
        }
        return FleshEaterCourts::toHitRerolls(weapon, target);
    }

    int CryptHaunterCourtier::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace FleshEaterCourt
