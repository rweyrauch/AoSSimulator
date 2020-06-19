/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nurgle/SorcererOfNurgle.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 110;

    bool SorcererOfNurgle::s_registered = false;

    Unit *SorcererOfNurgle::Create(const ParameterList &parameters) {
        auto unit = new SorcererOfNurgle();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalRotbringerCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalRotbringerArtefacts[0]);
        unit->setArtefact(artefact);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SorcererOfNurgle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SorcererOfNurgle::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    SorcererOfNurgle::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_mortalRotbringerCommandTraits[0], g_mortalRotbringerCommandTraits),
                            EnumParameter("Artefact", g_mortalRotbringerArtefacts[0], g_mortalRotbringerArtefacts),
                            EnumParameter("Lore", g_mortalRotbringerLore[0], g_mortalRotbringerLore)
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Sorcerer of Nurgle", factoryMethod);
        }
    }

    SorcererOfNurgle::SorcererOfNurgle() :
            NurgleBase("SorcererOfNurgle", 4, WOUNDS, 7, 5, false),
            m_staff(Weapon::Type::Melee, "Rotwood Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, HERO, WIZARD, SORCERER};
        m_weapons = {&m_staff};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool SorcererOfNurgle::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int SorcererOfNurgle::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Nurgle
