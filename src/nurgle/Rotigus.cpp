/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <nurgle/Rotigus.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "NurglePrivate.h"

namespace Nurgle {
    static const int BASESIZE = 130;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 320;

    struct TableEntry {
        int m_rodToHit;
        int m_mawToWound;
        int m_delugeOfNurgle;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {3, 6, 9, 12, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {2, 2, 4},
                    {3, 2, 5},
                    {3, 3, 5},
                    {4, 3, 6},
                    {4, 4, 6}
            };

    bool Rotigus::s_registered = false;

    Rotigus::Rotigus() :
            NurgleBase("Rotigus", 5, WOUNDS, 10, 4, false),
            m_gnarlrod(Weapon::Type::Melee, "Gnarlrod", 3, 5, 2, 3, -1, 2),
            m_fangedMaw(Weapon::Type::Melee, "Fanged Maw", 1, RAND_D3, 3, 2, -2, 2),
            m_nurglings(Weapon::Type::Melee, "Host of Nurglings", 1, 3, 5, 5, 0, 1) {
        m_keywords = {CHAOS, DAEMON, NURGLE, MONSTER, HERO, WIZARD, GREAT_UNCLEAN_ONE, ROTIGUS};
        m_weapons = {&m_gnarlrod, &m_fangedMaw, &m_nurglings};
        m_battleFieldRole = LeaderBehemoth;

        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    bool Rotigus::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_gnarlrod);
        model->addMeleeWeapon(&m_fangedMaw);
        model->addMeleeWeapon(&m_nurglings);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Rotigus::Create(const ParameterList &parameters) {
        auto unit = new Rotigus();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_daemonLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Rotigus::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Rotigus::Create,
                    NurgleBase::ValueToString,
                    NurgleBase::EnumStringToInt,
                    Rotigus::ComputePoints,
                    {
                            EnumParameter("Lore", g_daemonLore[0], g_daemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {NURGLE}
            };
            s_registered = UnitFactory::Register("Rotigus", factoryMethod);
        }
    }

    void Rotigus::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_gnarlrod.setToHit(g_damageTable[damageIndex].m_rodToHit);
        m_fangedMaw.setToWound(g_damageTable[damageIndex].m_mawToWound);
    }

    int Rotigus::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Rotigus::onCharged() {
        // Mountain of Loathsome Flesh
        if (m_charged) {
            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
            for (auto ip : units) {
                int roll = Dice::rollD6();
                if (roll >= 4) {
                    ip->applyDamage({Dice::rollD3(), 0});
                }
            }
        }
        Unit::onCharged();
    }

    Wounds Rotigus::applyWoundSave(const Wounds &wounds) {
        // Blubber and Bile
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(5);
        totalWounds.mortal -= mortalSaves.rollsGE(5);

        // TODO: on 6+ attacking unit takes a mortal wound.

        return totalWounds.clamp();
    }

    void Rotigus::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        if (owningPlayer() == player) {
            // Corpulent Mass
            heal(Dice::rollD3());

            auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 6.0);

            // Streams of Brackish Filth
            for (auto unit : units) {
                auto roll = Dice::rollD6();
                if (unit->fly()) {
                    if (roll >= 6) unit->applyDamage({0, Dice::rollD3()});
                } else if (roll >= 4) unit->applyDamage({0, Dice::rollD3()});
            }
        }
    }

    int Rotigus::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Nurgle

