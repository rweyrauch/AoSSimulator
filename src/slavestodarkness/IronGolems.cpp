/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/IronGolems.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 8;
    static const int g_maxUnitSize = 32;
    static const int g_pointsPerBlock = 70;
    static const int g_pointsMaxUnitSize = 280;

    bool IronGolems::s_registered = false;

    Unit *IronGolems::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        return new IronGolems(legion, numModels, ComputePoints(parameters));
    }

    void IronGolems::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    IronGolems::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    IronGolems::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Iron Golems", factoryMethod);
        }
    }

    IronGolems::IronGolems(DamnedLegion legion, int numModels, int points) :
            SlavesToDarknessBase(legion, "Iron Golems", 5, g_wounds, 6, 4, false, points) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, CULTISTS, IRON_GOLEMS};
        m_weapons = {&m_bolas, &m_legionWeapons, &m_legionWeaponsDominar};

        auto dominar = new Model(g_basesize, wounds());
        dominar->addMissileWeapon(&m_bolas);
        dominar->addMeleeWeapon(&m_legionWeaponsDominar);
        dominar->setName("Dominar");
        addModel(dominar);

        auto signifer = new Model(g_basesize, wounds());
        signifer->addMissileWeapon(&m_bolas);
        signifer->addMeleeWeapon(&m_legionWeapons);
        signifer->setName("Signifer");
        addModel(signifer);

        auto breacher = new Model(g_basesize, 3);
        breacher->addMissileWeapon(&m_bolas);
        breacher->addMeleeWeapon(&m_legionWeapons);
        breacher->setName("Ogor Breacher");
        addModel(breacher);

        for (auto i = 3; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_bolas);
            model->addMeleeWeapon(&m_legionWeapons);
            addModel(model);
        }
    }

    int IronGolems::braveryModifier() const {
        int modifier = SlavesToDarknessBase::braveryModifier();
        if (isNamedModelAlive("Signifer")) {
            modifier += 2;
        }
        return modifier;
    }

    int IronGolems::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls IronGolems::toSaveRerolls(const Weapon *weapon, const Unit *attacker) const {
        // Iron Resilience
        if (!m_moved) return Rerolls::Failed;

        return SlavesToDarknessBase::toSaveRerolls(weapon, attacker);
    }

} //SlavesToDarkness