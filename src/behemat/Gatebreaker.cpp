/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Gatebreaker.h>
#include <UnitFactory.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 35;
    static const int POINTS_PER_UNIT = 490;

    struct TableEntry {
        int m_move;
        int m_flailAttacks;
        int m_boulderRange;
        int m_smashDown;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {12, 18, 24, 30, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {12, 10, 18, 2},
                    {11, 9, 15, 3},
                    {10, 7, 12, 4},
                    {9, 6,  9, 5},
                    {8, 5,  6, 6}
            };

    bool Gatebreaker::s_registered = false;

    Gatebreaker::Gatebreaker() :
            SonsOfBehematBase("Gatebreaker Mega-Gargant", 12, WOUNDS, 7, 4, false),
            m_boulder(Weapon::Type::Missile, "Hurled Boulder", 18, 1, 3, 2, -3, 4),
            m_stomp(Weapon::Type::Melee, "Almighty Stomp", 2, 2, 3, 3, -2, RAND_D3),
            m_grip(Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6),
            m_flail(Weapon::Type::Melee, "Fortcrusha Flail", 3, 10, 4, 3, -3, 3) {
        m_weapons = {&m_boulder, &m_stomp, &m_grip, &m_flail};
        m_battleFieldRole = Behemoth;
    }

    bool Gatebreaker::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_boulder);
        model->addMeleeWeapon(&m_stomp);
        model->addMeleeWeapon(&m_flail);
        model->addMeleeWeapon(&m_grip);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Gatebreaker::Create(const ParameterList &parameters) {
        auto unit = new Gatebreaker();

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

    void Gatebreaker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Gatebreaker::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Gatebreaker::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Artefact", g_artefact[0], g_artefact),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Gatebreaker Mega-Gargant", factoryMethod);
        }
    }

    int Gatebreaker::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    int Gatebreaker::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Gatebreaker::onWounded() {
        Unit::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_boulder.setRange(g_damageTable[getDamageTableIndex()].m_boulderRange);
        m_flail.setAttacks(g_damageTable[getDamageTableIndex()].m_flailAttacks);
    }

    void Gatebreaker::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

} // namespace SonsOfBehemat
