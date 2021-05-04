/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/PusgoyleBlightlords.h>
#include <UnitFactory.h>
#include <Board.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 60;
    static const int g_wounds = 7;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 190;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 6;

    bool PusgoyleBlightlords::s_registered = false;

    PusgoyleBlightlords::PusgoyleBlightlords(PlagueLegion legion, int numModels, int numTocsins, int points) :
            NurgleBase(legion,"Pusgoyle Blightlords", 8, g_wounds, 10, 4, true, points),
            m_blightedWeapon(Weapon::Type::Melee, "Blighted Weapon", 1, 3, 3, 3, 0, 1),
            m_dolorousTocsin(Weapon::Type::Melee, "Dolorous Tocsin", 1, 1, 4, 3, -2, 2),
            m_mouthparts(Weapon::Type::Melee, "Foul Mouthparts", 1, 2, 3, 3, 0, 1),
            m_venemousSting(Weapon::Type::Melee, "Venomous String", 1, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, PUSGOYLE_BLIGHTLORDS};
        m_weapons = {&m_blightedWeapon, &m_dolorousTocsin, &m_mouthparts, &m_venemousSting};
        m_hasMount = true;
        m_mouthparts.setMount(true);
        m_venemousSting.setMount(true);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_blightedWeapon);
            model->addMeleeWeapon(&m_mouthparts);
            model->addMeleeWeapon(&m_venemousSting);
            if (i < numTocsins) {
                model->addMeleeWeapon(&m_dolorousTocsin);
            }
            addModel(model);
        }
    }

    Unit *PusgoyleBlightlords::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numTocsins = GetIntParam("Dolorous Tocsin", parameters, 0);
        return new PusgoyleBlightlords(legion, numModels, numTocsins, ComputePoints(parameters));
    }

    void PusgoyleBlightlords::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    PusgoyleBlightlords::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    PusgoyleBlightlords::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Dolorous Tocsin", 0, 0, g_maxUnitSize / 2, 1),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Pusgoyle Blightlords", factoryMethod);
        }
    }

    Wounds PusgoyleBlightlords::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Disgustingly Resilient
        return ignoreWounds(wounds, 5);
    }

    int PusgoyleBlightlords::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Blighted Weapon
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_blightedWeapon.name())) {
            return Dice::RollD6();
        }
        return NurgleBase::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    void PusgoyleBlightlords::onStartHero(PlayerId player) {
        // Virulent Discharge
        auto units = Board::Instance()->getUnitsWithin(this, PlayerId::None, 3.0);
        for (auto ip : units) {
            if (ip->hasKeyword(NURGLE)) {
                // Heal D3
                ip->heal(Dice::RollD3());
            } else {
                // Inflict D3 mortal wounds
                ip->applyDamage({0, Dice::RollD3()}, this);
            }
        }
        NurgleBase::onStartHero(player);
    }

    int PusgoyleBlightlords::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Nurgle

