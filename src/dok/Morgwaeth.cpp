/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <dok/Morgwaeth.h>
#include <UnitFactory.h>
#include <Board.h>
#include "DaughterOfKhainePrivate.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool MorgwaethTheBloodied::s_registered = false;

    MorgwaethTheBloodied::MorgwaethTheBloodied() :
            DaughterOfKhaine("Morgwaeth the Bloodied", 6, g_wounds, 8, 5, false),
            m_glaive(Weapon::Type::Melee, "Glaive of Khaine", 2, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, HAG_QUEEN, MORGWAETH_THE_BLOODIED};
        m_weapons = {&m_glaive};
        m_battleFieldRole = Leader;
    }

    bool MorgwaethTheBloodied::configure(Prayer prayer) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_glaive);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *MorgwaethTheBloodied::Create(const ParameterList &parameters) {
        auto unit = new MorgwaethTheBloodied();

        unit->setTemple(Temple::Hagg_Nar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);

        bool ok = unit->configure(prayer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void MorgwaethTheBloodied::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    DaughterOfKhaine::ValueToString,
                    DaughterOfKhaine::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Prayer", g_prayers[0], g_prayers),
                            BoolParameter("General")
                    },
                    ORDER,
                    {DAUGHTERS_OF_KHAINE}
            };
            s_registered = UnitFactory::Register("Morgwaeth the Bloodied", factoryMethod);
        }
    }

    int MorgwaethTheBloodied::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MorgwaethTheBloodied::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        m_glaive.setDamage(1);

        // Priestess of Khaine
        const auto roll = Dice::RollD6();
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 3.0) {
            // Touch of Death
            if (roll == 1) {
                applyDamage({0, 1});
            } else if (roll >= 3) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3()});
                }
            }
        } else {
            // Rune of Khorne
            if (roll == 1) {
                applyDamage({0, 1});
            } else if (roll >= 3) {
                m_glaive.setDamage(RAND_D3);
            }
        }
    }

} //namespace DaughtersOfKhaine

