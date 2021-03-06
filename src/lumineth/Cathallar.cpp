/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Cathallar.h"
#include "LuminethPrivate.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool ScinariCathallar::s_registered = false;

    Unit *ScinariCathallar::Create(const ParameterList &parameters) {
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfHysh[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraitsScinari[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefactsScinari[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ScinariCathallar(nation, lore, trait, artefact, general);
    }

    int ScinariCathallar::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void ScinariCathallar::Init() {
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

            s_registered = UnitFactory::Register("Scinari Cathallar", factoryMethod);
        }
    }

    ScinariCathallar::ScinariCathallar(GreatNation nation, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            LuminethBase(nation, "Scinari Cathallar", 6, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_touch(Weapon::Type::Melee, "Despairing Touch", 1, 1, 4, 2, 0, RAND_D3) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, SCINARI, HERO, WIZARD, CATHALLAR};
        m_weapons = {&m_touch};
        m_battleFieldRole = Role::Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_touch);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

} // namespace LuminethRealmLords