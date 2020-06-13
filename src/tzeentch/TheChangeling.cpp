/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/TheChangeling.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 120;

    bool TheChangeling::s_registered = false;

    Unit *TheChangeling::Create(const ParameterList &parameters) {
        auto unit = new TheChangeling();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int)ChangeCoven::None);
        unit->setChangeCoven(coven);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TheChangeling::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheChangeling::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TheChangeling::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange)
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("The Changeling", factoryMethod);
        }
    }

    TheChangeling::TheChangeling() :
            TzeentchBase("The Changeling", 6, WOUNDS, 7, 5, false),
            m_staff(Weapon::Type::Melee, "The Trickster's Staff", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, THE_CHANGELING};
        m_weapons = {&m_staff};

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool TheChangeling::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int TheChangeling::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // Tzeentch