/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/Vortemis.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool VortemisTheAllSeeing::s_registered = false;

    Unit *VortemisTheAllSeeing::Create(const ParameterList &parameters) {
        auto unit = new VortemisTheAllSeeing();

        unit->setChangeCoven(ChangeCoven::Cult_Of_The_Transient_Form);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);

        unit->configure(lore);
        return unit;
    }

    void VortemisTheAllSeeing::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    VortemisTheAllSeeing::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    VortemisTheAllSeeing::ComputePoints,
                    {
                            EnumParameter("Lore", g_loreOfFate[0], g_loreOfFate),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Vortemis the All-seeing", factoryMethod);
        }
    }

    VortemisTheAllSeeing::VortemisTheAllSeeing() :
            TzeentchBase("Vortemis the All-seeing", 6, g_wounds, 7, 5, false),
            m_staffMissile(Weapon::Type::Missile, "Tzeenchian Runestaff", 18, 1, 3, 4, 0, RAND_D3),
            m_staff(Weapon::Type::Melee, "Tzeenchian Runestaff", 1, 1, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, CULT_OF_THE_TRANSIENT_FORM, HERO, WIZARD, MAGISTER,
                      VORTEMIS_THE_ALL_SEEING};
        m_weapons = {&m_staffMissile, &m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    void VortemisTheAllSeeing::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_staffMissile);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int VortemisTheAllSeeing::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch