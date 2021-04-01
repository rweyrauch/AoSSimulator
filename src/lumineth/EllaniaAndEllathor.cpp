/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/EllaniaAndEllathor.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 40;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 260;

    bool EllaniaAndEllathor::s_registered = false;

    Unit *EllaniaAndEllathor::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfHysh[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new EllaniaAndEllathor(lore, general);
    }

    int EllaniaAndEllathor::ComputePoints(int numModels) {
        return g_pointsPerUnit;
    }

    void EllaniaAndEllathor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General"),
                            EnumParameter("Lore", g_loreOfHysh[0], g_loreOfHysh),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };
            s_registered = UnitFactory::Register("Ellania and Ellathor", factoryMethod);
        }
    }

    EllaniaAndEllathor::EllaniaAndEllathor(Lore lore, bool isGeneral) :
            LuminethBase("Ellania and Ellathor", 6, g_wounds, 8, 3, false) {
        m_keywords = {ORDER, AELF, LUMINETH_REALM_LORDS, SCINARI, VANARI, YMETRICA, HERO, WIZARD, ELLANIA_AND_ELLATHOR};
        m_weapons = {&m_altairi, &m_dianaer, &m_talons};
        m_battleFieldRole = Role::Leader;

        setNation(GreatNation::Ymetrica);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_altairi);
        model->addMeleeWeapon(&m_dianaer);
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);
    }
}