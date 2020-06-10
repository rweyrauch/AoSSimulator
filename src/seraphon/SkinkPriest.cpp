/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SkinkPriest.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Seraphon {
    static const int BASESIZE = 25;
    static const int WOUNDS = 4;
    static const int POINTS_PER_UNIT = 70;

    bool SkinkPriest::s_registered = false;

    SkinkPriest::SkinkPriest() :
            SeraphonBase("Skink Priest", 8, WOUNDS, 6, 5, false),
            m_starbolt(Weapon::Type::Missile, "Starbolt", 18, 2, 4, 3, -1, 1),
            m_staff(Weapon::Type::Melee, "Star-stone Staff", 1, 1, 4, 4, -1, 1) {
        m_keywords = {ORDER, SERAPHON, SKINK, HERO, PRIEST};
        m_weapons = {&m_starbolt, &m_staff};
    }

    bool SkinkPriest::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_starbolt);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    Unit *SkinkPriest::Create(const ParameterList &parameters) {
        auto unit = new SkinkPriest();

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, Starborne);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, NoConstellation);
        unit->setWayOfTheSeraphon(way, constellation);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SkinkPriest::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SeraphonBase::ValueToString,
                    SeraphonBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Way of the Seraphon", Seraphon::Starborne, g_wayOfTheSeraphon),
                            EnumParameter("Constellation", NoConstellation, g_constellation)
                    },
                    ORDER,
                    {SERAPHON}
            };
            s_registered = UnitFactory::Register("Skink Priest", factoryMethod);
        }
    }

    int SkinkPriest::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Seraphon
