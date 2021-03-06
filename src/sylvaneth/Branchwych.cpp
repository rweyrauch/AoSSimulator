/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <sylvaneth/Branchwych.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>
#include <sylvaneth/SylvanethSpells.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool Branchwych::s_registered = false;

    Branchwych::Branchwych(Glade glade, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            SylvanethBase("Branchwych", 7, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_greenwoodScythe(Weapon::Type::Melee, "Greenwood Scythe", 2, 2, 4, 3, 0, 2),
            m_bittergrubsMandibles(Weapon::Type::Melee, "Snapping Mandibles", 1, 1, 4, 4, -1, 1) {
        m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, HERO, WIZARD, BRANCHWYCH};
        m_weapons = {&m_greenwoodScythe, &m_bittergrubsMandibles};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setGlade(glade);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_greenwoodScythe);
        model->addMeleeWeapon(&m_bittergrubsMandibles);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateUnleashSpites(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *Branchwych::Create(const ParameterList &parameters) {
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDeepwood[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_relicsOfNature[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_aspectsOfRenewal[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new Branchwych(glade, lore, trait, artefact, general);
    }

    void Branchwych::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Branchwych::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    Branchwych::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Lore", g_loreOfTheDeepwood[0], g_loreOfTheDeepwood),
                            EnumParameter("Artefact", g_relicsOfNature[0], g_relicsOfNature),
                            EnumParameter("Command Trait", g_aspectsOfRenewal[0], g_aspectsOfRenewal),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Branchwych", factoryMethod);
        }
    }

    int Branchwych::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = SylvanethBase::extraAttacks(attackingModel, weapon, target);
        // Quick-tempered
        if ((weapon->name() == m_greenwoodScythe.name()) && (attackingModel->woundsRemaining() < wounds())) {
            extra += 2;
        }
        return extra;
    }

    int Branchwych::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = SylvanethBase::toHitModifier(weapon, target);

        // Fury of the Forest
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 6.0);
        if (unit != nullptr) {
            modifier += 1;
        }

        return modifier;
    }

    int Branchwych::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Sylvaneth
