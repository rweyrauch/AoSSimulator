/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusVeteranColdOne.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 110;

    bool SaurusScarVeteranOnColdOne::s_registered = false;

    SaurusScarVeteranOnColdOne::SaurusScarVeteranOnColdOne() :
            SeraphonBase("Saurus Scar-Veteran on Cold One", 8, WOUNDS, 8, 4, false),
            m_warpick(Weapon::Type::Melee, "Celestite Warpick", 1, 3, 3, 3, -1, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1),
            m_coldOneJaws(Weapon::Type::Melee, "Snapping Jaws", 1, 2, 3, 4, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SAURUS, HERO, COLD_ONE, SCAR_VETERAN};
        m_weapons = {&m_warpick, &m_jaws, &m_coldOneJaws};
        m_hasMount = true;
    }

    bool SaurusScarVeteranOnColdOne::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_warpick);
        model->addMeleeWeapon(&m_jaws);
        model->addMeleeWeapon(&m_coldOneJaws);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SaurusScarVeteranOnColdOne::Create(const ParameterList &parameters) {
        auto unit = new SaurusScarVeteranOnColdOne();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SaurusScarVeteranOnColdOne::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_saurusCommandTrait[0], g_saurusCommandTrait),
                            EnumParameter("Artefact", g_celestialRelicsOfTheWarrior[0], g_celestialRelicsOfTheWarrior)
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Scar-Veteran on Cold One", factoryMethod);
        }
    }

    int SaurusScarVeteranOnColdOne::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_warpick.name())) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusScarVeteranOnColdOne::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
