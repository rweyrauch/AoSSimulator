/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/GoreGruntas.h>
#include <UnitFactory.h>

namespace Ironjawz {
    static const int BASESIZE = 90;  // x52 oval
    static const int WOUNDS = 5;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    bool OrrukGoreGruntas::s_registered = false;

    OrrukGoreGruntas::OrrukGoreGruntas() :
            Ironjawz("Orruk Gore-gruntas", 9, WOUNDS, 7, 4, false),
            m_pigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 4, 3, 3, -1, 1),
            m_jaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 3, 3, 3, -1, 1),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 4, 4, 4, 0, 1),
            m_bossPigironChoppa(Weapon::Type::Melee, "Pig-iron Choppa", 1, 5, 3, 3, -1, 1),
            m_bossJaggedGorehacka(Weapon::Type::Melee, "Jagged Gore-hacka", 2, 5, 3, 3, -1, 1) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, GORE_GRUNTAS};
        m_weapons = {&m_pigironChoppa, &m_jaggedGorehacka, &m_tusksAndHooves, &m_bossPigironChoppa,
                     &m_bossJaggedGorehacka};
    }

    bool OrrukGoreGruntas::configure(int numModels, WeaponOption weapons) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        // Add the Boss
        auto bossModel = new Model(BASESIZE, wounds());
        if (weapons == PigIronChoppa) {
            bossModel->addMeleeWeapon(&m_bossPigironChoppa);
        } else if (weapons == JaggedGorehacka) {
            bossModel->addMeleeWeapon(&m_bossJaggedGorehacka);
        }
        bossModel->addMeleeWeapon(&m_tusksAndHooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            if (weapons == PigIronChoppa) {
                model->addMeleeWeapon(&m_pigironChoppa);
            } else if (weapons == JaggedGorehacka) {
                model->addMeleeWeapon(&m_jaggedGorehacka);
            }
            model->addMeleeWeapon(&m_tusksAndHooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *OrrukGoreGruntas::Create(const ParameterList &parameters) {
        auto unit = new OrrukGoreGruntas();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        WeaponOption weapons = (WeaponOption) GetEnumParam("Weapons", parameters, PigIronChoppa);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Ironjawz::Ironsunz);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string OrrukGoreGruntas::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "weapons") {
            if (parameter.intValue == PigIronChoppa) {
                return "Pig-iron Choppa";
            } else if (parameter.intValue == JaggedGorehacka) {
                return "Jagged Gore-hacka";
            }
        }
        return Ironjawz::ValueToString(parameter);
    }

    int OrrukGoreGruntas::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Pig-iron Choppa") {
            return PigIronChoppa;
        } else if (enumString == "Jagged Gore-hacka") {
            return JaggedGorehacka;
        }
        return Ironjawz::EnumStringToInt(enumString);
    }

    void OrrukGoreGruntas::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukGoreGruntas::Create,
                    OrrukGoreGruntas::ValueToString,
                    OrrukGoreGruntas::EnumStringToInt,
                    OrrukGoreGruntas::ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {
                                    ParamType::Enum, "Weapons", OrrukGoreGruntas::PigIronChoppa,
                                    OrrukGoreGruntas::PigIronChoppa,
                                    OrrukGoreGruntas::JaggedGorehacka, 1
                            },
                            {ParamType::Enum, "Warclan", Ironjawz::Ironsunz, Ironjawz::Ironsunz, Ironjawz::DaChoppas,
                             1},
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Gore-gruntas", factoryMethod);
        }
    }

    int OrrukGoreGruntas::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace Ironjawz