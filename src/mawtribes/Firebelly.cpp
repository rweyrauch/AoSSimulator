/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "mawtribes/Firebelly.h"
#include "MawtribesPrivate.h"
#include "MawtribesLore.h"

namespace OgorMawtribes {
    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 120;

    bool Firebelly::s_registered = false;

    Unit *Firebelly::Create(const ParameterList &parameters) {
        auto unit = new Firebelly();

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_firebellyLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }


    void Firebelly::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Firebelly::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    Firebelly::ComputePoints,
                    {
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Lore", g_firebellyLore[0], g_firebellyLore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Firebelly", factoryMethod);
        }
    }

    Firebelly::Firebelly() :
            MawtribesBase("Firebelly", 6, g_wounds, 6, 5, false),
            m_fireBreath(Weapon::Type::Missile, "Fire Breath", 6, 1, 0, 0, 0, 0),
            m_hammer(Weapon::Type::Melee, "Basalt Hammer", 2, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, HERO, WIZARD, FIREBELLY};
        m_weapons = {&m_fireBreath, &m_hammer};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Firebelly::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());

        model->addMissileWeapon(&m_fireBreath);
        model->addMeleeWeapon(&m_hammer);

        addModel(model);

        //m_knownSpells.push_back(std::make_unique<CascadingFireCloak>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = Firebelly::ComputePoints(1);

        return true;
    }

    void Firebelly::onStartShooting(PlayerId player) {
        MawtribesBase::onStartShooting(player);

        // Fire Breath
        if (player == owningPlayer() && m_shootingTarget) {
            if (distanceTo(m_shootingTarget) <= (double) m_fireBreath.range()) {
                if (Dice::RollD6() >= 4) {
                    m_shootingTarget->applyDamage({0, Dice::RollD3()}, this);
                }
            }
        }
    }

    int Firebelly::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OgorMawtribes
