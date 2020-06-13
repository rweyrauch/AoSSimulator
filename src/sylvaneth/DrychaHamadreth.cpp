/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/DrychaHamadreth.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <sylvaneth/SylvanethSpells.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 10;
    static const int POINTS_PER_UNIT = 320;

    bool DrychaHamadreth::s_registered = false;

    struct TableEntry {
        int m_flitterfuriesRange;
        int m_squirmlingsHit;
        int m_talonAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 6, 8, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {18, 3, 6},
                    {15, 4, 5},
                    {12, 4, 4},
                    {9,  5, 3},
                    {6,  5, 2}
            };

    DrychaHamadreth::DrychaHamadreth() :
            SylvanethBase("Drycha Hamadreth", 9, WOUNDS, 8, 3, false),
            m_colonyOfFlitterfuries(Weapon::Type::Missile, "Colony of Flitterfuries", 18, 10, 4, 3, -1, 1),
            m_swarmOfSquirmlings(Weapon::Type::Missile, "Swarm of Squirmlings", 2, 10, 3, 4, 0, 1),
            m_slashingTalons(Weapon::Type::Melee, "Slashing Talons", 2, 6, 4, 3, -2, 2) {
        m_keywords = {ORDER, SYLVANETH, OUTCASTS, MONSTER, HERO, WIZARD, DRYCHA_HAMADRETH};
        m_weapons = {&m_colonyOfFlitterfuries, &m_swarmOfSquirmlings, &m_slashingTalons};

        s_globalToWoundReroll.connect(this, &DrychaHamadreth::songOfSpiteToWoundRerolls, &m_songSlot);

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    DrychaHamadreth::~DrychaHamadreth() {
        m_songSlot.disconnect();
    }

    bool DrychaHamadreth::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_colonyOfFlitterfuries);
        model->addMissileWeapon(&m_swarmOfSquirmlings);
        model->addMeleeWeapon(&m_slashingTalons);
        model->addMeleeWeapon(&m_thornedSlendervines);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreatePrimalTerror(this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void DrychaHamadreth::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_colonyOfFlitterfuries.setRange(g_damageTable[damageIndex].m_flitterfuriesRange);
        m_swarmOfSquirmlings.setToHit(g_damageTable[damageIndex].m_squirmlingsHit);
        m_slashingTalons.setAttacks(g_damageTable[damageIndex].m_talonAttacks);
    }

    int DrychaHamadreth::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *DrychaHamadreth::Create(const ParameterList &parameters) {
        auto unit = new DrychaHamadreth();

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfTheDeepwood[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void DrychaHamadreth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    DrychaHamadreth::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    DrychaHamadreth::ComputePoints,
                    {
                            EnumParameter("Glade", g_glade[0], g_glade),
                            EnumParameter("Lore", g_loreOfTheDeepwood[0], g_loreOfTheDeepwood)
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Drycha Hamadreth", factoryMethod);
        }
    }

    Wounds DrychaHamadreth::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Deadly Infestation
        if (((weapon->name() == m_colonyOfFlitterfuries.name()) || weapon->name() == m_swarmOfSquirmlings.name()) &&
            (woundRoll == 6)) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    void DrychaHamadreth::onBeginRound(int battleRound) {
        // Mercurial Aspect
        m_enraged = (m_meleeTarget == nullptr);
        Unit::onBeginRound(battleRound);
    }

    int DrychaHamadreth::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto extra = Unit::extraAttacks(attackingModel, weapon, target);
        // Mecurial Aspect
        if (weapon->name() == m_colonyOfFlitterfuries.name() && m_enraged) {
            extra += 10;
        }
        if (weapon->name() == m_swarmOfSquirmlings.name() && !m_enraged) {
            extra += 10;
        }
        return extra;
    }

    int DrychaHamadreth::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    Rerolls DrychaHamadreth::songOfSpiteToWoundRerolls(const Unit *attacker, const Weapon *weapon, const Unit *target) {

        if (isFriendly(attacker) && attacker->hasKeyword(SPITE_REVENANTS) && (distanceTo(attacker) < 16.0))
            return RerollOnes;

        return NoRerolls;
    }

} // namespace Sylvaneth
