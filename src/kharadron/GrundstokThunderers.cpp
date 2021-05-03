/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <kharadron/GrundstokThunderers.h>
#include <UnitFactory.h>
#include "KharadronPrivate.h"

namespace KharadronOverlords {
    static const int g_basesize = 0;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 480;

    bool GrundstokThunderers::s_registered = false;

    Unit *GrundstokThunderers::Create(const ParameterList &parameters) {
        auto port = (Skyport) GetEnumParam("Skyport", parameters, g_skyport[0]);
        auto artycle = (Artycle) GetEnumParam("Artycle", parameters, g_artycles[0]);
        auto amendment = (Amendment) GetEnumParam("Amendment", parameters, g_amendments[0]);
        auto footnote = (Footnote) GetEnumParam("Footnote", parameters, g_footnotes[0]);
        int numModel = GetIntParam("Models", parameters, g_minUnitSize);
        int numMortars = GetIntParam("Grundstok Mortars", parameters, 1);
        int numCannons = GetIntParam("Aethercannons", parameters, 1);
        int numFumigators = GetIntParam("Aetheric Fumigators", parameters, 1);
        int numDecksweepers = GetIntParam("Desksweepers", parameters, 1);
        int numHonourBearers = GetIntParam("Honour Bearers", parameters, 1);
        return new GrundstokThunderers(port, artycle, amendment, footnote, numModel, numMortars, numCannons, numFumigators, numDecksweepers, numHonourBearers, ComputePoints(parameters));
    }

    std::string GrundstokThunderers::ValueToString(const Parameter &parameter) {
        return KharadronBase::ValueToString(parameter);
    }

    int GrundstokThunderers::EnumStringToInt(const std::string &enumString) {
        return KharadronBase::EnumStringToInt(enumString);
    }

    void GrundstokThunderers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GrundstokThunderers::Create,
                    GrundstokThunderers::ValueToString,
                    GrundstokThunderers::EnumStringToInt,
                    GrundstokThunderers::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Grundstok Mortars", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Aethercannons", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Aetheric Fumigator", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Desksweepers", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Honour Bearers", 1, 0, g_maxUnitSize / g_minUnitSize, 1),
                            EnumParameter("Skyport", g_skyport[0], g_skyport),
                            EnumParameter("Artycle", g_artycles[0], g_artycles),
                            EnumParameter("Amendment", g_amendments[0], g_amendments),
                            EnumParameter("Footnote", g_footnotes[0], g_footnotes),
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Endrinriggers", factoryMethod);
        }
    }

    GrundstokThunderers::GrundstokThunderers(Skyport port, Artycle artycle, Amendment amendment, Footnote footnote, int numModels, int numMortars,
                                             int numCannons, int numFumigators, int numDecksweeper, int numHonourBearers, int points) :
            KharadronBase(port, artycle, amendment, footnote, "Grundstok Thunderers", 4, g_wounds, 7, 4, false, points),
            m_rifle(Weapon::Type::Missile, "Aethershot Rifle", 18, 2, 3, 4, -1, 1),
            m_doubleBarrelledRifle(Weapon::Type::Missile, "Double-barrelled Aethershot Rifle", 18, 4, 3, 4, -1, 1),
            m_fumigator(Weapon::Type::Missile, "Aetheric Fumigator", 9, 3, 3, 3, -1, 1),
            m_decksweeper(Weapon::Type::Missile, "Desksweeper", 12, 4, 4, 4, -1, 1),
            m_aethercannon(Weapon::Type::Missile, "Aethercannon", 12, 1, 4, 2, -2, RAND_D3),
            m_mortar(Weapon::Type::Missile, "Grundstok Mortar", 12, 1, 4, 3, 0, RAND_D3),
            m_drillbill(Weapon::Type::Melee, "Drillbill", 3, RAND_D3, 4, 4, -1, 1),
            m_gunButt(Weapon::Type::Melee, "Gun Butt", 1, 1, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, SKYFARER, MARINE, GRUNDSTOK_THUNDERERS};
        m_weapons = {&m_rifle,
                     &m_doubleBarrelledRifle,
                     &m_fumigator,
                     &m_decksweeper,
                     &m_aethercannon,
                     &m_mortar,
                     &m_drillbill,
                     &m_gunButt};

        auto sergeant = new Model(g_basesize, wounds());
        sergeant->addMissileWeapon(&m_doubleBarrelledRifle);
        sergeant->addMeleeWeapon(&m_drillbill);
        sergeant->addMeleeWeapon(&m_gunButt);
        addModel(sergeant);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (numMortars > 0) {
                model->addMissileWeapon(&m_mortar);
                numMortars--;
            } else if (numCannons > 0) {
                model->addMissileWeapon(&m_aethercannon);
                numCannons--;
            } else if (numDecksweeper > 0) {
                model->addMissileWeapon(&m_decksweeper);
                numDecksweeper--;
            } else if (numFumigators > 0) {
                model->addMissileWeapon(&m_fumigator);
                numFumigators--;
            } else {
                model->addMissileWeapon(&m_rifle);
            }
            if (numHonourBearers > 0) {
                model->setName("Honour Bearer");
                numHonourBearers--;
            }
            model->addMeleeWeapon(&m_gunButt);
            addModel(model);
        }
    }

    int GrundstokThunderers::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //KharadronOverlords
