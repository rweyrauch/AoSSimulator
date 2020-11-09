/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <fec/CryptInfernalCourtier.h>
#include <UnitFactory.h>
#include <Board.h>
#include <iostream>
#include "FleshEaterCourtsPrivate.h"

namespace FleshEaterCourt {
    static const int g_basesize = 50;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 120;

    bool CryptInfernalCourtier::s_registered = false;

    CryptInfernalCourtier::CryptInfernalCourtier() :
            FleshEaterCourts("Crypt Infernal Courtier", 12, g_wounds, 10, 4, true),
            m_foetidBreath(Weapon::Type::Missile, "Foetid Breath", 9, 1, 4, 3, -1, RAND_D3),
            m_skeweringTalons(Weapon::Type::Melee, "Skewering Talons", 1, 5, 4, 3, -1, 2) {
        m_keywords = {DEATH, MORDANT, FLESH_EATER_COURTS, HERO, COURTIER, CRYPT_INFERNAL_COURTIER};
        m_weapons = {&m_foetidBreath, &m_skeweringTalons};
        m_battleFieldRole = Leader;
    }

    bool CryptInfernalCourtier::configure() {
        auto infernal = new Model(g_basesize, wounds());
        infernal->addMissileWeapon(&m_foetidBreath);
        infernal->addMeleeWeapon(&m_skeweringTalons);
        addModel(infernal);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *CryptInfernalCourtier::Create(const ParameterList &parameters) {
        auto unit = new CryptInfernalCourtier();

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

    void CryptInfernalCourtier::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CryptInfernalCourtier::Create,
                    FleshEaterCourts::ValueToString,
                    FleshEaterCourts::EnumStringToInt,
                    CryptInfernalCourtier::ComputePoints,
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
            s_registered = UnitFactory::Register("Crypt Infernal Courtier", factoryMethod);
        }
    }

    Wounds
    CryptInfernalCourtier::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Skewering Strike
        if ((hitRoll == 6) && weapon->name() == m_skeweringTalons.name()) {
            // Two mortal wounds.
            return {0, 2};
        }
        return FleshEaterCourts::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int CryptInfernalCourtier::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace FleshEaterCourt
