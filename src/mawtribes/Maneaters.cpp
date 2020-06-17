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
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = 720;

    bool Maneaters::s_registered = false;

    Unit *Maneaters::Create(const ParameterList &parameters) {
        auto unit = new Maneaters();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        auto ability = (Ability) GetEnumParam("Ability", parameters, Brawlers);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(numModels, ability);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Maneaters::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Ability") {
            if (parameter.intValue == Brawlers) return "Brawlers";
            else if (parameter.intValue == CrackShots) return "Crack Shots";
            else if (parameter.intValue == Striders) return "Striders";
            else if (parameter.intValue == Stubborn) return "Stubborn";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int Maneaters::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Brawlers") return Brawlers;
        else if (enumString == "Crack Shots") return CrackShots;
        else if (enumString == "Striders") return Striders;
        else if (enumString == "Stubborn") return Stubborn;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Maneaters::Init() {
        if (!s_registered) {
            static const std::array<int, 4> abilities = {Brawlers, CrackShots, Striders, Stubborn};
            static FactoryMethod factoryMethod = {
                    Maneaters::Create,
                    Maneaters::ValueToString,
                    Maneaters::EnumStringToInt,
                    Maneaters::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Ability", Brawlers, abilities),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Maneaters", factoryMethod);
        }
    }

    Maneaters::Maneaters() :
            MawtribesBase("Maneaters", 6, WOUNDS, 7, 5, false),
            m_pistolsOrStars(Weapon::Type::Missile, "Pistols or Throwing Stars", 12, 1, 3, 3, -1, RAND_D3),
            m_bashers(Weapon::Type::Melee, "Slicers and Bashers", 1, 4, 3, 3, -1, 2),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, MANEATERS};
        m_weapons = {&m_pistolsOrStars, &m_bite, &m_bashers};
    }

    bool Maneaters::configure(int numModels, Ability ability) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_ability = ability;

        if (m_ability == Striders)
            m_runAndCharge = true;

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_pistolsOrStars);
            model->addMeleeWeapon(&m_bashers);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int Maneaters::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    Rerolls Maneaters::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_ability == Brawlers) {
            if (!weapon->isMissile()) {
                return RerollOnes;
            } else if (m_ability == CrackShots) {
                if (weapon->isMissile()) {
                    return RerollOnes;
                }
            }
        } else {
            if (m_ability == CrackShots) {
                if (weapon->isMissile()) {
                    return RerollOnes;
                }
            }
        }
        return Unit::toHitRerolls(weapon, target);
    }

    bool Maneaters::battleshockRequired() const {
        if (m_ability == Stubborn) return false;
        return Unit::battleshockRequired();
    }

} // namespace OgorMawtribes
