/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Bestigors.h>
#include <UnitFactory.h>

namespace BeastsOfChaos {
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 300;

    bool Bestigors::s_registered = false;

    Bestigors::Bestigors() :
            BeastsOfChaosBase("Bestigors", 6, WOUNDS, 6, 4, false),
            m_despoilerAxe(Weapon::Type::Melee, "Despoiler Axe", 1, 2, 4, 3, -1, 1),
            m_despoilerAxeGougeHorn(Weapon::Type::Melee, "Despoiler Axe", 1, 3, 4, 3, -1, 1) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, BESTIGORS};
        m_weapons = {&m_despoilerAxe, &m_despoilerAxeGougeHorn};
    }

    bool Bestigors::configure(int numModels, bool brayhorn, bool bannerBearer) {
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            return false;
        }

        m_brayhorn = brayhorn;
        m_bannerBearer = bannerBearer;

        m_runAndCharge = m_brayhorn;

        auto gougehorn = new Model(BASESIZE, wounds());
        gougehorn->addMeleeWeapon(&m_despoilerAxeGougeHorn);
        addModel(gougehorn);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMeleeWeapon(&m_despoilerAxe);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *Bestigors::Create(const ParameterList &parameters) {
        auto unit = new Bestigors();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool brayhorn = GetBoolParam("Brayhorn", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        bool ok = unit->configure(numModels, brayhorn, bannerBearer);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Bestigors::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
                            BoolParameter("Brayhorn"),
                            BoolParameter("Banner Bearer"),
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Bestigors", factoryMethod);
        }
    }

    int Bestigors::toHitModifier(const Weapon *weapon, const Unit *unit) const {
        // Despoilers
        int modifier = Unit::toHitModifier(weapon, unit);
        if (unit->remainingModels() >= 10) {
            modifier += 1;
        }
        return modifier;
    }

    Rerolls Bestigors::toHitRerolls(const Weapon *weapon, const Unit *unit) const {
        // Despoilers
        if (unit->hasKeyword(ORDER)) {
            return RerollOnes;
        }
        return Unit::toHitRerolls(weapon, unit);
    }

    int Bestigors::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        // Beastial Charge
        int attacks = Unit::extraAttacks(attackingModel, weapon, target);
        if (m_charged) {
            attacks += 1;
        }
        return attacks;
    }

    int Bestigors::runModifier() const {
        int modifier = Unit::runModifier();
        if (m_bannerBearer) {
            modifier += 1;
        }
        return modifier;
    }

    int Bestigors::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }

        return points;
    }

} // namespace BeastsOfChaos