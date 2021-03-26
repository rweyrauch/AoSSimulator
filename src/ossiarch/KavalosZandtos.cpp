/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/KavalosZandtos.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 80;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 220;

    bool ArchKavalosZandtos::s_registered = false;

    Unit *ArchKavalosZandtos::Create(const ParameterList &parameters) {
        auto unit = new ArchKavalosZandtos();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
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
                            EnumParameter("Legion", g_legion[0], g_legion),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Arch-Kavalos Zandtos", factoryMethod);
        }
    }

    ArchKavalosZandtos::ArchKavalosZandtos() :
            OssiarchBonereaperBase("Arch-Kavalos Zandtos", 12, g_wounds, 10, 3, false),
            m_lance(Weapon::Type::Melee, "The Dark Lance", 2, 3, 3, 3, -1, 2),
            m_shield(Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, LIEGE, HERO, ARCH_KAVALOS_ZANDTOS};
        m_weapons = {&m_lance, &m_shield, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
    }

    bool ArchKavalosZandtos::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_lance);
        model->addMeleeWeapon(&m_shield);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_points = g_pointsPerUnit;

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
            return Rerolls::Ones;
        } else if (target->hasKeyword(CHAOS)) {
            return Rerolls::Failed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int ArchKavalosZandtos::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
