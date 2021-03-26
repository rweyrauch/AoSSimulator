/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include "mawtribes/Ironblaster.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 9;
    static const int g_pointsPerUnit = 120;

    bool Ironblaster::s_registered = false;

    bool Ironblaster::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *Ironblaster::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new Ironblaster(tribe);
        }
        return nullptr;
    }

    void Ironblaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Ironblaster::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Ironblaster::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Ironblaster", factoryMethod);
        }
    }

    Ironblaster::Ironblaster(Mawtribe tribe) :
            MawtribesBase(tribe, "Ironblaster", 7, g_wounds, 6, 4, false) {
        m_keywords = {DESTRUCTION, OGOR, RHINOX, OGOR_MAWTRIBES, GUTBUSTERS, IRONBLASTER};
        m_weapons = {&m_cannonBall, &m_hailShot, &m_clubber, &m_horns, &m_blade};
        m_battleFieldRole = Role::Artillery;
        m_hasMount = true;
        m_horns.setMount(true);

        auto model = new Model(g_basesize, wounds());

        m_hailShot.activate(false);

        model->addMissileWeapon(&m_cannonBall);
        model->addMissileWeapon(&m_hailShot);
        model->addMeleeWeapon(&m_clubber);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_blade);

        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Wounds Ironblaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Rhinox Charge
        if (m_charged && (weapon->name() == m_horns.name())) {
            return {weapon->damage() + 1, 0};
        }

        return MawtribesBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Ironblaster::onStartShooting(PlayerId player) {
        MawtribesBase::onStartShooting(player);

        auto board = Board::Instance();
        PlayerId otherPlayer = PlayerId::Red;
        if (player == PlayerId::Red) {
            otherPlayer = PlayerId::Blue;
        }
        auto otherRoster = board->getPlayerRoster(otherPlayer);

        auto nearestUnit = otherRoster ? otherRoster->nearestUnit(this) : nullptr;
        if (nearestUnit) {
            double rangeTo = distanceTo(nearestUnit);
            if (rangeTo < (double) m_hailShot.range()) {
                m_hailShot.activate(true);
                m_cannonBall.activate(false);
            } else {
                m_hailShot.activate(false);
                m_cannonBall.activate(true);
            }
        }
    }

    int Ironblaster::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
