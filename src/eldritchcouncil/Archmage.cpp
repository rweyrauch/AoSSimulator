/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <eldritchcouncil/Archmage.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace EldritchCouncil {
    static const int g_basesize = 25;
    static const int g_basesizeSteed = 60; // x35 oval
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Archmage::s_registered = false;

    Archmage::Archmage(bool steed, bool isGeneral) :
            Unit("Archmage", 6, g_wounds, 7, 6, false, g_pointsPerUnit),
            m_seerstaff(Weapon::Type::Melee, "Seerstaff", 2, 1, 4, 3, -1, 1),
            m_steedHooves(Weapon::Type::Melee, "Aelven Steed's Swift Hooves", 1, 2, 4, 5, 0, 1) {
        m_keywords = {ORDER, AELF, ELDRITCH_COUNCIL, HERO, WIZARD, ARCHMAGE};
        m_weapons = {&m_seerstaff, &m_steedHooves};

        setGeneral(isGeneral);

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        m_steed = steed;

        auto model = new Model(steed ? g_basesizeSteed : g_basesize, wounds());
        model->addMeleeWeapon(&m_seerstaff);
        if (m_steed) {
            model->addMeleeWeapon(&m_steedHooves);
            m_move = 14;
        }
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Unit *Archmage::Create(const ParameterList &parameters) {
        bool steed = GetBoolParam("Steed", parameters, false);
        auto general = GetBoolParam("General", parameters, false);
        return new Archmage(steed, general);
    }

    void Archmage::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    nullptr,
                    nullptr,
                    ComputePoints,
                    {
                            BoolParameter("Steed"),
                            BoolParameter("General")
                    },
                    ORDER,
                    {ELDRITCH_COUNCIL}
            };
            s_registered = UnitFactory::Register("Archmage", factoryMethod);
        }
    }

    int Archmage::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace EldritchCouncil