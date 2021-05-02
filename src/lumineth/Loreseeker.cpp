/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Loreseeker.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 160;

    bool ScinariLoreseeker::s_registered = false;

    Unit *ScinariLoreseeker::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraitsScinari[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsScinari[0]);
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfHysh[0]);
        return new ScinariLoreseeker(nation, lore, trait, artefact, general);
    }

    int ScinariLoreseeker::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ScinariLoreseeker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Lore", g_loreOfHysh[0], g_loreOfHysh),
                            EnumParameter("Command Trait", g_commandTraitsScinari[0], g_commandTraitsScinari),
                            EnumParameter("Artefact", g_artefactsScinari[0], g_artefactsScinari),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Scinari Loreseeker", factoryMethod);
        }
    }

    ScinariLoreseeker::ScinariLoreseeker(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            LuminethBase("Scinari Loreseeker", 6, g_wounds, 8, 4, false, g_pointsPerUnit) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, SCINARI, HERO, WIZARD, LORESEEKER};
        m_weapons = {&m_staff, &m_blade};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setNation(nation);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }
}