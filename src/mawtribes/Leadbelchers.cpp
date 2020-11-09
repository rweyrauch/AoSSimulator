/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Leadbelchers.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 480;


    bool Leadbelchers::s_registered = false;

    Unit *Leadbelchers::Create(const ParameterList &parameters) {
        auto unit = new Leadbelchers();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Leadbelchers::ValueToString(const Parameter &parameter) {
        return MawtribesBase::ValueToString(parameter);
    }

    int Leadbelchers::EnumStringToInt(const std::string &enumString) {
        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Leadbelchers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Leadbelchers::Create,
                    Leadbelchers::ValueToString,
                    Leadbelchers::EnumStringToInt,
                    Leadbelchers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Leadbelchers", factoryMethod);
        }
    }

    Leadbelchers::Leadbelchers() :
            MawtribesBase("Leadbelchers", 6, g_wounds, 6, 5, false),
            m_gun(Weapon::Type::Missile, "Leadbelcher Gun", 12, RAND_D3, 4, 3, -1, 1),
            m_blow(Weapon::Type::Melee, "Bludgeoning Blow", 1, 2, 3, 3, -1, 2),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
            m_blowThunderfist(Weapon::Type::Melee, "Bludgeoning Blow", 1, 3, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, LEADBELCHERS};
        m_weapons = {&m_gun, &m_blow, &m_bite, &m_blowThunderfist};
    }

    bool Leadbelchers::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        // Gunmasters
        if (hasKeyword(UNDERGUTS)) {
            m_gun.setRange(m_gun.range() + 6);
        }

        auto boss = new Model(g_basesize, wounds());
        boss->addMissileWeapon(&m_gun);
        boss->addMeleeWeapon(&m_blowThunderfist);
        boss->addMeleeWeapon(&m_bite);
        addModel(boss);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_gun);
            model->addMeleeWeapon(&m_blow);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    void Leadbelchers::onStartShooting(PlayerId player) {
        Unit::onStartShooting(player);

        if (m_moved) {
            m_gun.setAttacks(RAND_D3);
        } else {
            m_gun.setAttacks(RAND_D6);
        }
    }

    int Leadbelchers::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace OgorMawtribes
