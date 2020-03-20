/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/KavalosZandtos.h"

namespace OssiarchBonereapers {
    static const int BASESIZE = 80;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 220;

    bool ArchKavalosZandtos::s_registered = false;

    Unit *ArchKavalosZandtos::Create(const ParameterList &parameters) {
        auto unit = new ArchKavalosZandtos();

        auto legion = (Legion) GetEnumParam("Legion", parameters, None);
        unit->setLegion(legion);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string ArchKavalosZandtos::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int ArchKavalosZandtos::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void ArchKavalosZandtos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ArchKavalosZandtos::Create,
                    ArchKavalosZandtos::ValueToString,
                    ArchKavalosZandtos::EnumStringToInt,
                    ArchKavalosZandtos::ComputePoints,
                    {
                            {ParamType::Enum, "Legion", OssiarchBonereaperBase::None, OssiarchBonereaperBase::None,
                             OssiarchBonereaperBase::Crematorians, 1},
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Arch-Kavalos Zandtos", factoryMethod);
        }
    }

    ArchKavalosZandtos::ArchKavalosZandtos() :
            OssiarchBonereaperBase("Arch-Kavalos Zandtos", 12, WOUNDS, 10, 3, false),
            m_lance(Weapon::Type::Melee, "The Dark Lance", 2, 3, 3, 3, -1, 2),
            m_shield(Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, LIEGE, HERO, ARCH_KAVALOS_ZANDTOS};
        m_weapons = {&m_lance, &m_shield, &m_hoovesAndTeeth};
    }

    bool ArchKavalosZandtos::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_lance);
        model->addMeleeWeapon(&m_shield);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds
    ArchKavalosZandtos::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // The Dark Lance
        if (m_charged && (weapon->name() == m_lance.name())) {
            return {3, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls ArchKavalosZandtos::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Hatred of the Living
        if (target->hasKeyword(ORDER) || target->hasKeyword(DESTRUCTION)) {
            return RerollOnes;
        } else if (target->hasKeyword(CHAOS)) {
            return RerollFailed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int ArchKavalosZandtos::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace OssiarchBonereapers
