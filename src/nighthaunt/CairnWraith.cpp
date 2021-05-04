/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/CairnWraith.h>
#include <UnitFactory.h>
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 25;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 60;

    bool CairnWraith::s_registered = false;

    Unit *CairnWraith::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new CairnWraith(procession, trait, artefact, general);
    }

    int CairnWraith::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void CairnWraith::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Cairn Wraith", factoryMethod);
        }
    }

    CairnWraith::CairnWraith(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt(procession, "Cairn Wraith", 6, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_scythe(Weapon::Type::Melee, "Reaper Scythe", 2, 3, 4, 3, -1, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, CAIRN_WRAITH};
        m_weapons = {&m_scythe};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_scythe);
        addModel(model);
    }

    Wounds CairnWraith::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll >= 6) && (weapon->name() == m_scythe.name())) return {0, 2};
        return Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls CairnWraith::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Reaped Like Corn
        if (weapon->name() == m_scythe.name() && unit->remainingModels() >= 5) {
            return Rerolls::Failed;
        }
        return Nighthaunt::toHitRerolls(weapon, unit);
    }

} // namespace Nighthaunt
