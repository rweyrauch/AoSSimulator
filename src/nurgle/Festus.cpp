/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include "nurgle/Festus.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool FestusTheLeechlord::s_registered = false;

    Unit *FestusTheLeechlord::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new FestusTheLeechlord(legion, lore, general);
    }

    void FestusTheLeechlord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FestusTheLeechlord::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    FestusTheLeechlord::ComputePoints,
                    {
                            EnumParameter("Lore", g_mortalRotbringerLore[0], g_mortalRotbringerLore),
                            BoolParameter("General"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),

                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Festus the Leechlord", factoryMethod);
        }
    }

    FestusTheLeechlord::FestusTheLeechlord(PlagueLegion legion, Lore lore, bool isGeneral) :
            NurgleBase(legion,"Festus the Leechlord", 4, g_wounds, 7, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Plague Staff", 1, 2, 4, 3, 0, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, WIZARD, FESTUS_THE_LEECHLORD};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);
    }

    int FestusTheLeechlord::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void FestusTheLeechlord::onStartHero(PlayerId player) {
        NurgleBase::onStartHero(player);
        // Healing Elixiers
        if (owningPlayer() == player) {
            heal(1);
        }

        // Delightful Brews, Splendid Restoratives
        bool usedBrews = false;
        auto friendly = Board::Instance()->getUnitsWithin(this, owningPlayer(), 1.0);
        for (auto unit : friendly) {
            if (unit->numOfWoundedModels() > 0) {
                if (Dice::RollD6() >= 2) {
                    unit->heal(Dice::RollD3());
                    usedBrews = true;
                    break;
                }
            }
        }
        if (!usedBrews) {
            auto enemy = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto unit : enemy) {
                if (unit->remainingModels() > 0) {
                    if (Dice::RollD6() >= 2) {
                        unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Ability}, this);
                        usedBrews = true;
                        break;
                    }
                }
            }
        }
    }

} // namespace Nurgle
