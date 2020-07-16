/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukMegaboss.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int BASESIZE = 60;
    static const int WOUNDS = 7;
    static const int POINTS_PER_UNIT = 140;

    bool OrrukMegaboss::s_registered = false;

    OrrukMegaboss::OrrukMegaboss() :
            Ironjawz("Orruk Megaboss", 4, WOUNDS, 8, 3, false),
            m_bossChoppaAndFist(Weapon::Type::Melee, "Boss Choppa and Rip-toof Fist", 1, 6, 3, 3, -1, 2) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, HERO, MEGABOSS};
        m_weapons = {&m_bossChoppaAndFist};
        m_battleFieldRole = Leader;
    }

    bool OrrukMegaboss::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bossChoppaAndFist);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *OrrukMegaboss::Create(const ParameterList &parameters) {
        auto unit = new OrrukMegaboss();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_bossCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_bossArtefacts[0]);
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

    void OrrukMegaboss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    OrrukMegaboss::Create,
                    Ironjawz::ValueToString,
                    Ironjawz::EnumStringToInt,
                    OrrukMegaboss::ComputePoints,
                    {
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                            EnumParameter("Command Trait", g_bossCommandTraits[0], g_bossCommandTraits),
                            EnumParameter("Artefact", g_bossArtefacts[0], g_bossArtefacts),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Orruk Megaboss", factoryMethod);
        }
    }

    Wounds OrrukMegaboss::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        auto wounds = Ironjawz::computeReturnedDamage(weapon, saveRoll);
        // Rip-toof Fist
        if (saveRoll == 6) {
            wounds += {0, 1};
        }
        return wounds;
    }

    void OrrukMegaboss::onStartCombat(PlayerId player) {
        m_modelsSlainAtStartOfCombat = m_currentRecord.m_enemyModelsSlain;

        Ironjawz::onStartCombat(player);
    }

    Wounds OrrukMegaboss::onEndCombat(PlayerId player) {
        // Strength from Victory
        if (m_currentRecord.m_enemyModelsSlain > m_modelsSlainAtStartOfCombat) {
            heal(1);
            m_bossChoppaAndFist.setAttacks(m_bossChoppaAndFist.attacks() + 1);
        }
        return Ironjawz::onEndCombat(player);
    }

    int OrrukMegaboss::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace Ironjawz