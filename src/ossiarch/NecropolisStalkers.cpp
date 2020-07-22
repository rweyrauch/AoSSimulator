/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/NecropolisStalkers.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 6;
    static const int POINTS_PER_BLOCK = 180;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 3;

    bool NecropolisStalkers::s_registered = false;

    Unit *NecropolisStalkers::Create(const ParameterList &parameters) {
        auto unit = new NecropolisStalkers();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        int numFalchions = GetIntParam("Dread Falchions", parameters, 1);

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        bool ok = unit->configure(numModels, numFalchions);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string NecropolisStalkers::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int NecropolisStalkers::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void NecropolisStalkers::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    NecropolisStalkers::Create,
                    NecropolisStalkers::ValueToString,
                    NecropolisStalkers::EnumStringToInt,
                    NecropolisStalkers::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            IntegerParameter("Dread Falchions", 1, 0, MAX_UNIT_SIZE / 3, 1),
                            EnumParameter("Legion", g_legion[0], g_legion),
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Necropolis Stalkers", factoryMethod);
        }
    }

    NecropolisStalkers::NecropolisStalkers() :
            OssiarchBonereaperBase("Necropolis Stalkers", 6, WOUNDS, 10, 4, false),
            m_falchions(Weapon::Type::Melee, "Dread Falchions", 1, 3, 4, 3, -2, 2),
            m_blades(Weapon::Type::Melee, "Spirit Blades", 1, 5, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, HEKATOS, NECROPOLIS_STALKERS};
        m_weapons = {&m_falchions, &m_blades};
    }

    bool NecropolisStalkers::configure(int numModels, int numFalchions) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }
        const int maxFalchions = numModels / 3;
        if (numFalchions > maxFalchions) {
            // Invalid weapon configuration.
            return false;
        }

        for (auto i = 0; i < numFalchions; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_falchions);
            addModel(model);
        }
        for (auto i = numFalchions; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_blades);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    void NecropolisStalkers::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Select active aspect (randomly for now)
        m_activeAspect = (Aspect) Dice::rollD4();
    }

    Rerolls NecropolisStalkers::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_activeAspect == BladeStrike) return RerollFailed;
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls NecropolisStalkers::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        if (m_activeAspect == Destroyer) return RerollFailed;
        return Unit::toWoundRerolls(weapon, target);
    }

    Rerolls NecropolisStalkers::toSaveRerolls(const Weapon *weapon) const {
        if (m_activeAspect == BladeParry) return RerollFailed;
        return Unit::toSaveRerolls(weapon);
    }

    Wounds
    NecropolisStalkers::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto damage = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        if (m_activeAspect == Precision) damage.normal++;
        return damage;
    }

    int NecropolisStalkers::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto rend = Unit::weaponRend(weapon, target, hitRoll, woundRoll);
        if (m_activeAspect == Precision) rend--;
        return rend;
    }

    int NecropolisStalkers::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace OssiarchBonereapers
