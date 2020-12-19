/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/ChaosChariots.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 7;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 360;

    bool ChaosChariots::s_registered = false;

    Unit *ChaosChariots::Create(const ParameterList &parameters) {
        auto unit = new ChaosChariots();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Great_Blade_And_Whip);

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        bool ok = unit->configure(numModels, weapons);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ChaosChariots::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Great_Blade_And_Whip, War_Flail_And_Whip};
            static FactoryMethod factoryMethod = {
                    ChaosChariots::Create,
                    ChaosChariots::ValueToString,
                    ChaosChariots::EnumStringToInt,
                    ChaosChariots::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Great_Blade_And_Whip, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Chariots", factoryMethod);
        }
    }

    ChaosChariots::ChaosChariots() :
            SlavesToDarknessBase("Chaos Chariots", 12, g_wounds, 6, 4, false),
            m_greatBlade(Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 3, 3, -1, 2),
            m_flail(Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 4, 3, 0, 1),
            m_whip(Weapon::Type::Melee, "Lashing Whip", 2, 2, 4, 4, 0, 1),
            m_greatBladeExalted(Weapon::Type::Melee, "Chaos Greatblade", 2, 2, 2, 3, -1, 2),
            m_flailExalted(Weapon::Type::Melee, "Chaos War-flail", 2, RAND_D6, 3, 3, 0, 1),
            m_whipExalted(Weapon::Type::Melee, "Lashing Whip", 2, 2, 3, 4, 0, 1),
            m_hooves(Weapon::Type::Melee, "Trampling Hooves", 1, 4, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_CHARIOTS};
        m_weapons = {&m_greatBlade, &m_flail, &m_whip, &m_greatBladeExalted, &m_flailExalted, &m_whipExalted,
                     &m_hooves};
        m_hasMount = true;
    }

    bool ChaosChariots::configure(int numModels, WeaponOption weapons) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto exalted = new Model(g_basesize, wounds());
        if (weapons == Great_Blade_And_Whip) {
            exalted->addMeleeWeapon(&m_greatBladeExalted);
        } else if (weapons == War_Flail_And_Whip) {
            exalted->addMeleeWeapon(&m_flailExalted);
        }
        exalted->addMeleeWeapon(&m_whipExalted);
        exalted->addMeleeWeapon(&m_hooves);
        exalted->setName("Exalted Charioteer");
        addModel(exalted);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapons == Great_Blade_And_Whip)
                model->addMeleeWeapon(&m_greatBlade);
            else if (weapons == War_Flail_And_Whip)
                model->addMeleeWeapon(&m_flail);
            model->addMeleeWeapon(&m_whip);
            model->addMeleeWeapon(&m_hooves);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    std::string ChaosChariots::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Great_Blade_And_Whip) {
                return "Chaos Greatblade and Whip";
            } else if (parameter.intValue == War_Flail_And_Whip) {
                return "Chaos War-flail and Whip";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int ChaosChariots::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chaos Greatblade and Whip") {
            return Great_Blade_And_Whip;
        } else if (enumString == "Chaos War-flail and Whip") {
            return War_Flail_And_Whip;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int ChaosChariots::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void ChaosChariots::onRestore() {
        Unit::onRestore();

        m_runAndCharge = true;
    }

    void ChaosChariots::onCharged() {
        Unit::onCharged();

        // Don't Spare the Lash
        if (m_ran && m_charged) {
            // Can only run and charge once per battle
            m_runAndCharge = false;
        }

        // Swift Death
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 1.0)) {
            Dice::RollResult result;
            Dice::RollD6(m_unmodifiedChargeRoll, result);
            unit->applyDamage({0, result.rollsGE(5)}, this);
        }
    }

} //SlavesToDarkness