/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <Board.h>
#include <UnitFactory.h>
#include "nighthaunt/DreadbladeHarrow.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool DreadbladeHarrow::s_registered = false;

    Unit *DreadbladeHarrow::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new DreadbladeHarrow(procession, trait, artefact, general);
    }

    void DreadbladeHarrow::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DreadbladeHarrow::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    DreadbladeHarrow::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Dreadblade Harrow", factoryMethod);
        }
    }

    DreadbladeHarrow::DreadbladeHarrow(Procession procession, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt(procession, "Dreadblade Harrow", 12, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_dreadblade(Weapon::Type::Melee, "Dreadblade", 1, 3, 3, 3, -1, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 2, 4, 5, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, DREADBLADE_HARROW};
        m_weapons = {&m_dreadblade, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_dreadblade);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);
    }

    int DreadbladeHarrow::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Dreadblade
        auto attacks = Nighthaunt::extraAttacks(attackingModel, weapon, target);
        if (!m_charged && (weapon->name() == m_dreadblade.name())) attacks++;

        return attacks;
    }

    Wounds DreadbladeHarrow::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Dreadblade
        auto wounds = Nighthaunt::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        if (m_charged && (weapon->name() == m_dreadblade.name())) wounds.normal++;

        return wounds;
    }

    Rerolls DreadbladeHarrow::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), KNIGHT_OF_SHROUDS, 9.0);
        if (unit && (weapon->name() == m_dreadblade.name())) {
            return Rerolls::Ones;
        }
        return Nighthaunt::toWoundRerolls(weapon, target);
    }

    int DreadbladeHarrow::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }
} // namespace Nighthaunt
