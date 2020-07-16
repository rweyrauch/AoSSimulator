/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nighthaunt/GuardianOfSouls.h"
#include "NighthauntPrivate.h"

namespace Nighthaunt {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 130;

    bool GuardianOfSouls::s_registered = false;

    Unit *GuardianOfSouls::Create(const ParameterList &parameters) {
        auto unit = new GuardianOfSouls();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
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

    void GuardianOfSouls::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GuardianOfSouls::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    GuardianOfSouls::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Guardian of Souls with Nightmare Lantern", factoryMethod);
        }
    }

    GuardianOfSouls::GuardianOfSouls() :
            Nighthaunt("Guardian of Souls with Nightmare Lantern", 6, WOUNDS, 10, 4, true),
            m_blade(Weapon::Type::Melee, "Chill Blade", 1, 3, 3, 3, -1, 1),
            m_maul(Weapon::Type::Melee, "Maul of Judgement", 1, 2, 3, 3, 0, 2) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, GUARDIAN_OF_SOULS};
        m_weapons = {&m_blade, &m_maul};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool GuardianOfSouls::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_maul);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    int GuardianOfSouls::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nighthaunt
