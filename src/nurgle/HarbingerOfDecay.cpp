/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include "nurgle/HarbingerOfDecay.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int BASESIZE = 75; // x42 oval
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 160;

    bool HarbingerOfDecay::s_registered = false;

    Unit *HarbingerOfDecay::Create(const ParameterList &parameters) {
        auto unit = new HarbingerOfDecay();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HarbingerOfDecay::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HarbingerOfDecay::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    HarbingerOfDecay::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Harbinger of Decay", factoryMethod);
        }
    }

    HarbingerOfDecay::HarbingerOfDecay() :
            NurgleBase("Harbinger of Decay", 4, WOUNDS, 8, 4, false),
            m_scythe(Weapon::Type::Melee, "Plague Scythe", 1, 3, 3, 3, -1, RAND_D3),
            m_bite(Weapon::Type::Melee, "Daemonic Mount's Flyblown Bite", 1, RAND_D6, 4, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, DAEMON, NURGLE, ROTBRINGER, HERO, HARBINGER_OF_DECAY};
        m_weapons = {&m_scythe, &m_bite};
    }

    bool HarbingerOfDecay::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_scythe);
        model->addMeleeWeapon(&m_bite);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds HarbingerOfDecay::applyWoundSave(const Wounds &wounds) {
        // Soulbound Shield
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(4);
        totalWounds.mortal -= mortalSaves.rollsGE(4);

        return totalWounds.clamp();
    }

    int HarbingerOfDecay::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
