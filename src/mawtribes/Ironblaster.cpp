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

    Unit *Ironblaster::Create(const ParameterList &parameters) {
        auto unit = new Ironblaster();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Ironblaster::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int Ironblaster::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Ironblaster::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Ironblaster::Create,
                    Ironblaster::ValueToString,
                    Ironblaster::EnumStringToInt,
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

    Ironblaster::Ironblaster() :
            MawtribesBase("Ironblaster", 7, g_wounds, 6, 4, false),
            m_cannonBall(Weapon::Type::Missile, "Ironblaster Cannon: Cannon Ball", 24, 1, 4, 2, -2, RAND_D6),
            m_hailShot(Weapon::Type::Missile, "Ironblaster Cannon: Hail Shot", 12, 6, 3, 3, -1, 1),
            m_clubber(Weapon::Type::Melee, "Gunner's Clubber", 1, 3, 3, 3, 0, 2),
            m_horns(Weapon::Type::Melee, "Rhinox's Sharp Horns", 1, 2, 4, 3, -1, RAND_D3),
            m_blade(Weapon::Type::Melee, "Scrapper's Jagged Blade", 1, 2, 5, 5, 0, 1) {
        m_keywords = {DESTRUCTION, OGOR, RHINOX, OGOR_MAWTRIBES, GUTBUSTERS, IRONBLASTER};
        m_weapons = {&m_cannonBall, &m_hailShot, &m_clubber, &m_horns, &m_blade};
        m_battleFieldRole = Artillery;
    }

    bool Ironblaster::configure() {
        auto model = new Model(g_basesize, wounds());

        m_hailShot.activate(false);

        model->addMissileWeapon(&m_cannonBall);
        model->addMissileWeapon(&m_hailShot);
        model->addMeleeWeapon(&m_clubber);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_blade);

        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds Ironblaster::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Rhinox Charge
        if (m_charged && (weapon->name() == m_horns.name())) {
            return {weapon->damage() + 1, 0};
        }

        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Ironblaster::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

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
