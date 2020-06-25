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
    static const int BASESIZE = 50;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool CryptHaunterCourtier::s_registered = false;

    CryptHaunterCourtier::CryptHaunterCourtier() :
            FleshEaterCourts("Crypt Haunter Courtier", 12, WOUNDS, 10, 4, true),
            m_massiveBoneClub(Weapon::Type::Melee, "Massive Bone Clube", 1, 3, 4, 3, 0, 3),
            m_rancidTalons(Weapon::Type::Melee, "Rancid Talons", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_HAUNTER_COURTIER};
        m_weapons = {&m_massiveBoneClub, &m_rancidTalons};
    }

    bool CryptHaunterCourtier::configure() {
        auto infernal = new Model(BASESIZE, wounds());
        infernal->addMeleeWeapon(&m_massiveBoneClub);
        infernal->addMeleeWeapon(&m_rancidTalons);
        addModel(infernal);

        m_points = POINTS_PER_UNIT;

        return true;
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

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
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
        Unit::onStartHero(player);

        // Noble Blood
        if (owningPlayer() == player) heal(1);
    }

    Rerolls CryptHaunterCourtier::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Chosen of the King
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 18.0);
        for (auto unit : units) {
            if (unit->hasKeyword(ABHORRANT)) return RerollFailed;
        }
        return FleshEaterCourts::toHitRerolls(weapon, target);
    }

    int CryptHaunterCourtier::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace FleshEaterCourt
