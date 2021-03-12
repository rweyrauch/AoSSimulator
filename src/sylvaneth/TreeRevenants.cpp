/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/TreeRevenants.h>
#include <UnitFactory.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 80;
    static const int g_pointsMaxUnitSize = 420;

    bool TreeRevenants::s_registered = false;

    TreeRevenants::TreeRevenants() :
            SylvanethBase("Tree Revenants", 5, g_wounds, 6, 5, false),
            m_enchantedBlade(Weapon::Type::Melee, "Enchanted Blade", 1, 2, 4, 3, -1, 1),
            m_enchantedBladeScion(Weapon::Type::Melee, "Enchanted Blade", 1, 4, 4, 3, -1, 1),
            m_protectorGlaive(Weapon::Type::Melee, "Protector Glaive", 1, 2, 4, 3, -1, 2) {
        m_keywords = {ORDER, SYLVANETH, TREE_REVENANTS};
        m_weapons = {&m_enchantedBlade, &m_enchantedBladeScion, &m_protectorGlaive};
    }

    bool TreeRevenants::configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_gladeBanners = gladeBanners;
        m_waypipes = waypipes;

        auto scion = new Model(g_basesize, wounds());
        if (scionGlaive) {
            scion->addMeleeWeapon(&m_protectorGlaive);
        } else {
            scion->addMeleeWeapon(&m_enchantedBladeScion);
        }
        addModel(scion);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_enchantedBlade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        if (m_gladeBanners) {
            m_pileInMove = 6;
        }

        return true;
    }

    Unit *TreeRevenants::Create(const ParameterList &parameters) {
        auto unit = new TreeRevenants();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool scionGlaive = GetBoolParam("Scion Glaive", parameters, false);
        bool gladeBanners = GetBoolParam("Glade Banners", parameters, false);
        bool waypipes = GetBoolParam("Waypipes", parameters, false);

        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        unit->setGlade(glade);

        bool ok = unit->configure(numModels, scionGlaive, gladeBanners, waypipes);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void TreeRevenants::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    TreeRevenants::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    TreeRevenants::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Scion Glaive"),
                            BoolParameter("Glade Banners"),
                            BoolParameter("Waypipes"),
                            EnumParameter("Glade", g_glade[0], g_glade),
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Tree-Revenants", factoryMethod);
        }
    }

    void TreeRevenants::onBeginTurn(int battleRound) {
        m_combatRerollAvailable = true;
        m_moveRerollAvailable = true;
        m_missileRerollAvailable = true;
        m_battleshockRerollAvailable = true;

        SylvanethBase::onBeginTurn(battleRound);
    }

    Rerolls TreeRevenants::runRerolls() const {
        if (m_moveRerollAvailable) {
            m_moveRerollAvailable = false;
            return Rerolls::Failed;
        }
        return SylvanethBase::runRerolls();
    }

    Rerolls TreeRevenants::chargeRerolls() const {
        if (m_moveRerollAvailable) {
            m_moveRerollAvailable = false;
            return Rerolls::Failed;
        }
        return SylvanethBase::chargeRerolls();
    }

    Rerolls TreeRevenants::battleshockRerolls() const {
        return SylvanethBase::battleshockRerolls();
    }

    Rerolls TreeRevenants::toSaveRerolls(const Weapon *weapon, const Unit* attacker) const {
        return SylvanethBase::toSaveRerolls(weapon, attacker);
    }

    Rerolls TreeRevenants::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        return SylvanethBase::toHitRerolls(weapon, target);
    }

    Rerolls TreeRevenants::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        return SylvanethBase::toWoundRerolls(weapon, target);
    }

    int TreeRevenants::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} // namespace Sylvaneth