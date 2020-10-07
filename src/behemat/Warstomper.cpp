/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <behemat/Warstomper.h>
#include <UnitFactory.h>
#include "SonsOfBehehmetPrivate.h"

namespace SonsOfBehemat {
    static const int BASESIZE = 90; // x52 oval
    static const int WOUNDS = 35;
    static const int POINTS_PER_UNIT = 490;

    struct TableEntry {
        int m_move;
        int m_clubExtraAttacks;
        int m_hurledBodyMod;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    static int g_woundThresholds[NUM_TABLE_ENTRIES] = {12, 18, 24, 30, WOUNDS};
    static TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {10, 4, 2},
                    {9, 3, 1},
                    {8, 2, 0},
                    {7, 1,  -1},
                    {6, 0,  -2}
            };

    bool Warstomper::s_registered = false;

    Warstomper::Warstomper() :
            SonsOfBehematBase("Warstomper Mega-Gargant", 10, WOUNDS, 7, 4, false),
            m_grip(Weapon::Type::Melee, "Death Grip", 3, 1, 3, 2, -3, RAND_D6),
            m_jump(Weapon::Type::Melee, "Jump Up and Down", 3, 4, 3, 3, -2, RAND_D3),
            m_club(Weapon::Type::Melee, "Titanic Boulderclub", 3, 1, 3, 3, -2, 2) {
        m_weapons = {&m_grip, &m_jump, &m_club};
        m_battleFieldRole = Behemoth;
    }

    bool Warstomper::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_grip);
        model->addMeleeWeapon(&m_jump);
        model->addMeleeWeapon(&m_club);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Warstomper::Create(const ParameterList &parameters) {
        auto unit = new Warstomper();

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

    void Warstomper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Warstomper::Create,
                    SonsOfBehematBase::ValueToString,
                    SonsOfBehematBase::EnumStringToInt,
                    Warstomper::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTrait[0], g_commandTrait),
                            EnumParameter("Artefact", g_artefact[0], g_artefact),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {SONS_OF_BEHEMAT}
            };
            s_registered = UnitFactory::Register("Warstomper Mega-Gargant", factoryMethod);
        }
    }

    int Warstomper::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    int Warstomper::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void Warstomper::onWounded() {
        Unit::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    void Warstomper::onRestore() {
        Unit::onRestore();

        // Reset table-driven attributes
        onWounded();
    }

    int Warstomper::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        auto attacks = Unit::extraAttacks(attackingModel, weapon, target);

        // TODO: limit number of models in range
        attacks += target->remainingModels() + g_damageTable[getDamageTableIndex()].m_clubExtraAttacks;

        return attacks;
    }

} // namespace SonsOfBehemat
