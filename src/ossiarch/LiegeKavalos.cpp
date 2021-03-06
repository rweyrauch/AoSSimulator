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
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_kavalosCommandTraits[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_kavaloiArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new LiegeKavalos(legion, trait, artefact, general, lore);
    }

    void LiegeKavalos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LiegeKavalos::Create,
                    OssiarchBonereaperBase::ValueToString,
                    OssiarchBonereaperBase::EnumStringToInt,
                    LiegeKavalos::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Command Trait", g_kavalosCommandTraits[0], g_kavalosCommandTraits),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Artefact", g_kavaloiArtefacts[0], g_kavaloiArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Liege-Kavalos", factoryMethod);
        }
    }

    LiegeKavalos::LiegeKavalos(Legion legion, CommandTrait trait, Artefact artefact, bool isGeneral, Lore lore) :
            OssiarchBonereaperBase("Liege-Kavalos", 10, g_wounds, 10, 3, false, g_pointsPerUnit) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, LIEGE, HERO, LIEGE_KAVALOS};
        m_weapons = {&m_blade, &m_shield, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);

        setLegion(legion);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_shield);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        if (m_commandTrait == CommandTrait::Dark_Acolyte) {
            m_keywords.push_back(WIZARD);
            m_totalSpells = 1;
            m_totalUnbinds = 1;
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        }
    }

    int LiegeKavalos::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
