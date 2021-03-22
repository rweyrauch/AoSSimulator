/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusSunblood.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 32;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 130;

    bool SaurusSunblood::s_registered = false;

    SaurusSunblood::SaurusSunblood() :
            SeraphonBase("Saurus Sunblood", 5, g_wounds, 8, 3, false),
            m_celestiteWarmace(Weapon::Type::Melee, "Celestite Warmace", 1, 6, 3, 3, -1, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SAURUS, HERO, SUNBLOOD};
        m_weapons = {&m_celestiteWarmace, &m_jaws};
        m_battleFieldRole = Role::Leader;
    }

    bool SaurusSunblood::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_celestiteWarmace);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SaurusSunblood::Create(const ParameterList &parameters) {
        auto unit = new SaurusSunblood();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SaurusSunblood::Init() {
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
                            EnumParameter("Artefact", g_celestialRelicsOfTheWarrior[0], g_celestialRelicsOfTheWarrior),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Saurus Sunblood", factoryMethod);
        }
    }

    int SaurusSunblood::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Primal Rage
        if (unmodifiedHitRoll == 6) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusSunblood::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
