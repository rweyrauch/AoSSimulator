/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "ironjawz/WeirdnobShaman.h"
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int BASESIZE = 40;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    bool OrrukWeirdnobShaman::s_registered = false;

    Unit *OrrukWeirdnobShaman::Create(const ParameterList &parameters) {
        auto unit = new OrrukWeirdnobShaman();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_shamanCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_shamanArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int OrrukWeirdnobShaman::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void OrrukWeirdnobShaman::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukWeirdnobShaman::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukWeirdnobShaman::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_shamanCommandTraits[0], g_shamanCommandTraits),
                            EnumParameter("Artefact", g_shamanArtefacts[0], g_shamanArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Weirdnob Shaman", factoryMethod);
        }
    }

    OrrukWeirdnobShaman::OrrukWeirdnobShaman() :
            Ironjawz("Orruk Weirdnob Shaman", 4, WOUNDS, 6, 5, false),
            m_staff(Weapon::Type::Melee, "Waaagh! Staff", 1, 3, 4, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, WIZARD, WEIRDNOB_SHAMAN};
        m_weapons = {&m_staff};
    }

    bool OrrukWeirdnobShaman::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace Ironjawz