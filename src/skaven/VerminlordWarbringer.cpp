/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/VerminlordWarbringer.h>
#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "SkavenPrivate.h"

namespace Skaven {
    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 280;

    struct TableEntry {
        int m_move;
        int m_tailAttacks;
        int m_fistToWound;
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


    bool VerminlordWarbringer::s_registered = false;

    Unit *VerminlordWarbringer::Create(const ParameterList &parameters) {
        auto unit = new VerminlordWarbringer();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_verminousCommandTraits[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_verminousArtefacts[0]);
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

    void VerminlordWarbringer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_verminousCommandTraits[0], g_verminousCommandTraits),
                            EnumParameter("Artefact", g_verminousArtefacts[0], g_verminousArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Verminlord Warbringer", factoryMethod);
        }
    }

    VerminlordWarbringer::VerminlordWarbringer() :
            Skaventide("Verminlord Warbringer", 12, g_wounds, 10, 4, false),
            m_tails(Weapon::Type::Missile, "Prehensile Tails", 6, 4, 3, 3, -1, 1),
            m_glaive(Weapon::Type::Melee, "Doom Glaive", 3, 6, 3, 3, -1, RAND_D3),
            m_fist(Weapon::Type::Melee, "Spike-fist", 1, 1, 3, 2, -2, 2) {
        m_keywords = {CHAOS, DAEMON, VERMINLORD, SKAVENTIDE, CLANS_VERMINUS, MONSTER, HERO, WIZARD,
                      VERMINLORD_WARBRINGER};
        m_weapons = {&m_tails, &m_glaive, &m_fist};
        m_battleFieldRole = Role::Leader_Behemoth;

        s_globalBraveryMod.connect(this, &VerminlordWarbringer::terrifying, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    VerminlordWarbringer::~VerminlordWarbringer() {
        m_connection.disconnect();
    }

    bool VerminlordWarbringer::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_tails);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_fist);
        addModel(model);

        //m_knownSpells.push_back(std::make_unique<DreadedDeathFrenzy>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Wounds VerminlordWarbringer::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Skaventide::applyWoundSave(wounds, attackingUnit);

        // Protection of the Horned Rat
        return ignoreWounds(totalWounds, 5);
    }

    int VerminlordWarbringer::terrifying(const Unit *target) {
        // Terrifying
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    Wounds
    VerminlordWarbringer::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Fist of Verminus Supremacy
        if ((woundRoll == 6) && (weapon->name() == m_fist.name())) {
            return {weapon->damage() + 4, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    Rerolls VerminlordWarbringer::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Amidst the Seething Tide
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 13.0);
        int totalModels = 0;
        for (auto unit : units) {
            if (unit->hasKeyword(SKAVEN)) {
                totalModels += unit->remainingModels();
            }
        }
        if (totalModels >= 13)
            return Rerolls::Failed;

        return Unit::toWoundRerolls(weapon, target);
    }

    void VerminlordWarbringer::onWounded() {
        Unit::onWounded();

        const int damageIndex = getDamageTableIndex();
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_tails.setAttacks(g_damageTable[damageIndex].m_tailAttacks);
        m_fist.setToWound(g_damageTable[damageIndex].m_fistToWound);
    }

    void VerminlordWarbringer::onRestore() {
        Unit::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    int VerminlordWarbringer::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int VerminlordWarbringer::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
