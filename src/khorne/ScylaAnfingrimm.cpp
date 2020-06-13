/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/ScylaAnfingrimm.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 60;
    static const int WOUNDS = 8;
    static const int POINTS_PER_UNIT = 100;

    bool ScylaAnfingrimm::s_registered = false;

    ScylaAnfingrimm::ScylaAnfingrimm() :
            KhorneBase("Scyla Anfingrimm", 8, WOUNDS, 9, 5, false),
            m_brutalFists(Weapon::Type::Melee, "Brutal Fists", 2, RAND_2D6, 4, 3, -1, 1),
            m_serpentineTail(Weapon::Type::Melee, "Serpentine Tail", 3, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, SCYLA_ANFINGRIMM};
        m_weapons = {&m_brutalFists, &m_serpentineTail};

        // Brass Collar of Khorne
        m_totalUnbinds = 1;

        // Beastial Leap
        m_pileInMove = 8;
    }

    bool ScylaAnfingrimm::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_brutalFists);
        model->addMeleeWeapon(&m_serpentineTail);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *ScylaAnfingrimm::Create(const ParameterList &parameters) {
        auto unit = new ScylaAnfingrimm();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ScylaAnfingrimm::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ScylaAnfingrimm::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    ScylaAnfingrimm::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost)
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Scyla Anfingrimm", factoryMethod);
        }
    }

    int ScylaAnfingrimm::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);
        // Raging Fury
        attacks += (WOUNDS - remainingWounds());

        return attacks;
    }

    int ScylaAnfingrimm::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne