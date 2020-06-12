/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <beastsofchaos/Beastlord.h>
#include <UnitFactory.h>
#include "BeastsOfChaosPrivate.h"

namespace BeastsOfChaos {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 90;

    bool Beastlord::s_registered = false;

    Beastlord::Beastlord() :
            BeastsOfChaosBase("Beastlord", 6, WOUNDS, 7, 4, false),
            m_pairedAxes(Weapon::Type::Melee, "Paired Man-ripper Axes", 1, 6, 3, 3, -1, 1) {
        m_keywords = {CHAOS, GOR, BEASTS_OF_CHAOS, BRAYHERD, HERO, BEASTLORD};
        m_weapons.push_back(&m_pairedAxes);
    }

    bool Beastlord::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_pairedAxes);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Beastlord::Create(const ParameterList &parameters) {
        auto unit = new Beastlord();

        auto fray = (Greatfray) GetEnumParam("Greatfray", parameters, g_greatFray[0]);
        unit->setGreatfray(fray);

        // TODO: set command trait
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_brayherdCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_brayherdArtefact[0]);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Beastlord::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    BeastsOfChaosBase::ValueToString,
                    BeastsOfChaosBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Greatfray", g_greatFray[0], g_greatFray),
                            EnumParameter("Command Trait", g_brayherdCommandTrait[0], g_brayherdCommandTrait),
                            EnumParameter("Artefact", g_brayherdArtefact[0], g_brayherdArtefact)
                    },
                    CHAOS,
                    {BEASTS_OF_CHAOS}
            };

            s_registered = UnitFactory::Register("Beastlord", factoryMethod);
        }
    }

    Rerolls Beastlord::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Dual Axes
        if (weapon->name() == m_pairedAxes.name()) {
            return RerollOnes;
        }
        return Unit::toHitRerolls(weapon, target);
    }

    Rerolls Beastlord::toWoundRerolls(const Weapon *weapon, const Unit *target) const {
        // Hatred of Heroes
        if (target->hasKeyword(HERO)) {
            return RerollFailed;
        }
        return Unit::toWoundRerolls(weapon, target);
    }

    int Beastlord::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace BeastsOfChaos
