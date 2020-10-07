/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/KrakenEater.h>
#include <UnitFactory.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 35;
    static const int POINTS_PER_UNIT = 490;

    struct TableEntry {
        int m_move;
        int m_clubAttacks;
        int m_debrisRange;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {12, 18, 24, 30, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {11, 7, 24},
                    {10, 7, 21},
                    {9, 7, 18},
                    {8, 6,  15},
                    {7, 5,  12}
            };

    bool KrakenEater::s_registered = false;

    KrakenEater::KrakenEater() :
            SonsOfBehematBase("Kraken-Eater Mega-Gargant", 11, WOUNDS, 7, 4, false),
            m_debris(Weapon::Type::Missile, "Hurled Debris", 24, 3, 4, 3, -1, RAND_D3),
            m_stomp(Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6),
            m_grip(Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6),
            m_warclub(Weapon::Type::Melee, "Shipwrecka Warclub", 3, 8, 3, 3, -2, 2) {
        m_weapons = {&m_debris, &m_stomp, &m_grip, &m_warclub};
        m_battleFieldRole = Behemoth;
    }

    bool KrakenEater::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_debris);
        model->addMeleeWeapon(&m_stomp);
        model->addMeleeWeapon(&m_warclub);
        model->addMeleeWeapon(&m_grip);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *KrakenEater::Create(const ParameterList &parameters) {
        auto unit = new KrakenEater();

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

    void KrakenEater::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KrakenEater::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    KrakenEater::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Artefact", g_artefact[0], g_artefact),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Kraken-Eater Mega-Gargant", factoryMethod);
        }
    }

    int KrakenEater::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    int KrakenEater::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void KrakenEater::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    void KrakenEater::onWounded() {
        Unit::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
        m_debris.setRange(g_damageTable[getDamageTableIndex()].m_debrisRange);
        m_warclub.setAttacks(g_damageTable[getDamageTableIndex()].m_clubAttacks);
    }

} // namespace SonsOfBehemat
