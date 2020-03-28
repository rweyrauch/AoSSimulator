/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Gors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 200;

    bool Gors::s_registered = false;

    Gors::Gors() :
            BeastsOfChaosBase("Gors", 6, WOUNDS, 5, 5, false),
            m_gorBlade(Weapon::Type::Melee, "Gor Blade", 1, 1, 4, 3, 0, 1),
            m_gorBladeFoeRender(Weapon::Type::Melee, "Gor Blade", 1, 2, 4, 3, 0, 1) {
        m_keywords = {CHAOS, BEASTS_OF_CHAOS, BRAYHERD, GORS};
        m_weapons = {&m_gorBlade, &m_gorBladeFoeRender};
    }

    bool Gors::configure(int numModels, bool pairedBlades, bool brayhorn, bool bannerBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_brayhorn = brayhorn;
        m_bannerBearer = bannerBearer;
        m_pairedBlades = pairedBlades;

        auto foe = new Model(BASESIZE, wounds());
        foe->addMeleeWeapon(&m_gorBladeFoeRender);
        addModel(foe);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_gorBlade);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Gors::Create(const ParameterList &parameters) {
        auto unit = new Gors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool pairedBlades = GetBoolParam("Paired Blades", parameters, false);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, BeastsOfChaosBase::None);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, pairedBlades, brayhorn, bannerBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Gors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Integer, "Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE},
                            {ParamType::Boolean, "Paired Blades", SIM_FALSE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Brayhorn", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Boolean, "Banner Bearer", SIM_TRUE, SIM_FALSE, SIM_FALSE, 0},
                            {ParamType::Enum, "Greatfray", BeastsOfChaosBase::None, BeastsOfChaosBase::None,
                             BeastsOfChaosBase::Gavespawn, 1},
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Gors", factoryMethod);
        }
    }

    Rerolls Gors::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Rend and Tear
        if (m_pairedBlades) { return RerollOnes; }
        return Unit::toHitRerolls(weapon, target);
    }

    int Gors::toSaveModifier(const Weapon *weapon) const {
        int modifier = Unit::toSaveModifier(weapon);
        // Beastsheilds
        if (!m_pairedBlades) { modifier += 1; }
        return modifier;
    }

    int Gors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = Unit::extraAttacks(attackingModel, weapon, target);
        // Anarchy and Mayhem
        if (remainingModels() >= 20) {
            attacks += 1;
        }
        return attacks;
    }

    int Gors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} // namespace BeastsOfChaos