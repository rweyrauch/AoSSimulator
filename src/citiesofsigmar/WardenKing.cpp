/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <citiesofsigmar/WardenKing.h>
#include <UnitFactory.h>

namespace CitiesOfSigmar {
    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 110;

    bool WardenKing::s_registered = false;

    WardenKing::WardenKing() :
            CitizenOfSigmar("Warden King", 4, WOUNDS, 8, 3, false),
            m_runeWeapon(Weapon::Type::Melee, "Rune Hammer", 1, 4, 3, 3, -1, RAND_D3) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, WARDEN_KING};
        m_weapons = {&m_runeWeapon};
    }

    bool WardenKing::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_runeWeapon);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *WardenKing::Create(const ParameterList &parameters) {
        auto unit = new WardenKing();

        auto city = (City) GetEnumParam("City", parameters, Hammerhal);
        unit->setCity(city);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WardenKing::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    WardenKing::Create,
                    CitizenOfSigmar::ValueToString,
                    CitizenOfSigmar::EnumStringToInt,
                    WardenKing::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Warden King", factoryMethod);
        }
    }

    int WardenKing::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace CitiesOfSigmar