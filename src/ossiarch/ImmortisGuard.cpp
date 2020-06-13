/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/ImmortisGuard.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 200;
    static const int POINTS_MAX_UNIT_SIZE = 800;

    bool ImmortisGuard::s_registered = false;

    Unit *ImmortisGuard::Create(const ParameterList &parameters) {
        auto unit = new ImmortisGuard();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ImmortisGuard::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int ImmortisGuard::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void ImmortisGuard::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ImmortisGuard::Create,
                    ImmortisGuard::ValueToString,
                    ImmortisGuard::EnumStringToInt,
                    ImmortisGuard::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Immortis Guard", factoryMethod);
        }
    }

    ImmortisGuard::ImmortisGuard() :
            OssiarchBonereaperBase("Immortis Guard", 5, WOUNDS, 10, 3, false),
            m_halberd(Weapon::Type::Melee, "Dread Halberd", 2, 2, 3, 3, -2, 2),
            m_battleShield(Weapon::Type::Melee, "Battle Shield", 1, 2, 4, 3, 0, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, HEKATOS, IMMORTIS_GUARD};
        m_weapons = {&m_halberd, &m_battleShield};
    }

    bool ImmortisGuard::configure(int numModels) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_halberd);
            model->addMeleeWeapon(&m_battleShield);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds ImmortisGuard::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deadly Combination
        if ((hitRoll == 6) && (weapon->name() == m_battleShield.name())) {
            return {weapon->damage(), 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ImmortisGuard::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace OssiarchBonereapers
