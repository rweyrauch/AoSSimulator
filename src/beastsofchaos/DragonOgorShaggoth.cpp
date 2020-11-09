/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/DragonOgorShaggoth.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int g_basesize = 90; // x52 oval;
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 170;

    bool DragonOgorShaggoth::s_registered = false;

    DragonOgorShaggoth::DragonOgorShaggoth() :
            BeastsOfChaosBase("Dragon Ogor Shaggoth", 8, g_wounds, 7, 4, false),
            m_stormWroughtAxe(Weapon::Type::Melee, "Storm-wrought Axe", 2, 3, 3, 3, -1, 3),
            m_sweepingTail(Weapon::Type::Melee, "Sweeping Tail", 3, RAND_D3, 4, 3, 0, 1),
            m_talonedForelimbs(Weapon::Type::Melee, "Taloned Forelimbs", 1, 2, 3, 3, -1, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, THUNDERSCORN, DRAGON_OGOR, MONSTER, HERO, WIZARD, SHAGGOTH};
        m_weapons = {&m_stormWroughtAxe, &m_sweepingTail, &m_talonedForelimbs};
        m_battleFieldRole = Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool DragonOgorShaggoth::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_stormWroughtAxe);
        model->addMeleeWeapon(&m_sweepingTail);
        model->addMeleeWeapon(&m_talonedForelimbs);
        addModel(model);

        m_lore = lore;

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *DragonOgorShaggoth::Create(const ParameterList &parameters) {
        auto unit = new DragonOgorShaggoth();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_thunderscornCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_thunderscornArtefact[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDarkStorms[0]);
        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DragonOgorShaggoth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                            EnumParameter("Command Trait", g_thunderscornCommandTrait[0], g_thunderscornCommandTrait),
                            EnumParameter("Artefact", g_thunderscornArtefact[0], g_thunderscornArtefact),
                            EnumParameter("Lore", g_loreOfTheDarkStorms[0], g_loreOfTheDarkStorms),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Dragon Ogor Shaggoth", factoryMethod);
        }
    }

    int DragonOgorShaggoth::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace BeastsOfChaos
