/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/BlueScribes.h"
#include "TzeentchPrivate.h"

namespace Tzeentch {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 0;

    bool TheBlueScribes::s_registered = false;

    Unit *TheBlueScribes::Create(const ParameterList &parameters) {
        auto unit = new TheBlueScribes();

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

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

    int TheBlueScribes::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void TheBlueScribes::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheBlueScribes::Create,
                    TzeentchBase::ValueToString,
                    TzeentchBase::EnumStringToInt,
                    TheBlueScribes::ComputePoints,
                    {
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("The Blue Scribes", factoryMethod);
        }
    }

    TheBlueScribes::TheBlueScribes() :
            TzeentchBase("The Blue Scribes", 16, g_wounds, 10, 5, true),
            m_quills(Weapon::Type::Melee, "Sharpened Quills", 1, 2, 5, 5, 0, 1),
            m_teethAndHorns(Weapon::Type::Melee, "Teeth and Horns", 1, RAND_D3, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, HORROR, TZEENTCH, HERO, WIZARD, THE_BLUE_SCRIBES};
        m_weapons = {&m_quills, &m_teethAndHorns};
        m_hasMount = true;
        m_battleFieldRole = Leader;
        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool TheBlueScribes::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_quills);
        model->addMeleeWeapon(&m_teethAndHorns);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

} // namespace Tzeentch


