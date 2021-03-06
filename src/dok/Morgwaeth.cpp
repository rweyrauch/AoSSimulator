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
#include "DoKCommands.h"

namespace DaughtersOfKhaine {
    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 80;

    bool MorgwaethTheBloodied::s_registered = false;

    MorgwaethTheBloodied::MorgwaethTheBloodied(Prayer prayer, bool isGeneral) :
            DaughterOfKhaine(Temple::Hagg_Nar, "Morgwaeth the Bloodied", 6, g_wounds, 8, 5, false, g_pointsPerUnit),
            m_glaive(Weapon::Type::Melee, "Glaive of Khaine", 2, 3, 3, 3, -1, 1) {
        m_keywords = {ORDER, AELF, DAUGHTERS_OF_KHAINE, HERO, PRIEST, HAG_QUEEN, MORGWAETH_THE_BLOODIED};
        m_weapons = {&m_glaive};
        m_battleFieldRole = Role::Leader;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_glaive);
        addModel(model);

        configureCommon();
    }

    Unit *MorgwaethTheBloodied::Create(const ParameterList &parameters) {
        auto prayer = (Prayer) GetEnumParam("Prayer", parameters, g_prayers[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new MorgwaethTheBloodied(prayer, general);
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

    int MorgwaethTheBloodied::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void MorgwaethTheBloodied::onStartHero(PlayerId player) {
        DaughterOfKhaine::onStartHero(player);

        m_glaive.setDamage(1);

        // Priestess of Khaine
        auto roll = Dice::RollD6();
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && distanceTo(unit) <= 3.0) {
            // Touch of Death
            if (roll == 1) {
                applyDamage({0, 1}, this);
            } else if (roll >= 3) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        } else {
            // Rune of Khorne
            if (roll == 1) {
                applyDamage({0, 1}, this);
            } else if (roll >= 3) {
                m_glaive.setDamage(RAND_D3);
            }
        }

        // Witchbrew
        auto friendlies = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12);
        for (auto friendly : friendlies) {
            auto dok = dynamic_cast<DaughterOfKhaine *>(friendly);
            if (dok) {
                auto bloodRightAdj = std::min(3, getBloodRiteRound() - 1); // Bonus for Headlong Fury, Zealot's Rage and Slaughter's Strength
                roll = Dice::RollD6() + bloodRightAdj;
                if (roll >= 5) {
                    const Duration duration = {GamePhase::Hero, m_battleRound + 1, owningPlayer()};
                    dok->buffReroll(Attribute::To_Wound_Melee, Rerolls::Failed, duration);
                    dok->buffAbility(Ability::Ignore_Battleshock, 1, duration);
                }
            }
        }
    }

} //namespace DaughtersOfKhaine

