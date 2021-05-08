/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordDeceiver.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
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
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_eshinClanCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_eshinArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new VerminlordDeceiver(trait, artefact, general);
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

    VerminlordDeceiver::VerminlordDeceiver(CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Verminlord Deceiver", 12, g_wounds, 10, 4, false, g_pointsPerUnit),
            m_doomstar(Weapon::Type::Missile, "Doomstar", 13, 1, 3, 3, -1, RAND_D3),
            m_tails(Weapon::Type::Missile, "Prehensile Tail", 6, 4, 3, 3, -1, 1),
            m_warpstiletto(Weapon::Type::Melee, "Warpstiletto", 1, 6, 3, 2, -3, RAND_D3) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_ESHIN, MONSTER, HERO, WIZARD,
                      VERMINLORD_DECEIVER};
        m_weapons = {&m_doomstar, &m_tails, &m_warpstiletto};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &VerminlordDeceiver::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_doomstar);
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_warpstiletto);
        addModel(model);

        //m_knownSpells.push_back(std::make_unique<DreadedSkitterleap>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    VerminlordDeceiver::~VerminlordDeceiver() {
        m_connection.disconnect();
    }

    Wounds VerminlordDeceiver::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    int VerminlordDeceiver::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    Wounds
    VerminlordDeceiver::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Doomstar
        if ((weapon->name() == m_doomstar.name()) && (target->remainingModels() >= 10)) {
            return {Dice::RollD6(), 0};
        }
        return Skaventide::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int VerminlordDeceiver::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Skaventide::targetHitModifier(weapon, attacker);

        // Shrouded in Darkness
        if (weapon->isMissile()) mod -= 2;

        return mod;
    }

    void VerminlordDeceiver::onWounded() {
        Skaventide::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_warpstiletto.setToWound(g_damageTable[damageIndex].m_stilettoToWound);
    }

    void VerminlordDeceiver::onRestore() {
        Skaventide::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    size_t VerminlordDeceiver::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int VerminlordDeceiver::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
