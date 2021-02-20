/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/TroggothHag.h>
#include <UnitFactory.h>
#include <iostream>
#include <Board.h>
#include <spells/MysticShield.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 120; // oval
    static const int g_wounds = 16;
    static const int g_pointsPerUnit = 380;

    bool TroggothHag::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_vomitAttacks;
        int m_staffToWound;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {4, 8, 11, 13, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 6, 2},
                    {7, 5, 3},
                    {6, 4, 3},
                    {5, 3, 4},
                    {4, 2, 5}
            };

    TroggothHag::TroggothHag() :
            GloomspiteGitzBase("Troggoth Hag", RAND_4D6, g_wounds, 10, 5, false),
            m_vomit(Weapon::Type::Missile, "Copious Vomit", 10, 6, 3, 3, -2, RAND_D3),
            m_staff(Weapon::Type::Melee, "Deadwood Staff", 2, 4, 4, 2, -1, 3),
            m_bulk(Weapon::Type::Melee, "Crushing Bulk", 1, RAND_D3, 4, 3, 0, RAND_D6) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, FELLWATER, MONSTER, HERO, WIZARD, TROGGOTH_HAG};
        m_weapons = {&m_vomit, &m_staff, &m_bulk};
        m_battleFieldRole = Leader_Behemoth;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    void TroggothHag::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    bool TroggothHag::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_bulk);
        model->addMissileWeapon(&m_vomit);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    void TroggothHag::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_vomit.setAttacks(g_damageTable[damageIndex].m_vomitAttacks);
        m_staff.setToWound(g_damageTable[damageIndex].m_staffToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int TroggothHag::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void TroggothHag::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Hag Regeneration
            if (remainingWounds() < g_wounds && remainingWounds() > 0) {
                int woundsHealed = Dice::RollD6();
                for (auto &m : m_models) {
                    m->applyHealing(woundsHealed);
                }
            }
        }
    }

    Unit *TroggothHag::Create(const ParameterList &parameters) {
        auto unit = new TroggothHag();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fortuitousTroggbossTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_glintyGubbinzThatTroggothsFound[0]);
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

    void TroggothHag::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TroggothHag::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    TroggothHag::ComputePoints,
                    {
                        EnumParameter("Command Trait", g_fortuitousTroggbossTraits[0], g_fortuitousTroggbossTraits),
                        EnumParameter("Artefact", g_glintyGubbinzThatTroggothsFound[0], g_glintyGubbinzThatTroggothsFound),
                        BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Troggoth Hag", factoryMethod);
        }
    }

    int TroggothHag::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void TroggothHag::onUnboundSpell(Unit *caster, int castRoll) {
        Unit::onUnboundSpell(caster, castRoll);

        // Spell Eater
        if (Dice::RollD6() >= 4) {
            caster->applyDamage({0, Dice::RollD3()}, this);
        }
    }

    int TroggothHag::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);

        if (!weapon->isMissile()) {
            // Terrible Stench
            modifier -= 1;
        }
        return modifier;
    }

} // namespace GloomspiteGitz
