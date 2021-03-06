/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "slavestodarkness/TheddraSkullScryer.h"
#include "SlavesToDarknessPrivate.h"
#include "StDSpells.h"

namespace SlavesToDarkness {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 70;

    bool TheddraSkullscryer::s_registered = false;

    Unit *TheddraSkullscryer::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        return new TheddraSkullscryer(legion, lore, general);
    }

    int TheddraSkullscryer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void TheddraSkullscryer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheddraSkullscryer::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    TheddraSkullscryer::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS}
            };
            s_registered = UnitFactory::Register("Theddra Skull-scryer", factoryMethod);
        }
    }

    TheddraSkullscryer::TheddraSkullscryer(DamnedLegion legion, Lore lore, bool isGeneral) :
            SlavesToDarknessBase(legion, "Theddra Skull-scryer", 6, g_wounds, 7, 5, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, SLAVES_TO_DARKNESS, HERO, WIZARD, GOD_SPEAKER, THEDDRA_SKULL_SCRYER};
        m_weapons = {&m_wand};
        m_battleFieldRole = Role::Leader;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_wand);
        addModel(model);

        m_knownSpells.push_back(
                std::make_unique<BuffModifierSpell>(this, "Enfeeblement", 6, 12, Attribute::To_Wound_Melee, -1,
                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Rerolls TheddraSkullscryer::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Pact of Soul and Iron
        return Rerolls::Failed;
    }

    Rerolls TheddraSkullscryer::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Pact of Soul and Iron
        if (target->hasKeyword(STORMCAST_ETERNAL)) return Rerolls::Failed;
        return SlavesToDarknessBase::toWoundRerolls(weapon, target);
    }

} // namespace SlavesToDarkness


