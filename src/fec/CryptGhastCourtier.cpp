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

namespace FleshEaterCourt {
    static const int BASESIZE = 32;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 60;

    bool CryptGhastCourtier::s_registered = false;

    CryptGhastCourtier::CryptGhastCourtier() :
            FleshEaterCourts("Crypt Ghast Courtier", 6, WOUNDS, 10, 5, false),
            m_boneClub(Weapon::Type::Melee, "Bone Club", 1, 3, 3, 3, 0, 1),
            m_filthyClaws(Weapon::Type::Melee, "Filthy Claws", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_GHAST_COURTIER};
        m_weapons = {&m_boneClub, &m_filthyClaws};
    }

    bool CryptGhastCourtier::configure() {
        auto infernal = new Model(BASESIZE, wounds());
        infernal->addMeleeWeapon(&m_boneClub);
        infernal->addMeleeWeapon(&m_filthyClaws);
        addModel(infernal);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *CryptGhastCourtier::Create(const ParameterList &parameters) {
        auto unit = new CryptGhastCourtier();

        auto court = (GrandCourt) GetEnumParam("Grand Court", parameters, NoCourt);
        auto delusion = (Delusion) GetEnumParam("Delusion", parameters, None);
        // Can only select delusion if GrandCourt is NoCourt.
        unit->setGrandCourt(court);
        if (court == NoCourt)
            unit->setCourtsOfDelusion(delusion);

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
                            {ParamType::Enum, "Grand Court", FleshEaterCourts::NoCourt, FleshEaterCourts::NoCourt,
                             FleshEaterCourts::Gristlegore, 1},
                            {ParamType::Enum, "Delusion", FleshEaterCourts::None, FleshEaterCourts::None,
                             FleshEaterCourts::DefendersOfTheRealm, 1},
                    },
                    DEATH,
                    {FLESH_EATER_COURTS}
            };
            s_registered = UnitFactory::Register("Crypt Ghast Courtier", factoryMethod);
        }
    }

    int CryptGhastCourtier::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace FleshEaterCourt
