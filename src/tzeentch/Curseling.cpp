/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Curseling.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 160;

    bool CurselingEyeOfTzeentch::s_registered = false;

    Unit *CurselingEyeOfTzeentch::Create(const ParameterList &parameters) {
        auto unit = new CurselingEyeOfTzeentch();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void CurselingEyeOfTzeentch::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    CurselingEyeOfTzeentch::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    CurselingEyeOfTzeentch::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_arcaniteCommandTraits[0], g_arcaniteCommandTraits),
                            EnumParameter("Artefact", g_arcaniteArtefacts[0], g_arcaniteArtefacts),
                            EnumParameter("Lore", g_loreOfFate[0], g_loreOfFate),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };

            s_registered = UnitFactory::Register("Curseling Eye of Tzeentch", factoryMethod);
        }
    }

    CurselingEyeOfTzeentch::CurselingEyeOfTzeentch() :
            TzeentchBase("Curseling Eye of Tzeentch", 5, g_wounds, 7, 4, false),
            m_sword(Weapon::Type::Melee, "Blazing Sword", 1, 3, 3, 4, -1, 1),
            m_flail(Weapon::Type::Melee, "Threshing Flail", 1, 3, 4, 3, 0, 1),
            m_staff(Weapon::Type::Melee, "Staff of Tzeentch", 2, 1, 5, 4, 0, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, CURSELING};
        m_weapons = {&m_sword, &m_flail, &m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool CurselingEyeOfTzeentch::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_sword);
        model->addMeleeWeapon(&m_flail);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int CurselingEyeOfTzeentch::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch