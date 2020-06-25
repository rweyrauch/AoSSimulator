/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nurgle/BloabRotspawned.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int BASESIZE = 100;
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 240;

    bool BloabRotspawned::s_registered = false;

    Unit *BloabRotspawned::Create(const ParameterList &parameters) {
        auto unit = new BloabRotspawned();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void BloabRotspawned::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BloabRotspawned::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    BloabRotspawned::ComputePoints,
                    {
                        EnumParameter("Lore", g_mortalRotbringerLore[0], g_mortalRotbringerLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Bloab Rotspawned", factoryMethod);
        }
    }

    BloabRotspawned::BloabRotspawned() :
            NurgleBase("Bloab Rotspawned", 10, WOUNDS, 9, 4, false),
            m_bile(Weapon::Type::Missile, "Bilespurter's Vile Bile", 12, RAND_D3, 4, 2, -2, RAND_D3),
            m_scythe(Weapon::Type::Melee, "Harvestman's Scythe", 2, 3, 3, 3, -1, 2),
            m_claws(Weapon::Type::Melee, "Bilespurter's Monstrous Claws", 3, 5, 4, 2, -1, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, WIZARD, BLOAB_ROTSPAWNED};
        m_weapons = {&m_bile, &m_scythe, &m_claws};

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        s_globalCastMod.connect(this, &BloabRotspawned::windspeakerBellsCastingMod, &m_windspeakerSlot);
    }

    BloabRotspawned::~BloabRotspawned() {
        m_windspeakerSlot.disconnect();
    }

    bool BloabRotspawned::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_bile);
        model->addMeleeWeapon(&m_scythe);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int BloabRotspawned::windspeakerBellsCastingMod(const Unit *caster) {
        // Windspeaker Bells
        if (!isFriendly(caster) && caster->hasKeyword(WIZARD) && (distanceTo(caster) <= 14.0)) {
            return -1;
        }
        return 0;
    }

    int BloabRotspawned::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle
