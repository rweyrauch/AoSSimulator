/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Bonegrinder.h>
#include <UnitFactory.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 20;
    static const int POINTS_PER_UNIT = 400;

    struct TableEntry {
        int m_move;
        int m_boulderRange;
        int m_clubAttacks;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {4, 8, 12, 16, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 18, RAND_3D6},
                    {10, 16, RAND_2D6},
                    {8, 14, RAND_2D3},
                    {7, 12,  RAND_D6},
                    {6, 10,  RAND_D3}
            };

    bool Bonegrinder::s_registered = false;

    Bonegrinder::Bonegrinder() :
            SonsOfBehematBase("Bonegrinder Gargant", 12, WOUNDS, 7, 4, false) {
    }

    bool Bonegrinder::configure() {
        return false;
    }

    Unit *Bonegrinder::Create(const ParameterList &parameters) {
        auto unit = new Bonegrinder();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTrait[0]);
        unit->setCommandTrait(trait);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefact[0]);
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

    void Bonegrinder::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bonegrinder::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Bonegrinder::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Artefact", g_artefact[0], g_artefact),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Bonegrinder Gargant", factoryMethod);
        }
    }

    int Bonegrinder::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void Bonegrinder::onWounded() {
        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_boulder.setRange(g_damageTable[getDamageTableIndex()].m_boulderRange);
        m_club.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

    int Bonegrinder::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Bonegrinder::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

} // namespace SonsOfBehemat
