/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/Branchwych.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>
#include <spells/SylvanethSpells.h>

namespace Sylvaneth {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool Branchwych::s_registered = false;

    Branchwych::Branchwych() :
            SylvanethBase("Branchwych", 7, WOUNDS, 7, 5, false),
            m_greenwoodScythe(Weapon::Type::Melee, "Greenwood Scythe", 2, 2, 4, 3, 0, 2),
            m_bittergrubsMandibles(Weapon::Type::Melee, "Snapping Mandibles", 1, 1, 4, 4, -1, 1) {
        m_keywords = {ORDER, SYLVANETH, NOBLE_SPIRITS, HERO, WIZARD, BRANCHWYCH};
        m_weapons = {&m_greenwoodScythe, &m_bittergrubsMandibles};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Branchwych::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_greenwoodScythe);
        model->addMeleeWeapon(&m_bittergrubsMandibles);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateUnleashSpites(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Branchwych::Create(const ParameterList &parameters) {
        auto unit = new Branchwych();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
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
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Branchwych", factoryMethod);
        }
    }

    int Branchwych::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);
        // Quick-tempered
        if ((weapon->name() == m_greenwoodScythe.name()) && (attackingModel->woundsRemaining() < wounds())) {
            extra += 2;
        }
        return extra;
    }

    int Branchwych::toHitModifier(const Weapon *weapon, const Unit *target) const {
        int modifier = Unit::toHitModifier(weapon, target);

        // Fury of the Forest
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 6.0);
        if (unit != nullptr) {
            modifier += 1;
        }

        return modifier;
    }

    int Branchwych::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Sylvaneth
