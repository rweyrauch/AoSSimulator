/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkPriest.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 25;
    static const int g_wounds = 4;
    static const int g_pointsPerUnit = 70;

    bool SkinkPriest::s_registered = false;

    SkinkPriest::SkinkPriest() :
            SeraphonBase("Skink Priest", 8, g_wounds, 6, 5, false),
            m_starbolt(Weapon::Type::Missile, "Starbolt", 18, 2, 4, 3, -1, 1),
            m_staff(Weapon::Type::Melee, "Star-stone Staff", 1, 1, 4, 4, -1, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, PRIEST};
        m_weapons = {&m_starbolt, &m_staff};
        m_battleFieldRole = Role::Leader;
    }

    bool SkinkPriest::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_starbolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SkinkPriest::Create(const ParameterList &parameters) {
        auto unit = new SkinkPriest();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_skinkCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_vestmentsOfThePriesthood[0]);

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

    void SkinkPriest::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_skinkCommandTrait[0], g_skinkCommandTrait),
                            EnumParameter("Artefact", g_vestmentsOfThePriesthood[0], g_vestmentsOfThePriesthood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Skink Priest", factoryMethod);
        }
    }

    int SkinkPriest::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
