/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/GrimwrathBerzerker.h>
#include <Board.h>
#include <UnitFactory.h>

namespace Fyreslayers {
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool GrimwrathBerzerker::s_registered = false;

    GrimwrathBerzerker::GrimwrathBerzerker() :
            Fyreslayer("Grimwrath Berzerker", 4, WOUNDS, 9, 4, false),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_greatAxe(Weapon::Type::Melee, "Fyrestorm Greataxe", 1, 4, 3, 3, -2, 2) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, GRIMWRATH_BERZERKER};
        m_weapons = {&m_throwingAxe, &m_greatAxe};
    }

    bool GrimwrathBerzerker::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMeleeWeapon(&m_greatAxe);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *GrimwrathBerzerker::Create(const ParameterList &parameters) {
        auto unit = new GrimwrathBerzerker();

        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, Fyreslayer::None);
        unit->setLodge(lodge);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void GrimwrathBerzerker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GrimwrathBerzerker::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    GrimwrathBerzerker::ComputePoints,
                    {
                            {ParamType::Enum, "Lodge", Fyreslayer::None, Fyreslayer::None, Fyreslayer::Lofnir, 1}
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Grimwrath Berzerker", factoryMethod);
        }
    }

    Wounds GrimwrathBerzerker::applyWoundSave(const Wounds &wounds) {
        // Unstoppable Berserker
        Dice::RollResult woundSaves, mortalSaves;
        Dice::rollD6(wounds.normal, woundSaves);
        Dice::rollD6(wounds.mortal, mortalSaves);

        int threshold = 6;
        auto unit = Board::Instance()->getNearestUnit(this, GetEnemyId(owningPlayer()));
        if (unit && (distanceTo(unit) <= 3.0f)) threshold--;

        Wounds totalWounds = wounds;
        totalWounds.normal -= woundSaves.rollsGE(threshold);
        totalWounds.mortal -= mortalSaves.rollsGE(threshold);

        return totalWounds.clamp();
    }

    int GrimwrathBerzerker::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Fyreslayers