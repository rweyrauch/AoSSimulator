/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/Dryads.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 270;

    bool Dryads::s_registered = false;

    Dryads::Dryads() :
            SylvanethBase("Dryads", 7, g_wounds, 6, 5, false),
            m_wrackingTalons(Weapon::Type::Melee, "Wracking Talons", 2, 2, 4, 4, 0, 1),
            m_wrackingTalonsNymph(Weapon::Type::Melee, "Wracking Talons", 2, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, SYLVANETH, FOREST_FOLK, DRYADS};
        m_weapons = {&m_wrackingTalons, &m_wrackingTalonsNymph};
        m_battleFieldRole = Battleline;
    }

    bool Dryads::configure(int numModels) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto nymph = new Model(g_basesize, wounds());
        nymph->addMeleeWeapon(&m_wrackingTalonsNymph);
        addModel(nymph);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_wrackingTalons);
            addModel(model);
        }

        m_ranks = std::max(1, (numModels / g_minUnitSize) / 2);

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Dryads::Create(const ParameterList &parameters) {
        auto unit = new Dryads();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Dryads::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Dryads::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    Dryads::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Glade", g_glade[0], g_glade),
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Dryads", factoryMethod);
        }
    }

    int Dryads::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);

        // Impenetrable Thicket
        if (remainingModels() >= 10) modifier += 1;

        return modifier;
    }

    int Dryads::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = Unit::targetHitModifier(weapon, attacker);

        // Blessing of the Forest
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), AWAKENED_WYLDWOOD, 6.0);
        if (unit != nullptr) {
            modifier -= 1;
        }

        return modifier;
    }

    int Dryads::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    void Dryads::onStartCombat(PlayerId player) {
        Unit::onStartCombat(player);

        // Enrapturing Song
        // For now always select the unit the Dryads are targeting in combat.
        m_enrapturedUnit = m_meleeTarget;
    }

    int Dryads::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = Unit::toHitModifier(weapon, target);

        // Enrapturing Song
        if (m_enrapturedUnit && (target == m_enrapturedUnit)) {
            mod++;
        }

        return mod;
    }

} // namespace Sylvaneth