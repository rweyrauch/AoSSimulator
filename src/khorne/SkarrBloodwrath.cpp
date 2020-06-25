/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/SkarrBloodwrath.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 100;

    bool SkarrBloodwrath::s_registered = false;

    SkarrBloodwrath::SkarrBloodwrath() :
            KhorneBase("Skarr Bloodwrath", 5, WOUNDS, 9, 4, false),
            m_blades(Weapon::Type::Melee, "Bloodstorm Blades", 3, 5, 2, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKARR_BLOODWRATH};
        m_weapons = {&m_blades};
    }

    bool SkarrBloodwrath::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *SkarrBloodwrath::Create(const ParameterList &parameters) {
        auto unit = new SkarrBloodwrath();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        unit->setSlaughterHost(host);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SkarrBloodwrath::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SkarrBloodwrath::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    SkarrBloodwrath::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Skarr Bloodwrath", factoryMethod);
        }
    }

    int SkarrBloodwrath::extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const {
        int attacks = KhorneBase::extraAttacks(attackingModel, weapon, target);
        // Slaughterstorm
        auto units = Board::Instance()->getUnitsWithin(this, GetEnemyId(owningPlayer()), 3.0);
        int totalModels = 0;
        for (auto ip : units) {
            // TODO: check for models within 3"
            totalModels += ip->remainingModels();
        }
        attacks += std::max(totalModels - m_blades.attacks(), 0);

        return attacks;
    }

    int SkarrBloodwrath::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne