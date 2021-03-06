/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <greenskinz/OrrukGreatShaman.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Greenskinz {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 120;

    bool OrrukGreatShaman::s_registered = false;

    OrrukGreatShaman::OrrukGreatShaman(bool warboar, bool isGeneral) :
            Unit("Orruk Great Shaman", 5, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_totemicStaff(Weapon::Type::Melee, "Totemic Staff", 2, 1, 4, 3, 0, RAND_D3),
            m_boarsTusks(Weapon::Type::Melee, "War Boar's Tusks", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, GREENSKINZ, HERO, WIZARD, ORRUK_GREAT_SHAMAN};
        m_weapons = {&m_totemicStaff, &m_boarsTusks};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_totemicStaff);
        if (warboar) {
            model->addMeleeWeapon(&m_boarsTusks);
            m_move = 9;
        }
        addModel(model);
    }

    Unit *OrrukGreatShaman::Create(const ParameterList &parameters) {
        bool warboar = GetBoolParam("War Boar", parameters, false);
        auto general = GetBoolParam("General", parameters, false);
        return new OrrukGreatShaman(warboar, general);
    }

    void OrrukGreatShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukGreatShaman::Create,
                    nullptr,
                    nullptr,
                    OrrukGreatShaman::ComputePoints,
                    {
                            BoolParameter("War Boar"),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GREENSKINZ}
            };
            s_registered = UnitFactory::Register("Orruk Great Shaman", factoryMethod);
        }
    }

    int OrrukGreatShaman::castingModifier() const {
        auto modifier = Unit::castingModifier();

        // Waaagh! Energy
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 8.0);
        int totalOrruks = 0;
        for (auto ip : units) {
            if (ip->hasKeyword(ORRUK)) {
                totalOrruks = ip->remainingModels();
            }
        }
        if (totalOrruks >= 20) {
            modifier += 1;
        }
        return modifier;
    }

    int OrrukGreatShaman::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Greenskinz
