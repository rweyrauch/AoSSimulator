/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "mawtribes/FrostSabres.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 2;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 40;
    static const int POINTS_MAX_UNIT_SIZE = 240;


    bool FrostSabres::s_registered = false;

    Unit *FrostSabres::Create(const ParameterList &parameters) {
        auto unit = new FrostSabres();

        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(numModels);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void FrostSabres::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FrostSabres::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    FrostSabres::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Frost Sabres", factoryMethod);
        }
    }

    FrostSabres::FrostSabres() :
            MawtribesBase("Frost Sabres", 9, WOUNDS, 5, 6, false),
            m_fangs(Weapon::Type::Melee, "Elongated Fangs", 1, 3, 4, 3, -1, 1) {
        m_keywords = {DESTRUCTION, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, FROST_SABRES};
        m_weapons = {&m_fangs};
    }

    bool FrostSabres::configure(int numModels) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_fangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int FrostSabres::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

    int FrostSabres::chargeModifier() const {
        auto mod = Unit::chargeModifier();

        // Their Master's Voice
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ICEBROW_HUNTER, 16.0);
        if (unit) mod += 3;

        return mod;
    }

    int FrostSabres::braveryModifier() const {
        auto mod = MawtribesBase::braveryModifier();

        // Their Master's Voice
        auto unit = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), ICEBROW_HUNTER, 16.0);
        if (unit) mod += 2;

        return mod;
    }

} // namespace OgorMawtribes
