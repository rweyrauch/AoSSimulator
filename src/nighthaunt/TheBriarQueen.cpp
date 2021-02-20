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
        auto unit = new TheBriarQueen();

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

    void TheBriarQueen::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TheBriarQueen::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    TheBriarQueen::ComputePoints,
                    {
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("The Briar Queen", factoryMethod);
        }
    }

    TheBriarQueen::TheBriarQueen() :
            Nighthaunt("The Briar Queen", 6, g_wounds, 10, 4, true),
            m_scream(Weapon::Type::Missile, "Rending Scream", 10, 3, 3, 3, -3, 1),
            m_whip(Weapon::Type::Melee, "Briar Whip", 3, 1, 3, 3, 2, RAND_D3) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, MIRRORGHAST_BANSHEE, THE_BRIAR_QUEEN};
        m_weapons = {&m_scream, &m_whip};
        m_battleFieldRole = Leader;

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool TheBriarQueen::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_scream);
        model->addMeleeWeapon(&m_whip);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

    int TheBriarQueen::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
