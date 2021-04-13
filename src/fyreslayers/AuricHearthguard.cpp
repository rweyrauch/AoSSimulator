/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricHearthguard.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 600;

    bool AuricHearthguard::s_registered = false;

    AuricHearthguard::AuricHearthguard() :
            Fyreslayer("Auric Hearthguard", 4, g_wounds, 7, 5, false),
            m_magmapike(Weapon::Type::Missile, "Magmapike", 18, 2, 4, 3, -1, 1),
            m_magmapikeKarl(Weapon::Type::Missile, "Magmapike", 18, 3, 4, 3, -1, 1),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_magmapikeMelee(Weapon::Type::Melee, "Magmapike", 1, 1, 3, 3, 0, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, AURIC_HEARTHGUARD};
        m_weapons = {&m_magmapike, &m_magmapikeKarl, &m_throwingAxe, &m_magmapikeMelee};
    }

    bool AuricHearthguard::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto karl = new Model(g_basesize, wounds());
        karl->addMissileWeapon(&m_magmapikeKarl);
        karl->addMissileWeapon(&m_throwingAxe);
        karl->addMeleeWeapon(&m_magmapikeMelee);
        addModel(karl);

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_magmapike);
            model->addMissileWeapon(&m_throwingAxe);
            model->addMeleeWeapon(&m_magmapikeMelee);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *AuricHearthguard::Create(const ParameterList &parameters) {
        auto unit = new AuricHearthguard();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        unit->setLodge(lodge);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void AuricHearthguard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricHearthguard::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricHearthguard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Hearthguard", factoryMethod);
        }
    }

    Wounds AuricHearthguard::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Molten Rockbolts
        if ((weapon->name() == m_magmapike.name()) && target->hasKeyword(MONSTER)) {
            return {weapon->damage() + 1, 0};
        }
        return Fyreslayer::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int AuricHearthguard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace Fyreslayers


