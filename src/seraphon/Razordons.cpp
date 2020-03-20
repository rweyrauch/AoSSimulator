/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/Razordons.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon {
    static const int BASESIZE = 26;
    static const int BASESIZE_RAZORDON = 60; // x35 oval
    static const int WOUNDS = 1;
    static const int WOUNDS_RAZORDON = 3;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 240;

    bool Razordons::s_registered = false;

    Razordons::Razordons() :
            SeraphonBase("Razordons", 8, WOUNDS, 5, 4, false),
            m_spikes(Weapon::Type::Missile, "Volley of Spikes", 18, RAND_2D6, 3, 4, 0, 1),
            m_tail(Weapon::Type::Melee, "Spiked Tail", 1, 3, 3, 3, -2, 2),
            m_goad(Weapon::Type::Melee, "Celestite Goad", 1, 1, 4, 5, 0, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, RAZORDON, HUNTING_PACK};
        m_weapons = {&m_spikes, &m_tail, &m_goad};
    }

    bool Razordons::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        auto razordon = new Model(BASESIZE_RAZORDON, WOUNDS_RAZORDON);
        razordon->addMissileWeapon(&m_spikes);
        razordon->addMeleeWeapon(&m_tail);
        addModel(razordon);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_goad);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Razordons::Create(const ParameterList &parameters) {
        auto unit = new Razordons();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, SeraphonBase::Starborne);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, SeraphonBase::None);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Razordons::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Enum, "Way of the Seraphon", SeraphonBase::Starborne, SeraphonBase::Starborne,
                             SeraphonBase::Coalesced, 1},
                            {ParamType::Enum, "Constellation", SeraphonBase::None, SeraphonBase::None,
                             SeraphonBase::FangsOfSotek, 1}
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Razordons", factoryMethod);
        }
    }

    int Razordons::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int Razordons::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Piercing Barbs
        if ((weapon->name() == m_spikes.name()) && (distanceTo(m_shootingTarget) <= 6.0f)) {
            return -1;
        }
        return SeraphonBase::weaponRend(weapon, target, hitRoll, woundRoll);
    }

} //namespace Seraphon
