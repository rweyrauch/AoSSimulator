/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/Maneaters.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 180;
    static const int g_pointsMaxUnitSize = 720;

    bool Maneaters::s_registered = false;

    bool Maneaters::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *Maneaters::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            auto ability = (Ability) GetEnumParam("Ability", parameters, Brawlers);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new Maneaters(tribe, numModels, ability, ComputePoints(parameters));
        }
        return nullptr;
    }

    std::string Maneaters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Ability") {
            if (parameter.intValue == Brawlers) return "Brawlers";
            else if (parameter.intValue == Crack_Shots) return "Crack Shots";
            else if (parameter.intValue == Striders) return "Striders";
            else if (parameter.intValue == Stubborn) return "Stubborn";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int Maneaters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Brawlers") return Brawlers;
        else if (enumString == "Crack Shots") return Crack_Shots;
        else if (enumString == "Striders") return Striders;
        else if (enumString == "Stubborn") return Stubborn;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Maneaters::Init() {
        if (!s_registered) {
            static const std::array<int, 4> abilities = {Brawlers, Crack_Shots, Striders, Stubborn};
            static FactoryMethod factoryMethod = {
                    Maneaters::Create,
                    Maneaters::ValueToString,
                    Maneaters::EnumStringToInt,
                    Maneaters::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Ability", Brawlers, abilities),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Maneaters", factoryMethod);
        }
    }

    Maneaters::Maneaters(Mawtribe tribe, int numModels, Ability ability, int points) :
            MawtribesBase(tribe, "Maneaters", 6, g_wounds, 7, 5, false, points) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, MANEATERS};
        m_weapons = {&m_pistolsOrStars, &m_bite, &m_bashers};

        m_ability = ability;

        if (m_ability == Striders)
            m_runAndCharge = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_pistolsOrStars);
            model->addMeleeWeapon(&m_bashers);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }
    }

    int Maneaters::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls Maneaters::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_ability == Brawlers) {
            if (!weapon->isMissile()) {
                return Rerolls::Ones;
            } else if (m_ability == Crack_Shots) {
                if (weapon->isMissile()) {
                    return Rerolls::Ones;
                }
            }
        } else {
            if (m_ability == Crack_Shots) {
                if (weapon->isMissile()) {
                    return Rerolls::Ones;
                }
            }
        }
        return MawtribesBase::toHitRerolls(weapon, target);
    }

    bool Maneaters::battleshockRequired() const {
        if (m_ability == Stubborn) return false;
        return MawtribesBase::battleshockRequired();
    }

} // namespace OgorMawtribes
