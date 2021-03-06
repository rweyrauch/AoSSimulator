/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nighthaunt/TheBriarQueen.h"
#include "NighthauntPrivate.h"
#include "NighthauntLore.h"

namespace Nighthaunt {
    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 75;

    bool TheBriarQueen::s_registered = false;

    Unit *TheBriarQueen::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new TheBriarQueen(procession, lore, general);
    }

    void TheBriarQueen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheBriarQueen::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    TheBriarQueen::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("The Briar Queen", factoryMethod);
        }
    }

    TheBriarQueen::TheBriarQueen(Procession procession, Lore lore, bool isGeneral) :
            Nighthaunt(procession, "The Briar Queen", 6, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_scream(Weapon::Type::Missile, "Rending Scream", 10, 3, 3, 3, -3, 1),
            m_whip(Weapon::Type::Melee, "Briar Whip", 3, 1, 3, 3, 2, RAND_D3) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, MIRRORGHAST_BANSHEE, THE_BRIAR_QUEEN};
        m_weapons = {&m_scream, &m_whip};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_scream);
        model->addMeleeWeapon(&m_whip);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int TheBriarQueen::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
