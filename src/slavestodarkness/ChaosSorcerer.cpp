/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "slavestodarkness/ChaosSorcerer.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 110;

    bool ChaosSorcerer::s_registered = false;

    Unit *ChaosSorcerer::Create(const ParameterList &parameters) {
        auto unit = new ChaosSorcerer();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

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

    void ChaosSorcerer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ChaosSorcerer::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ChaosSorcerer::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Chaos Sorcerer Lord", factoryMethod);
        }
    }

    ChaosSorcerer::ChaosSorcerer() :
            SlavesToDarknessBase("Chaos Sorcerer Lord", 5, WOUNDS, 7, 4, false),
            m_staff(Weapon::Type::Melee, "Sorcerous Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_blade(Weapon::Type::Melee, "Chaos Runeblade", 1, 2, 3, 3, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, WIZARD,
                      CHAOS_SORCERER_LORD};
        m_weapons = {&m_staff, &m_blade};

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool ChaosSorcerer::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int ChaosSorcerer::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace SlavesToDarkness
