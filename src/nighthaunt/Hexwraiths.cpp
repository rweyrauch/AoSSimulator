/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nighthaunt/Hexwraiths.h>
#include <UnitFactory.h>

namespace Nighthaunt {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 130;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Hexwraiths::s_registered = false;

    Hexwraiths::Hexwraiths() :
            Nighthaunt("Hexwraiths", 12, g_wounds, 10, 4, true),
            m_spectralScythe(Weapon::Type::Melee, "Spectral Scythe", 1, 2, 4, 3, -1, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 2, 4, 5, 0, 1),
            m_spectralScytheHellwraith(Weapon::Type::Melee, "Spectral Scythe", 1, 3, 4, 3, -1, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, SUMMONABLE, HEXWRAITHS};
        m_weapons = {&m_spectralScythe, &m_spectralScytheHellwraith, &m_hoovesAndTeeth};
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
    }

    bool Hexwraiths::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto boss = new Model(g_basesize, wounds());
        boss->addMeleeWeapon(&m_spectralScytheHellwraith);
        boss->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(boss);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_spectralScythe);
            model->addMeleeWeapon(&m_hoovesAndTeeth);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Hexwraiths::Create(const ParameterList &parameters) {
        auto unit = new Hexwraiths();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Hexwraiths::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Hexwraiths::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    Hexwraiths::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Hexwraiths", factoryMethod);
        }
    }

    Wounds Hexwraiths::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((woundRoll == 6) && (weapon->name() == m_spectralScythe.name())) {
            return {0, 1};
        }
        return Nighthaunt::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Hexwraiths::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Nighthaunt
