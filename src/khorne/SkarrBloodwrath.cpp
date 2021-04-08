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
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool SkarrBloodwrath::s_registered = false;

    SkarrBloodwrath::SkarrBloodwrath(SlaughterHost host, bool isGeneral) :
            KhorneBase("Skarr Bloodwrath", 5, g_wounds, 9, 4, false),
            m_blades(Weapon::Type::Melee, "Bloodstorm Blades", 3, 5, 2, 3, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, SKARR_BLOODWRATH};
        m_weapons = {&m_blades};
        m_battleFieldRole = Role::Leader;

        setSlaughterHost(host);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blades);
        addModel(model);

        m_points = g_pointsPerUnit;
    }

    Unit *SkarrBloodwrath::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new SkarrBloodwrath(host, general);
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
            // Check for models within 3"
            totalModels += ip->numModelsWithin(m_models.front().get(), 3.0f);
        }
        attacks += std::max(totalModels - m_blades.attacks(), 0);

        return attacks;
    }

    int SkarrBloodwrath::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void SkarrBloodwrath::onEndMovement(PlayerId player) {
        KhorneBase::onEndMovement(player);

        // The Slaughterborn
        if (remainingModels() == 0) {
            if (Dice::Roll2D6() >= 8) {
                restore();
                // TODO: redeploy 9" from enemy models
                deploy(position(), orientation());
            }
        }
    }

} // namespace Khorne