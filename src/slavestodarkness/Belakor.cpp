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

namespace SlavesToDarkness {

    static const int g_basesize = 50;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 240;

    bool Belakor::s_registered = false;

    Unit *Belakor::Create(const ParameterList &parameters) {
        auto unit = new Belakor();

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

    Belakor::Belakor() :
            SlavesToDarknessBase("Be'lakor", 12, g_wounds, 10, 4, true),
            m_blade(Weapon::Type::Melee, "Blade of Shadows", 1, 6, 3, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, SLAVES_TO_DARKNESS, UNDIVIDED, HERO, WIZARD, DAEMON_PRINCE, BELAKOR};
        m_weapons = {&m_blade};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    bool Belakor::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Enfeeble Foe", 6, 18, To_Wound_Melee, -1, Spell::Target::Enemy));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int Belakor::toSaveModifier(const Weapon *weapon) const {
        // Shadow Form - no save modifiers allowed.
        return 0;
    }

} // namespace SlavesToDarkness


