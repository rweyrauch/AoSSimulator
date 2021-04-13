/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "everchosen/Varanguard.h"
#include "../slavestodarkness/SlavesToDarknessPrivate.h"
#include <array>
#include <Board.h>

namespace SlavesToDarkness {
    static const int g_basesize = 75; //x42 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 280;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Varanguard::s_registered = false;

    Unit *Varanguard::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Daemonforged_Blade);
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);

        return new Varanguard(legion, mark, numModels, weapons);
    }

    std::string Varanguard::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            switch (parameter.intValue) {
                case Ensorcelled_Weapon:
                    return "Ensorcelled Weapon";
                case Fellspear:
                    return "Fellspear";
                case Daemonforged_Blade:
                    return "Daemonforged Blade";
                default:
                    break;
            }
        }
        return SlavesToDarknessBase::ValueToString(parameter);
    }

    int Varanguard::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Ensorcelled Weapon") return Ensorcelled_Weapon;
        else if (enumString == "Fellspear") return Fellspear;
        else if (enumString == "Daemonforged Blade") return Daemonforged_Blade;

        return SlavesToDarknessBase::EnumStringToInt(enumString);
    }

    void Varanguard::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Ensorcelled_Weapon, Fellspear, Daemonforged_Blade};
            static FactoryMethod factoryMethod = {
                    Varanguard::Create,
                    Varanguard::ValueToString,
                    Varanguard::EnumStringToInt,
                    Varanguard::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Daemonforged_Blade, weapons),
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, SLAANESH, TZEENTCH, NURGLE}
            };
            s_registered = UnitFactory::Register("Varanguard", factoryMethod);
        }
    }

    Varanguard::Varanguard(DamnedLegion legion, MarkOfChaos mark, int numModels, WeaponOption option) :
            SlavesToDarknessBase("Varanguard", 10, g_wounds, 9, 3, false) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EVERCHOSEN, MARK_OF_CHAOS, VARANGUARD};
        m_weapons = {&m_ensorcelledWeapon, &m_fellspear, &m_blade, &m_fangs};
        m_hasMount = true;
        m_fangs.setMount(true);

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (option == Ensorcelled_Weapon) {
                model->addMeleeWeapon(&m_ensorcelledWeapon);
            } else if (option == Fellspear) {
                model->addMeleeWeapon(&m_fellspear);
            } else if (option == Daemonforged_Blade) {
                model->addMeleeWeapon(&m_blade);
            }
            model->addMeleeWeapon(&m_fangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    Wounds Varanguard::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = SlavesToDarknessBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
        // Daemonbound
        if ((hitRoll == 6) && (weapon->name() == m_blade.name())) {
            damage.mortal++;
        }
        return damage;
    }

    int Varanguard::weaponRend(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = SlavesToDarknessBase::weaponRend(attackingModel, weapon, target, hitRoll, woundRoll);

        // Impaling Charge
        if (m_charged && (weapon->name() == m_fellspear.name())) {
            rend -= 1;
        }
        return rend;
    }

    int Varanguard::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SlavesToDarknessBase::toWoundModifier(weapon, target);

        // Impaling Charge
        if (m_charged && (weapon->name() == m_fellspear.name())) {
            mod++;
        }

        return mod;
    }

    int Varanguard::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Wounds Varanguard::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = SlavesToDarknessBase::applyWoundSave(wounds, attackingUnit);

        if (wounds.source == Wounds::Source::Spell) {
            // Warpsteel Shields
            if (Dice::RollD6() >= 5) {
                totalWounds.mortal = 0;
            }
        }
        return totalWounds;
    }

    int Varanguard::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SlavesToDarknessBase::toHitModifier(weapon, target);

        // Favoured of the Everchosen
        auto units = Board::Instance()->getAllUnits(owningPlayer());
        for (auto unit : units) {
            if (unit->isGeneral() && unit->isNamedModelAlive("Archaon")) {
                mod++;
            }
        }
        return mod;
    }

    void Varanguard::onEndCombat(PlayerId player) {
        SlavesToDarknessBase::onEndCombat(player);

        // Relentless Killers
        if (!m_usedRelentlessKillers) {
            if (meleeTarget() && (distanceTo(meleeTarget()) <= 3.0f)) {
                int numSlain;
                auto wounds = fight(-1, meleeTarget(), numSlain);
                if (!wounds.zero()) {
                    PLOG_INFO << name() << " using Relentless Killers, fights again inflicting " << wounds
                              << " wounds.  Slaying " << numSlain;
                }
                m_usedRelentlessKillers = true;
            }
        }
    }

    void Varanguard::onRestore() {
        SlavesToDarknessBase::onRestore();

        m_usedRelentlessKillers = false;
    }

} //namespace SlavesToDarkness