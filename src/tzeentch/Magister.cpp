/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Magister.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool Magister::s_registered = false;

    Unit *Magister::Create(const ParameterList &parameters) {
        auto unit = new Magister();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        unit->setChangeCoven(coven);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Magister::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Magister::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    Magister::ComputePoints,
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

            s_registered = UnitFactory::Register("Magister", factoryMethod);
        }
    }

    Magister::Magister() :
            TzeentchBase("Magister", 6, g_wounds, 7, 5, false),
            m_staff(Weapon::Type::Missile, "Tzeentchian Runestaff", 18, 1, 3, 4, 0, RAND_D3),
            m_sword(Weapon::Type::Melee, "Warpsteel Sword", 1, 1, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, MAGISTER};
        m_weapons = {&m_staff,
                     &m_sword};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool Magister::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_staff);
        model->addMeleeWeapon(&m_sword);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateBoltOfChange(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int Magister::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch