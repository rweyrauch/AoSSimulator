/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordDeceiver.h>
#include <UnitFactory.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 320;

    struct TableEntry {
        int m_move;
        int m_tailAttacks;
        int m_stilettoToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 4, 2},
                    {10, 3, 3},
                    {8,  2, 3},
                    {6,  1, 4},
                    {4,  0, 4}
            };


    bool VerminlordDeceiver::s_registered = false;

    Unit *VerminlordDeceiver::Create(const ParameterList &parameters) {
        auto unit = new VerminlordDeceiver();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_eshinClanCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_eshinArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void VerminlordDeceiver::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_eshinClanCommandTraits[0], g_eshinClanCommandTraits),
                            EnumParameter("Artefact", g_eshinArtefacts[0], g_eshinArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Verminlord Deceiver", factoryMethod);
        }
    }

    VerminlordDeceiver::VerminlordDeceiver() :
            Skaventide("Verminlord Deceiver", 12, g_wounds, 10, 4, false),
            m_doomstar(Weapon::Type::Missile, "Doomstar", 13, 1, 3, 3, -1, RAND_D3),
            m_tails(Weapon::Type::Missile, "Prehensile Tail", 6, 4, 3, 3, -1, 1),
            m_warpstiletto(Weapon::Type::Melee, "Warpstiletto", 1, 6, 3, 2, -3, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_ESHIN, MONSTER, HERO, WIZARD,
                      VERMINLORD_DECEIVER};
        m_weapons = {&m_doomstar, &m_tails, &m_warpstiletto};
        m_battleFieldRole = Leader_Behemoth;

        s_globalBraveryMod.connect(this, &VerminlordDeceiver::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    VerminlordDeceiver::~VerminlordDeceiver() {
        m_connection.disconnect();
    }

    bool VerminlordDeceiver::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_doomstar);
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_warpstiletto);
        addModel(model);

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds VerminlordDeceiver::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = Skaventide::applyWoundSave(wounds);

        // Protection of the Horned Rat
        Dice::RollResult resultNormal, resultMortal;

        Dice::RollD6(wounds.normal, resultNormal);
        Dice::RollD6(wounds.mortal, resultMortal);

        Wounds negatedWounds = {resultNormal.rollsGE(5), resultNormal.rollsGE(5)};
        totalWounds -= negatedWounds;
        return totalWounds.clamp();
    }

    int VerminlordDeceiver::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    Wounds
    VerminlordDeceiver::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Doomstar
        if ((weapon->name() == m_doomstar.name()) && (target->remainingModels() >= 10)) {
            return {Dice::RollD6(), 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int VerminlordDeceiver::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Shrouded in Darkness
        if (weapon->isMissile()) mod -= 2;

        return mod;
    }

    void VerminlordDeceiver::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_warpstiletto.setToWound(g_damageTable[damageIndex].m_stilettoToWound);
    }

    void VerminlordDeceiver::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int VerminlordDeceiver::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int VerminlordDeceiver::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
