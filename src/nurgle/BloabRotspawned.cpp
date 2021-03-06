/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "nurgle/BloabRotspawned.h"
#include "NurglePrivate.h"

namespace Nurgle {
    static const int g_basesize = 100;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 240;

    bool BloabRotspawned::s_registered = false;

    Unit *BloabRotspawned::Create(const ParameterList &parameters) {
        auto legion = (PlagueLegion) GetEnumParam("Plague Legion", parameters, (int) PlagueLegion::None);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_mortalRotbringerLore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new BloabRotspawned(legion, lore, general);
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
                            BoolParameter("General"),
                            EnumParameter("Plague Legion", g_plagueLegions[0], g_plagueLegions),
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Bloab Rotspawned", factoryMethod);
        }
    }

    BloabRotspawned::BloabRotspawned(PlagueLegion legion, Lore lore, bool isGeneral) :
            NurgleBase(legion, "Bloab Rotspawned", 10, g_wounds, 9, 4, false, g_pointsPerUnit),
            m_bile(Weapon::Type::Missile, "Bilespurter's Vile Bile", 12, RAND_D3, 4, 2, -2, RAND_D3),
            m_scythe(Weapon::Type::Melee, "Harvestman's Scythe", 2, 3, 3, 3, -1, 2),
            m_claws(Weapon::Type::Melee, "Bilespurter's Monstrous Claws", 3, 5, 4, 2, -1, 1) {
        m_keywords = {CHAOS, MORTAL, NURGLE, ROTBRINGER, MONSTER, HERO, WIZARD, BLOAB_ROTSPAWNED};
        m_weapons = {&m_bile, &m_scythe, &m_claws};
        m_battleFieldRole = Role::Leader_Behemoth;
        m_hasMount = true;
        m_claws.setMount(true);
        m_bile.setMount(true);
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setGeneral(isGeneral);

        s_globalCastMod.connect(this, &BloabRotspawned::windspeakerBellsCastingMod, &m_windspeakerSlot);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bile);
        model->addMeleeWeapon(&m_scythe);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    BloabRotspawned::~BloabRotspawned() {
        m_windspeakerSlot.disconnect();
    }

    int BloabRotspawned::windspeakerBellsCastingMod(const Unit *caster) {
        // Windspeaker Bells
        if (!isFriendly(caster) && caster->hasKeyword(WIZARD) && (distanceTo(caster) <= 14.0)) {
            return -1;
        }
        return 0;
    }

    int BloabRotspawned::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void BloabRotspawned::onStartHero(PlayerId player) {
        NurgleBase::onStartHero(player);

        // Daemon Flies
        if (owningPlayer() == player) {
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 7.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 4) {
                    unit->buffModifier(Attribute::To_Hit_Melee, -1, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                    unit->buffModifier(Attribute::To_Hit_Missile, -1, {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
                }
            }
        }
    }

} // namespace Nurgle
