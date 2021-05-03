/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Loremaster.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace EldritchCouncil {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool Loremaster::s_registered = false;

    Loremaster::Loremaster(bool isGeneral) :
            Unit("Loremaster", 6, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_greatsword(Weapon::Type::Melee, "Greatsword", 1, 2, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, HERO, WIZARD, LOREMASTER};
        m_weapons = {&m_greatsword};

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_greatsword);
        addModel(model);

        m_points = g_pointsPerUnit;

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *Loremaster::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new Loremaster(general);
    }

    void Loremaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {ELDRITCH_COUNCIL}
            };
            s_registered = UnitFactory::Register("Loremaster", factoryMethod);
        }
    }

    Rerolls Loremaster::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Deflect Shots
        if (weapon->isMissile()) {
            return Rerolls::Failed;
        }
        return Unit::toSaveRerolls(weapon, attacker);
    }

    int Loremaster::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace EldritchCouncil