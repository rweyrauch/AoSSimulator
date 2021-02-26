/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <everchosen/Archaon.h>
#include <UnitFactory.h>
#include <Roster.h>
#include <spells/MysticShield.h>
#include "../slavestodarkness/SlavesToDarknessPrivate.h"

namespace SlavesToDarkness {
    static const int g_basesize = 130;
    static const int g_wounds = 20;
    static const int g_pointsPerUnit = 800;

    struct TableEntry {
        int m_move;
        int m_headsAttacks;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {4, 8, 12, 16, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {14, 6},
                    {12, 5},
                    {10, 4},
                    {8,  3},
                    {6,  2}
            };

    bool Archaon::s_registered = false;

    Archaon::Archaon() :
            SlavesToDarknessBase("Archaon", 14, g_wounds, 10, 3, true),
            m_slayerOfKings(Weapon::Type::Melee, "The Slayer of Kings", 1, 4, 2, 3, -2, 3),
            m_dorgharsClaws(Weapon::Type::Melee, "Monstrous Claws", 1, 2, 2, 3, -2, RAND_D6),
            m_dorgharsTails(Weapon::Type::Melee, "Lashing Tails", 3, RAND_2D6, 4, 3, 0, 1),
            m_dorgharsHeads(Weapon::Type::Melee, "Three Heads", 3, 6, 3, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, MORTAL, SLAVES_TO_DARKNESS, EVERCHOSEN, KHORNE, TZEENTCH, NURGLE, SLAANESH,
                      HEDONITE, UNDIVIDED, MONSTER, HERO, WIZARD, ARCHAON};
        m_weapons = {&m_slayerOfKings, &m_dorgharsClaws, &m_dorgharsTails, &m_dorgharsHeads};
        m_battleFieldRole = Leader_Behemoth;

        s_globalBraveryMod.connect(this, &Archaon::crownOfDomination, &m_connection);

        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    Archaon::~Archaon() {
        m_connection.disconnect();
    }

    bool Archaon::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_slayerOfKings);
        model->addMeleeWeapon(&m_dorgharsClaws);
        model->addMeleeWeapon(&m_dorgharsTails);
        model->addMeleeWeapon(&m_dorgharsHeads);
        model->setName("Archaon");
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void Archaon::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *Archaon::Create(const ParameterList &parameters) {
        auto unit = new Archaon();

        auto legion = (DamnedLegion) GetEnumParam("Damned Legion", parameters, g_damnedLegion[0]);
        unit->setDamnedLegion(legion);

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

    void Archaon::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlavesToDarknessBase::ValueToString,
                    SlavesToDarknessBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Damned Legion", g_damnedLegion[0], g_damnedLegion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {EVERCHOSEN, SLAVES_TO_DARKNESS, KHORNE, TZEENTCH, NURGLE, SLAANESH}
            };
            s_registered = UnitFactory::Register("Archaon", factoryMethod);
        }
    }

    void Archaon::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_dorgharsHeads.setAttacks(g_damageTable[damageIndex].m_headsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int Archaon::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Wounds Archaon::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        return Unit::computeReturnedDamage(weapon, saveRoll);
    }

    Wounds Archaon::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        auto modifiedWounds = Unit::applyWoundSave(wounds, attackingUnit);

        return modifiedWounds;
    }

    Wounds Archaon::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        auto wounds = Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
        // The Slayer of Kings
        if ((woundRoll == 6) && (target->hasKeyword(HERO)) && (weapon->name() == m_slayerOfKings.name())) {
            m_slayerOfKingsSixesThisCombat++;

            if (m_slayerOfKingsSixesThisCombat >= 2) {
                // Target is slain.  (Fake it by generating a lot of mortal wounds).
                wounds = {weapon->damage(), target->wounds() * 10};
            }
        }
        return wounds;
    }

    void Archaon::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        m_slayerOfKingsSixesThisCombat = 0;
    }

    void Archaon::onStartHero(PlayerId player) {
        Unit::onStartHero(player);

        // Warlord Without Equal
        if (owningPlayer() == player) {
            if (m_roster) m_roster->addCommandPoints(1);
        }
    }

    int Archaon::crownOfDomination(const Unit *unit) {
        // The Crown of Domination
        if (unit->hasKeyword(CHAOS) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return 2;
        } else if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return -2;
        }

        return 0;
    }

    int Archaon::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace SlavesToDarkness
