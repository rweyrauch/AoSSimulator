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
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 2;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 40;
    static const int g_pointsMaxUnitSize = 240;

    bool FrostSabres::s_registered = false;

    bool FrostSabres::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *FrostSabres::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto numModels = GetIntParam("Models", parameters, g_minUnitSize);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new FrostSabres(tribe, numModels, ComputePoints(parameters));
        }
        return nullptr;
    }

    void FrostSabres::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    FrostSabres::Create,
                    MawtribesBase::ValueToString,
                    MawtribesBase::EnumStringToInt,
                    FrostSabres::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Frost Sabres", factoryMethod);
        }
    }

    FrostSabres::FrostSabres(Mawtribe tribe, int numModels, int points) :
            MawtribesBase(tribe, "Frost Sabres", 9, g_wounds, 5, 6, false, points) {
        m_keywords = {DESTRUCTION, OGOR_MAWTRIBES, BEASTCLAW_RAIDERS, FROST_SABRES};
        m_weapons = {&m_fangs};

        for (auto i = 0; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_fangs);
            addModel(model);
        }
    }

    int FrostSabres::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
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
