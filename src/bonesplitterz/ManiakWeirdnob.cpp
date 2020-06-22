/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "bonesplitterz/ManiakWeirdnob.h"
#include "BonesplitterzPrivate.h"

namespace Bonesplitterz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool ManiakWeirdnob::s_registered = false;

    Unit *ManiakWeirdnob::Create(const ParameterList &parameters) {
        auto unit = new ManiakWeirdnob();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_wizardCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_wizardArtefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ManiakWeirdnob::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_wizardCommandTraits[0], g_wizardCommandTraits),
                            EnumParameter("Artefact", g_wizardArtefacts[0], g_wizardArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore)
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Maniak Weirdnob", factoryMethod);
        }
    }

    ManiakWeirdnob::ManiakWeirdnob() :
            Bonesplitterz("Maniak Weirdnob", 12, WOUNDS, 7, 6, false),
            m_bonebeastStaff(Weapon::Type::Melee, "Bonebeast Staff", 1, 3, 4, 3, 0, RAND_D3),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, MANIAK_WEIRDNOB};
        m_weapons = {&m_bonebeastStaff, &m_tusksAndHooves};
        m_hasMount = true;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ManiakWeirdnob::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bonebeastStaff);
        model->addMeleeWeapon(&m_tusksAndHooves);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int ManiakWeirdnob::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Bonesplitterz
