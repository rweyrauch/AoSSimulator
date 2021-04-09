/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <sylvaneth/SpiteRevenants.h>
#include <UnitFactory.h>
#include "SylvanethPrivate.h"

namespace Sylvaneth {
    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 60;
    static const int g_pointsMaxUnitSize = 200;

    bool SpiteRevenants::s_registered = false;

    SpiteRevenants::SpiteRevenants(Glade glade, int numModels) :
            SylvanethBase("Spite Revenants", 5, g_wounds, 6, 5, false),
            m_cruelTalonsAndFangs(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 3, 3, 3, 0, 1),
            m_cruelTalonsAndFangsShadestalker(Weapon::Type::Melee, "Cruel Talons and Fangs", 1, 4, 3, 3, 0, 1) {
        m_keywords = {ORDER, SYLVANETH, OUTCASTS, SPITE_REVENANTS};
        m_weapons = {&m_cruelTalonsAndFangs, &m_cruelTalonsAndFangsShadestalker};

        setGlade(glade);

        s_globalBraveryMod.connect(this, &SpiteRevenants::unbridledMalice, &m_maliceBraverySlot);
        s_globalBattleshockReroll.connect(this, &SpiteRevenants::unbridledMaliceReroll, &m_maliceRerollSlot);

        auto shadestalker = new Model(g_basesize, wounds());
        shadestalker->addMeleeWeapon(&m_cruelTalonsAndFangsShadestalker);
        addModel(shadestalker);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_cruelTalonsAndFangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);
    }

    SpiteRevenants::~SpiteRevenants() {
        m_maliceBraverySlot.disconnect();
        m_maliceRerollSlot.disconnect();
    }

    Unit *SpiteRevenants::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto glade = (Glade) GetEnumParam("Glade", parameters, g_glade[0]);
        return new SpiteRevenants(glade, numModels);
    }

    void SpiteRevenants::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiteRevenants::Create,
                    SylvanethBase::ValueToString,
                    SylvanethBase::EnumStringToInt,
                    SpiteRevenants::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Glade", g_glade[0], g_glade),
                    },
                    ORDER,
                    {SYLVANETH}
            };
            s_registered = UnitFactory::Register("Spite-Revenants", factoryMethod);
        }
    }

    Rerolls SpiteRevenants::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Malicious Tormentors
        if (hasKeyword(DREADWOOD)) {
            return Rerolls::Ones;
        }
        return SylvanethBase::toHitRerolls(weapon, target);
    }

    int SpiteRevenants::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int SpiteRevenants::unbridledMalice(const Unit *target) {
        // Unbridled Malice
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }

        return 0;
    }

    Rerolls SpiteRevenants::unbridledMaliceReroll(const Unit *unit) {
        // Unbridled Malice
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 3.0)) {
            return Rerolls::Successful;
        }
        return Rerolls::None;
    }

} // namespace Sylvaneth