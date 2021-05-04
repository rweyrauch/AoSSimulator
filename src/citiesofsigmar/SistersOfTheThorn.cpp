/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/SistersOfTheThorn.h"
#include "CitiesOfSigmarPrivate.h"

namespace CitiesOfSigmar {
    static const int g_basesize = 60;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = 130 * 4;

    bool SistersOfTheThorn::s_registered = false;

    Unit *SistersOfTheThorn::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool standard = GetBoolParam("Standard Bearer", parameters, true);
        bool hornblower = GetBoolParam("Hornblower", parameters, true);
        return new SistersOfTheThorn(city, numModels, standard, hornblower, ComputePoints(parameters));
    }

    std::string SistersOfTheThorn::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int SistersOfTheThorn::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void SistersOfTheThorn::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SistersOfTheThorn::Create,
                    SistersOfTheThorn::ValueToString,
                    SistersOfTheThorn::EnumStringToInt,
                    SistersOfTheThorn::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Standard Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("City", g_city[0], g_city),
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Sisters of the Thorn", factoryMethod);
        }
    }

    SistersOfTheThorn::SistersOfTheThorn(City city, int numModels, bool standardBearer, bool hornblower, int points) :
            CitizenOfSigmar(city, "Sisters of the Thorn", 12, g_wounds, 7, 5, false, points),
            m_javelin(Weapon::Type::Missile, "Blackbriar Javelin", 9, 2, 4, 4, -1, 1),
            m_staff(Weapon::Type::Melee, "Deepwood Coven Staff", 2, 1, 4, 4, 0, 1),
            m_antlersAndHooves(Weapon::Type::Melee, "Antlers and Horns", 1, 2, 4, 4, 0, 1),
            m_staffMaiden(Weapon::Type::Melee, "Deepwood Coven Staff", 2, 2, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, WANDERER, WIZARD, SISTERS_OF_THE_THORN};
        m_weapons = {&m_javelin, &m_staff, &m_antlersAndHooves, &m_staffMaiden};
        m_hasMount = true;
        m_antlersAndHooves.setMount(true);

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        // Add the Handmaiden
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMissileWeapon(&m_javelin);
        bossModel->addMeleeWeapon(&m_staffMaiden);
        bossModel->addMeleeWeapon(&m_antlersAndHooves);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_javelin);
            model->addMeleeWeapon(&m_staff);
            model->addMeleeWeapon(&m_antlersAndHooves);
            if (standardBearer) {
                model->setName(Model::StandardBearer);
                standardBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int SistersOfTheThorn::runModifier() const {
        auto mod = CitizenOfSigmar::runModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int SistersOfTheThorn::chargeModifier() const {
        auto mod = CitizenOfSigmar::chargeModifier();
        if (isNamedModelAlive(Model::Hornblower)) mod++;
        return mod;
    }

    int SistersOfTheThorn::braveryModifier() const {
        auto mod = CitizenOfSigmar::braveryModifier();
        if (isNamedModelAlive(Model::StandardBearer)) mod++;
        return mod;
    }

    int SistersOfTheThorn::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace CitiesOfSigmar