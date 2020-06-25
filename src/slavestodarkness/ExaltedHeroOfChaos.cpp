/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "slavestodarkness/ExaltedHeroOfChaos.h"
#include "SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    bool ExaltedHeroOfChaos::s_registered = false;

    Unit *ExaltedHeroOfChaos::Create(const ParameterList &parameters) {
        auto unit = new ExaltedHeroOfChaos();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        unit->setMarkOfChaos(mark);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ExaltedHeroOfChaos::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ExaltedHeroOfChaos::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ExaltedHeroOfChaos::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Exalted Hero of Chaos", factoryMethod);
        }
    }

    ExaltedHeroOfChaos::ExaltedHeroOfChaos() :
            SlavesToDarknessBase("Exalted Hero of Chaos", 5, WOUNDS, 8, 4, false),
            m_blades(Weapon::Type::Melee, "Rune-etched Blades", 1, RAND_D6, 3, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, MARK_OF_CHAOS, EYE_OF_THE_GODS, HERO, EXALTED_HERO_OF_CHAOS};
        m_weapons = {&m_blades};
    }

    bool ExaltedHeroOfChaos::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Wounds ExaltedHeroOfChaos::applyWoundSave(const Wounds &wounds) {
        // Dark Blessings
        Dice::RollResult mortalSaves;
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.mortal -= mortalSaves.rollsGE(5);
        totalWounds.mortal = std::max(totalWounds.mortal, 0);

        return totalWounds;
    }

    int ExaltedHeroOfChaos::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Glory-hungry Bladesman
        if (target->hasKeyword(HERO) || target->hasKeyword(MONSTER)) mod++;

        return mod;
    }

    int ExaltedHeroOfChaos::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} //namespace SlavesToDarkness
