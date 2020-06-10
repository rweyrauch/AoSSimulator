/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <dispossessed/Unforged.h>
#include <UnitFactory.h>

namespace Dispossessed {
    static const int BASESIZE = 25;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool Unforged::s_registered = false;

    Unforged::Unforged() :
            Dispossessed("Unforged", 4, WOUNDS, 10, 6, false),
            m_runicAxes(Weapon::Type::Melee, "Runic Axes", 1, 6, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, DISPOSSESSED, HERO, UNFORGED};
        m_weapons = {&m_runicAxes};
    }

    bool Unforged::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_runicAxes);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Unforged::Create(const ParameterList &parameters) {
        auto unit = new Unforged();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Unforged::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Unforged::Create,
                    Dispossessed::ValueToString,
                    Dispossessed::EnumStringToInt,
                    Unforged::ComputePoints,
                    {
                            EnumParameter("Grudge", g_grudge[0], g_grudge)
                    },
                    ORDER,
                    {DISPOSSESSED}
            };
            s_registered = UnitFactory::Register("Unforged", factoryMethod);
        }
    }

    Rerolls Unforged::toHitRerolls(const Weapon * /*weapon*/, const Unit * /*target*/) const {
        // Runic Axes
        return RerollOnes;
    }

    Wounds Unforged::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Nemesis
        if (target->hasKeyword(CHAOS)) {
            return {weapon->damage() * 2, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int Unforged::toWoundModifier(const Weapon *weapon, const Unit *target) const {
        auto modifier = Unit::toWoundModifier(weapon, target);

        // The Bigger They Are...
        if (target->wounds() > 1) {
            modifier += 1;
        }
        return modifier;
    }

    int Unforged::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Dispossessed