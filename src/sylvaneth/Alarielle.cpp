/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <Dice.h>
#include <sylvaneth/Alarielle.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <sylvaneth/SylvanethSpells.h>
#include <Board.h>
#include <Roster.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int BASESIZE = 160;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 600;

    bool Alarielle::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_spearKurnothRange;
        int m_greatAntlerDamage;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 7, 10, 13, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {16, 30, 5},
                    {14, 25, 4},
                    {10, 20, 3},
                    {8,  15, 2},
                    {6,  10, 1}
            };


    Alarielle::Alarielle() :
            SylvanethBase("Alarielle", 16, WOUNDS, 10, 3, true),
            m_spearOfKurnoth(Weapon::Type::Missile, "Spear of Kurnoth", 30, 1, 3, 2, -2, RAND_D6),
            m_talonOfDwindling(Weapon::Type::Melee, "Talon of Dwindling", 1, 4, 3, 4, 0, 1),
            m_beetleGreatAntlers(Weapon::Type::Melee, "Great Antlers", 2, 5, 4, 3, -2, 5) {
        m_keywords = {ORDER, SYLVANETH, MONSTER, HERO, WIZARD, ALARIELLE_THE_EVERQUEEN};
        m_weapons = {&m_spearOfKurnoth, &m_talonOfDwindling, &m_beetleGreatAntlers};
        m_battleFieldRole = LeaderBehemoth;

        m_totalUnbinds = 3;
        m_totalSpells = 3;
    }

    Alarielle::~Alarielle() {
        m_ghyransWrathSlot.disconnect();
    }

    bool Alarielle::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_spearOfKurnoth);
        model->addMeleeWeapon(&m_talonOfDwindling);
        model->addMeleeWeapon(&m_beetleGreatAntlers);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateMetamorphosis(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Alarielle::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        // Sweeping Blows
        if (weapon->name() == m_beetleGreatAntlers.name() && unit->remainingModels() >= 5) {
            return 1;
        }
        return Unit::toHitModifier(weapon, unit);
    }

    void Alarielle::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                // Lifebloom - heal herself D3
                int woundsHealed = Dice::rollD3();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }

            // Lifebloom
            auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 30.0);
            for (auto ip : units) {
                if (ip->hasKeyword(SYLVANETH))
                    ip->heal(Dice::rollD3());
            }
        }
    }

    void Alarielle::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_spearOfKurnoth.setRange(g_damageTable[damageIndex].m_spearKurnothRange);
        m_beetleGreatAntlers.setDamage(g_damageTable[damageIndex].m_greatAntlerDamage);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int Alarielle::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *Alarielle::Create(const ParameterList &parameters) {
        auto unit = new Alarielle();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDeepwood[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Alarielle::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Lore", g_loreOfTheDeepwood[0], g_loreOfTheDeepwood),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Alarielle", factoryMethod);
        }
    }

    void Alarielle::onCharged() {
        // Living Battering Ram
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 1.0);
        for (auto ip : units) {
            if (Dice::rollD6() >= 4) {
                Wounds wounds = {0, 0};
                wounds.mortal = Dice::rollD3();
                ip->applyDamage(wounds);
            }
        }
        Unit::onCharged();
    }

    Wounds Alarielle::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Talon of the Dwindling
        if (weapon->name() == m_talonOfDwindling.name() && hitRoll == 6) {
            return {weapon->damage(), Dice::rollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void Alarielle::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        m_ghyransWrathSlot.disconnect();

        // Ghyran's Wrath
        if (getCommandPoints() > 0) {
            if (m_roster && m_roster->useCommandPoint()) {
                // Buff Alarielle
                buffReroll(ToWoundMelee, RerollOnes, {Combat, m_battleRound + 1, owningPlayer()});

                // Buff all friends with in 14".
                s_globalToWoundReroll.connect(this, &Alarielle::ghyransWrathToWoundReroll, &m_ghyransWrathSlot);
            }
        }
    }

    void Alarielle::onEndMovement(PlayerId player) {
        Unit::onEndMovement(player);

        if (owningPlayer() == player && !m_usedSoulAmphorae) {
            // Summon a unit and add to roster.
            auto factory = UnitFactory::LookupUnit("Kurnoth Hunters");
            if (factory) {
                if (m_roster) {
                    auto unit = UnitFactory::Create("Kurnoth Hunters", factory->m_parameters);
                    unit->deploy(position(), m_orientation);
                    m_roster->addUnit(unit);
                }
                m_usedSoulAmphorae = true;
            }
        }
    }

    void Alarielle::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    int Alarielle::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    Rerolls Alarielle::ghyransWrathToWoundReroll(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(SYLVANETH) && (distanceTo(attacker) < 14.0))
            return RerollOnes;
        return NoRerolls;
    }

} // namespace Sylvaneth
