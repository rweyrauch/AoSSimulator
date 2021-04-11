/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "slavestodarkness/MindstealerSphiranx.h"
#include "SlavesToDarknessPrivate.h"
#include "Board.h"

namespace SlavesToDarkness {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 100;

    bool MindstealerSphiranx::s_registered = false;

    Unit *MindstealerSphiranx::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new MindstealerSphiranx(legion);
    }

    void MindstealerSphiranx::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Mindstealer Sphiranx", factoryMethod);
        }
    }

    MindstealerSphiranx::MindstealerSphiranx(DamnedLegion legion) :
            SlavesToDarknessBase("Mindstealer Sphiranx", 10, g_wounds, 10, 5, false) {
        m_keywords = {CHAOS, MORTAL, MONSTER, SLAVES_TO_DARKNESS, MINDSTEALER_SPHIRANX};

        s_globalBraveryMod.connect(this, &MindstealerSphiranx::telepathicDread, &m_connection);

        setDamnedLegion(legion);

        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_claws);
        model->addMeleeWeapon(&m_tail);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    MindstealerSphiranx::~MindstealerSphiranx() {
        m_connection.disconnect();
    }

    int MindstealerSphiranx::telepathicDread(const Unit *unit) {
        // Telepathic Dread
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return -2;
        }

        return 0;
    }

    int MindstealerSphiranx::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void MindstealerSphiranx::onStartHero(PlayerId player) {
        SlavesToDarknessBase::onStartHero(player);

        // Dominate Mind
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 12.0);
            for (auto unit : units) {
                if (unit->remainingModels() > 0) {
                    auto rollThis = Dice::RollD6();
                    auto rollEnemy = Dice::RollD6();
                    if (rollThis != rollEnemy) {
                        unit->buffAbility(Ability::Fights_Last, 1, {Phase::Hero, m_battleRound+1, owningPlayer()});
                        break;
                    }
                }
            }
        }
    }

}//namespace SlavesToDarkness
