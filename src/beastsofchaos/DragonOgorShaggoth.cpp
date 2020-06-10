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

namespace BeastsOfChaos {
    static const int BASESIZE = 90; // x52 oval;
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 180;

    bool DragonOgorShaggoth::s_registered = false;

    DragonOgorShaggoth::DragonOgorShaggoth() :
            BeastsOfChaosBase("Dragon Ogor Shaggoth", 8, WOUNDS, 7, 4, false),
            m_stormWroughtAxe(Weapon::Type::Melee, "Storm-wrought Axe", 2, 3, 3, 3, -1, 3),
            m_sweepingTail(Weapon::Type::Melee, "Sweeping Tail", 3, RAND_D3, 4, 3, 0, 1),
            m_talonedForelimbs(Weapon::Type::Melee, "Taloned Forelimbs", 1, 2, 3, 3, -1, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, THUNDERSCORN, DRAGON_OGOR, MONSTER, HERO, WIZARD, SHAGGOTH};
        m_weapons = {&m_stormWroughtAxe, &m_sweepingTail, &m_talonedForelimbs};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool DragonOgorShaggoth::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_stormWroughtAxe);
        model->addMeleeWeapon(&m_sweepingTail);
        model->addMeleeWeapon(&m_talonedForelimbs);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *DragonOgorShaggoth::Create(const ParameterList &parameters) {
        auto unit = new DragonOgorShaggoth();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure();
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
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Dragon Ogor Shaggoth", factoryMethod);
        }
    }

    int DragonOgorShaggoth::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace BeastsOfChaos
