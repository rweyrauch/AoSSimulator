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
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Great_Blade_And_Whip);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto lord = GetBoolParam("Idolator Lord", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_idolatorCommandTraits[0]);
        auto prayer = (Prayer) GetEnumParam("Idolator Prayer", parameters, g_idolatorPrayers[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new ChaosChariots(legion, mark, numModels, weapons, lord, trait, prayer, general);
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
                            EnumParameter("Weapons", weapons[0], weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            BoolParameter("Idolator Lord"),
                            EnumParameter("Command Trait", g_idolatorCommandTraits[0], g_idolatorCommandTraits),
                            EnumParameter("Idolator Prayer", g_idolatorPrayers[0], g_idolatorPrayers),
                            BoolParameter("General"),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Chariots", factoryMethod);
        }
    }

    ChaosChariots::ChaosChariots(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption weapons, bool idolatorLord,
                                 CommandTrait trait, Prayer prayer, bool isGeneral) :
            SlavesToDarknessBase("Chaos Chariots", 12, g_wounds, 6, 4, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, CHAOS_CHARIOTS};
        m_weapons = {&m_greatBlade, &m_flail, &m_whip, &m_greatBladeExalted, &m_flailExalted, &m_whipExalted,
                     &m_hooves};
        m_hasMount = true;
        m_hooves.setMount(true);

        setDamnedLegion(legion);
        setMarkOfChaos(mark);
        setGeneral(isGeneral);
        setCommandTrait(trait);

        if ((legion == DamnedLegion::Idolators) && idolatorLord) {
            m_keywords.push_back(IDOLATOR_LORD);
            m_keywords.push_back(HERO);
            m_keywords.push_back(PRIEST);
            m_battleFieldRole = Role::Leader;
        }

        if ((numModels > 1) || ((legion == DamnedLegion::Idolators) && idolatorLord)) {
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
        }

        for (auto i = (int)m_models.size(); i < numModels; i++) {
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
        SlavesToDarknessBase::onRestore();

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