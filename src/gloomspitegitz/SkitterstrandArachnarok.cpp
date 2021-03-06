/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <gloomspitegitz/SkitterstrandArachnarok.h>
#include <UnitFactory.h>
#include <iostream>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int g_basesize = 160;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 200;

    bool SkitterstrandArachnarok::s_registered = false;

    struct TableEntry {
        int m_move;
        int m_legsAttacks;
        int m_fangsToHit;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 8, 2},
                    {7, 7, 3},
                    {6, 6, 3},
                    {5, 5, 4},
                    {4, 4, 4}
            };

    SkitterstrandArachnarok::SkitterstrandArachnarok(Allegiance allegiance) :
            GloomspiteGitzBase(allegiance, "Skitterstrand Arachnarok", 8, g_wounds, 6, 4, true, g_pointsPerUnit),
            m_chitinousLegs(Weapon::Type::Melee, "Chitinous Legs", 3, 8, 4, 3, -1, 1),
            m_monstrousFangs(Weapon::Type::Melee, "Monstrous Fangs", 1, 4, 2, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, ARACHNAROK_SPIDER, GLOOMSPITE_GITZ, SPIDERFANG, SKITTERSTRAND, MONSTER};
        m_weapons = {&m_chitinousLegs, &m_monstrousFangs};
        m_battleFieldRole = Role::Behemoth;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_chitinousLegs);
        model->addMeleeWeapon(&m_monstrousFangs);
        addModel(model);
    }

    void SkitterstrandArachnarok::onRestore() {
        GloomspiteGitzBase::onRestore();
        // Reset table-driven attributes
        onWounded();
    }

    void SkitterstrandArachnarok::onWounded() {
        GloomspiteGitzBase::onWounded();
        const auto damageIndex = getDamageTableIndex();
        m_monstrousFangs.setToHit(g_damageTable[damageIndex].m_fangsToHit);
        m_chitinousLegs.setAttacks(g_damageTable[damageIndex].m_legsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t SkitterstrandArachnarok::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *SkitterstrandArachnarok::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        return new SkitterstrandArachnarok(allegiance);
    }

    void SkitterstrandArachnarok::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SkitterstrandArachnarok::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SkitterstrandArachnarok::ComputePoints,
                    {
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Skitterstrand Arachnarok", factoryMethod);
        }
    }

    Wounds
    SkitterstrandArachnarok::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Spider Venom
        int threshold = inLightOfTheBadMoon() ? 5 : 6;
        if ((hitRoll >= threshold) && (weapon->name() == m_monstrousFangs.name())) {
            return {0, Dice::RollD3()};
        }
        return         GloomspiteGitzBase
        ::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SkitterstrandArachnarok::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace GloomspiteGitz
