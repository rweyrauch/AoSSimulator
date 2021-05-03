/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/Windmage.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool HurakanWindmage::s_registered = false;

    Unit *HurakanWindmage::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraitsScinari[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsScinari[0]);
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfHysh[0]);
        return new HurakanWindmage(nation, lore, trait, artefact, general);
    }

    int HurakanWindmage::ComputePoints(const ParameterList& parameters) {
        return g_pointsPerUnit;
    }

    void HurakanWindmage::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Lore", g_loreOfWinds[0], g_loreOfWinds),
                            EnumParameter("Command Trait", g_commandTraitsHurakan[0], g_commandTraitsHurakan),
                            EnumParameter("Artefact", g_artefactsHurakan[0], g_artefactsHurakan),
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };
            s_registered = UnitFactory::Register("Hurakan Windmage", factoryMethod);
        }
    }

    HurakanWindmage::HurakanWindmage(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            LuminethBase(nation, "Hurakan Windmage", 16, g_wounds, 7, 5, true, g_pointsPerUnit) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, AELEMENTIRI, HURAKAN, HERO, WIZARD, WINDMAGE};
        m_weapons = {&m_aspiragillum};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_aspiragillum);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }
}