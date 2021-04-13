/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "slavestodarkness/Belakor.h"
#include "SlavesToDarknessPrivate.h"
#include "StDSpells.h"

namespace SlavesToDarkness {

    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 240;

    bool Belakor::s_registered = false;

    Unit *Belakor::Create(const ParameterList &parameters) {
        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        auto mark = (MarkOfChaos) GetEnumParam("Mark of Chaos", parameters, g_markOfChaos[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        return new Belakor(legion, mark, lore, general);
    }

    int Belakor::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void Belakor::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Belakor::Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    Belakor::ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Mark of Chaos", g_markOfChaos[0], g_markOfChaos),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, SLAANESH, NURGLE}
            };
            s_registered = UnitFactory::Register("Be'lakor", factoryMethod);
        }
    }

    Belakor::Belakor(DamnedLegion legion, MarkOfChaos mark, Lore lore, bool isGeneral) :
            SlavesToDarknessBase("Be'lakor", 12, g_wounds, 10, 4, true) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, UNDIVIDED, HERO, WIZARD, DAEMON_PRINCE, BELAKOR};
        m_weapons = {&m_blade};
        m_battleFieldRole = Role::Leader;

        setDamnedLegion(legion);
        setMarkOfChaos(mark);
        setGeneral(isGeneral);

        m_totalUnbinds = 2;
        m_totalSpells = 2;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Enfeeble Foe", 6, 18, Attribute::To_Wound_Melee, -1,
                                                                    Abilities::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int Belakor::toSaveModifier(const Weapon *weapon, const Unit *attacker) const {
        // Shadow Form - no save modifiers allowed.
        return 0;
    }

} // namespace SlavesToDarkness


