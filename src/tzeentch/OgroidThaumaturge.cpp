/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/OgroidThaumaturge.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 160;

    bool OgroidThaumaturge::s_registered = false;

    Unit *OgroidThaumaturge::Create(const ParameterList &parameters) {
        auto unit = new OgroidThaumaturge();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int)ChangeCoven::None);
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

    void OgroidThaumaturge::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OgroidThaumaturge::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    OgroidThaumaturge::ComputePoints,
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
            s_registered = UnitFactory::Register("Ogroid Thaumaturge", factoryMethod);
        }
    }

    OgroidThaumaturge::OgroidThaumaturge() :
            TzeentchBase("OgroidT haumaturge", 6, g_wounds, 8, 5, false),
            m_staff(Weapon::Type::Melee, "Thaumaturge Staff", 2, 3, 3, 3, -1, RAND_D3),
            m_horns(Weapon::Type::Melee, "Great Horns", 1, 2, 3, 3, -2, 3),
            m_hooves(Weapon::Type::Melee, "Cloven Hooves", 1, 4, 4, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, HERO, WIZARD, OGROID_THAUMATURGE};
        m_weapons = {&m_staff, &m_horns, &m_hooves};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool OgroidThaumaturge::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_horns);
        model->addMeleeWeapon(&m_hooves);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int OgroidThaumaturge::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Tzeentch