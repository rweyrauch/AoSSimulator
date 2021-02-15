/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "slaanesh/GlutosOrscollion.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 100;
    static const int g_wounds = 18;
    static const int g_pointsPerUnit = 400;

    struct TableEntry {
        int m_move;
        int m_clawAttacks;
    };

    const size_t g_numTableEntries = 4;
    static int g_woundThresholds[g_numTableEntries] = {4, 8, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {8, 6},
                    {6, 5},
                    {5, 4},
                    {4, 3},
            };

    bool GlutosOrscollion::s_registered = false;

    GlutosOrscollion::GlutosOrscollion() :
            SlaaneshBase("Glutos Orscollion", 14, g_wounds, 10, 4, false),
            m_greatblade(Weapon::Type::Melee, "Wailing Greatblade", 1, 3, 3, 3, -2, 2),
            m_scourge(Weapon::Type::Melee, "Flaying Scourge", 1, 2, 3, 4, 0, 1),
            m_dagger(Weapon::Type::Melee, "Sacrificial Dagger", 1, 1, 4, 3, 0, 1),
            m_claws(Weapon::Type::Melee, "Crushing Claws", 1, 6, 3, 3, -1, 3) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HERO, WIZARD, GLUTOS_ORSCOLLION};
        m_weapons = {&m_greatblade, &m_scourge, &m_dagger, &m_claws};
        m_battleFieldRole = Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    bool GlutosOrscollion::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());

        model->addMeleeWeapon(&m_greatblade);
        model->addMeleeWeapon(&m_scourge);
        model->addMeleeWeapon(&m_dagger);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    Unit *GlutosOrscollion::Create(const ParameterList &parameters) {
        auto unit = new GlutosOrscollion();
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greaterDaemonLore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GlutosOrscollion::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GlutosOrscollion::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    GlutosOrscollion::ComputePoints,
                    {
                            EnumParameter("Host", g_host[0], g_host),
                            EnumParameter("Lore", g_greaterDaemonLore[0], g_greaterDaemonLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Glutos Orscollion", factoryMethod);
        }
    }

    void GlutosOrscollion::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_claws.setAttacks(g_damageTable[damageIndex].m_clawAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        if (woundsTaken() >= 13) {
            m_greatblade.activate(false);
        } else if (woundsTaken() >= 8) {
            m_scourge.activate(false);
        } else if (woundsTaken() >= 4) {
            m_dagger.activate(false);
        }
        Unit::onWounded();
    }

    int GlutosOrscollion::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void GlutosOrscollion::onRestore() {
        // Reset table-drive attributes
        onWounded();

        m_greatblade.activate(true);
        m_scourge.activate(true);
        m_dagger.activate(true);
    }

    int GlutosOrscollion::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // Slannesh
