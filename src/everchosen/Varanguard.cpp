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

namespace SlavesToDarkness {
    static const int g_basesize = 75; //x42 oval
    static const int g_wounds = 5;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 280;
    static const int g_pointsMaxUnitSize = g_pointsPerBlock * 4;

    bool Varanguard::s_registered = false;

    Unit *Varanguard::Create(const ParameterList &parameters) {
        auto unit = new Varanguard();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Daemonforged_Blade);

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

    Varanguard::Varanguard() :
            SlavesToDarknessBase("Varanguard", 10, g_wounds, 9, 3, false),
            m_ensorcelledWeapon(Weapon::Type::Melee, "Ensorcelled Weapon", 1, 6, 3, 3, -1, 1),
            m_fellspear(Weapon::Type::Melee, "Fellspear", 2, 3, 3, 4, -1, 2),
            m_blade(Weapon::Type::Melee, "Daemonforged Blade", 1, 3, 3, 3, -1, RAND_D3),
            m_fangs(Weapon::Type::Melee, "Tearing Fangs", 1, 3, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, EVERCHOSEN, MARK_OF_CHAOS, VARANGUARD};
        m_weapons = {&m_ensorcelledWeapon, &m_fellspear, &m_blade, &m_fangs};
        m_hasMount = true;
    }

    bool Varanguard::configure(int numModels, Varanguard::WeaponOption weapon) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            if (weapon == Ensorcelled_Weapon) {
                model->addMeleeWeapon(&m_ensorcelledWeapon);
            } else if (weapon == Fellspear) {
                model->addMeleeWeapon(&m_fellspear);
            } else if (weapon == Daemonforged_Blade) {
                model->addMeleeWeapon(&m_blade);
            }
            model->addMeleeWeapon(&m_fangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds Varanguard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Daemonbound
        if ((hitRoll == 6) && (weapon->name() == m_blade.name())) {
            damage.mortal++;
        }
        return damage;
    }

    int Varanguard::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);

        // Impaling Charge
        if (m_charged && (weapon->name() == m_fellspear.name())) {
            rend -= 1;
        }
        return rend;
    }

    int Varanguard::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toWoundModifier(weapon, target);

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

} //namespace SlavesToDarkness