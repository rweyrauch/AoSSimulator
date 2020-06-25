/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeer.h>
#include <UnitFactory.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 140;

    bool GreySeer::s_registered = false;

    Unit *GreySeer::Create(const ParameterList &parameters) {
        auto unit = new GreySeer();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterClanCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_masterClanArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greySeerLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GreySeer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_masterClanCommandTraits[0], g_masterClanCommandTraits),
                            EnumParameter("Artefact", g_masterClanArtefacts[0], g_masterClanArtefacts),
                            EnumParameter("Lore", g_greySeerLore[0], g_greySeerLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Grey Seer", factoryMethod);
        }
    }

    GreySeer::GreySeer() :
            Skaventide("Grey Seer", 6, WOUNDS, 6, 5, false),
            m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, HERO, WIZARD, GREY_SEER};
        m_weapons = {&m_staff};

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool GreySeer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int GreySeer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Skaven
