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
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int BASESIZE = 120; // oval
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 380;

    bool TroggothHag::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_vomitAttacks;
        int m_staffToWound;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 8, 11, 13, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, 6, 2},
                    {7, 5, 3},
                    {6, 4, 3},
                    {5, 3, 4},
                    {4, 2, 5}
            };

    TroggothHag::TroggothHag() :
            GloomspiteGitzBase("Troggoth Hag", RAND_4D6, WOUNDS, 10, 5, false),
            m_vomit(Weapon::Type::Missile, "Copious Vomit", 10, 6, 3, 3, -2, RAND_D3),
            m_staff(Weapon::Type::Melee, "Deadwood Staff", 2, 4, 4, 2, -1, 3),
            m_bulk(Weapon::Type::Melee, "Crushing Bulk", 1, RAND_D3, 4, 3, 0, RAND_D6) {
        m_keywords = {DESTRUCTION, TROGGOTH, GLOOMSPITE_GITZ, FELLWATER, MONSTER, HERO, WIZARD, TROGGOTH_HAG};
        m_weapons = {&m_vomit, &m_staff, &m_bulk};
        m_battleFieldRole = LeaderBehemoth;
    }

    void TroggothHag::onRestore() {
        // Restore table-driven attributes
        onWounded();
    }

    bool TroggothHag::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_bulk);
        model->addMissileWeapon(&m_vomit);
        addModel(model);

        m_points = POINTS_PER_UNIT;

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
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void TroggothHag::onStartHero(PlayerId player) {
        if (player == owningPlayer()) {
            // Hag Regeneration
            if (remainingWounds() < WOUNDS && remainingWounds() > 0) {
                int woundsHealed = Dice::rollD6();
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
        return POINTS_PER_UNIT;
    }

    void TroggothHag::onUnboundSpell(Unit *caster, int castRoll) {
        Unit::onUnboundSpell(caster, castRoll);

        // Spell Eater
        if (Dice::rollD6() >= 4) {
            caster->applyDamage({0, Dice::rollD3()});
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
