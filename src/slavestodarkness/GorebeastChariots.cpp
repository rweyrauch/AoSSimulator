/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "slavestodarkness/GorebeastChariots.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 9;
    static const int g_minUnitSize = 1;
    static const int g_maxUnitSize = 3;
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = 450;

    bool GorebeastChariots::s_registered = false;

    Unit *GorebeastChariots::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Great_Blade_And_Whip);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto lord = GetBoolParam("Idolator Lord", parameters, false);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_idolatorCommandTraits[0]);
        auto prayer = (Prayer) GetEnumParam("Idolator Prayer", parameters, g_idolatorPrayers[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new GorebeastChariots(legion, mark, numModels, weapons, lord, trait, prayer, general, ComputePoints(parameters));
    }

    void GorebeastChariots::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Great_Blade_And_Whip, War_Flail_And_Whip};
            static FactoryMethod factoryMethod = {
                    GorebeastChariots::Create,
                    GorebeastChariots::ValueToString,
                    GorebeastChariots::EnumStringToInt,
                    GorebeastChariots::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Great_Blade_And_Whip, weapons),
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
            s_registered = UnitFactory::Register("Gorebeast Chariots", factoryMethod);
        }
    }

    GorebeastChariots::GorebeastChariots(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption weapons, bool idolatorLord,
                                         CommandTrait trait, Prayer prayer, bool isGeneral, int points) :
            SlavesToDarknessBase(legion, "Gorebeast Chariots", 9, g_wounds, 6, 4, false, points) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, GOREBEAST_CHARIOTS};
        m_weapons = {&m_greatBlade, &m_flail, &m_whip, &m_greatBladeExalted, &m_flailExalted, &m_whipExalted, &m_fists};
        m_hasMount = true;
        m_fists.setMount(true);

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
            exalted->addMeleeWeapon(&m_fists);
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
            model->addMeleeWeapon(&m_fists);
            addModel(model);
        }
    }

    std::string GorebeastChariots::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Great_Blade_And_Whip) {
                return "Chaos Greatblade and Whip";
            } else if (parameter.intValue == War_Flail_And_Whip) {
                return "Chaos War-flail and Whip";
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int GorebeastChariots::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Chaos Greatblade and Whip") {
            return Great_Blade_And_Whip;
        } else if (enumString == "Chaos War-flail and Whip") {
            return War_Flail_And_Whip;
        }
        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    int GorebeastChariots::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int GorebeastChariots::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SlavesToDarknessBase::toHitModifier(weapon, target);

        // Explosive Brutality
        if (m_charged && (m_unmodifiedChargeRoll >= 8)) mod++;

        return mod;
    }

    int GorebeastChariots::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SlavesToDarknessBase::toWoundModifier(weapon, target);

        // Explosive Brutality
        if (m_charged && (m_unmodifiedChargeRoll >= 8)) mod++;

        return mod;
    }

    void GorebeastChariots::onCharged() {
        SlavesToDarknessBase::onCharged();

        // Crashing Charge
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto unit : units) {
            if (Dice::RollD6() >= 2) {
                unit->applyDamage({0, Dice::RollD3()}, this);
            }
        }
    }

} //SlavesToDarkness