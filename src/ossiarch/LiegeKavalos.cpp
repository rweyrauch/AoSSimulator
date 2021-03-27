/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ossiarch/LiegeKavalos.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {
    static const int g_basesize = 80;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 200;

    bool LiegeKavalos::s_registered = false;

    Unit *LiegeKavalos::Create(const ParameterList &parameters) {
        auto unit = new LiegeKavalos();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_kavalosCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_kavaloiArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->configure();
        return unit;
    }

    std::string LiegeKavalos::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int LiegeKavalos::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void LiegeKavalos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LiegeKavalos::Create,
                    LiegeKavalos::ValueToString,
                    LiegeKavalos::EnumStringToInt,
                    LiegeKavalos::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Command Trait", g_kavalosCommandTraits[0], g_kavalosCommandTraits),
                            EnumParameter("Artefact", g_kavaloiArtefacts[0], g_kavaloiArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Liege-Kavalos", factoryMethod);
        }
    }

    LiegeKavalos::LiegeKavalos() :
            OssiarchBonereaperBase("Liege-Kavalos", 10, g_wounds, 10, 3, false),
            m_blade(Weapon::Type::Melee, "Commander's Blade", 1, 3, 3, 3, -1, 2),
            m_shield(Weapon::Type::Melee, "Nadirite Battle-shield", 1, 1, 3, 4, 0, 1),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Hooves and Teeth", 1, 6, 3, 3, -1, 1) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, LIEGE, HERO, LIEGE_KAVALOS};
        m_weapons = {&m_blade, &m_shield, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
    }

    void LiegeKavalos::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_shield);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    int LiegeKavalos::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
