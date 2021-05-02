/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusAstrolithBearer.h>
#include <UnitFactory.h>
#include "SeraphonPrivate.h"

namespace Seraphon {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool SaurusAstrolithBearer::s_registered = false;

    SaurusAstrolithBearer::SaurusAstrolithBearer(WayOfTheSeraphon way, Constellation constellation, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SeraphonBase("Saurus Astrolith Bearer", 5, g_wounds, 8, 4, false, g_pointsPerUnit),
            m_warpick(Weapon::Type::Melee, "Celestite Warpick", 1, 3, 3, 3, -1, 1),
            m_jaws(Weapon::Type::Melee, "Fearsome Jaws", 1, 1, 4, 3, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SAURUS, HERO, TOTEM, ASTROLITH_BEARER};
        m_weapons = {&m_warpick, &m_jaws};
        m_battleFieldRole = Role::Leader;

        setWayOfTheSeraphon(way, constellation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_warpick);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);
    }

    Unit *SaurusAstrolithBearer::Create(const ParameterList &parameters) {
        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SaurusAstrolithBearer(way, constellation, trait, artefact, general);
    }

    void SaurusAstrolithBearer::Init() {
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

            s_registered = UnitFactory::Register("Saurus Astrolith Bearer", factoryMethod);
        }
    }

    int SaurusAstrolithBearer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
