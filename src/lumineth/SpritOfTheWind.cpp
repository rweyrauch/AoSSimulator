/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <lumineth/SpiritOfTheWind.h>
#include <UnitFactory.h>
#include <Board.h>
#include "LuminethPrivate.h"

namespace LuminethRealmLords {

    static const int g_basesize = 60;
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 250;

    bool HurakanSpiritOfTheWind::s_registered = false;

    Unit *HurakanSpiritOfTheWind::Create(const ParameterList &parameters) {
        auto nation = (GreatNation) GetEnumParam("Nation", parameters, (int) GreatNation::None);
        return new HurakanSpiritOfTheWind(nation);
    }

    int HurakanSpiritOfTheWind::ComputePoints(const ParameterList& parameters) {
        return g_pointsPerUnit;
    }

    void HurakanSpiritOfTheWind::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Nation", g_greatNations[0], g_greatNations),
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Hurakan Spirit of the Wind", factoryMethod);
        }
    }

    HurakanSpiritOfTheWind::HurakanSpiritOfTheWind(GreatNation nation) :
            LuminethBase(nation, "Hurakan Spirit of the Wind", 24, g_wounds, 10, 5, true, g_pointsPerUnit) {
        m_keywords = {ORDER, LUMINETH_REALM_LORDS, HURAKAN, SPIRIT_OF_THE_WIND};
        m_weapons = {&m_bow, &m_bowMelee, &m_shards};
        m_battleFieldRole = Role::Other;

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_bow);
        model->addMeleeWeapon(&m_bowMelee);
        model->addMeleeWeapon(&m_shards);
        addModel(model);
    }

    Wounds HurakanSpiritOfTheWind::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        // Into the Gale
        return ignoreWounds(wounds, 5);
    }

    void HurakanSpiritOfTheWind::onStartHero(PlayerId player) {
        LuminethBase::onStartHero(player);

        // Windmage Symbiosis
        if (owningPlayer() == player) {
            auto mages = Board::Instance()->getUnitWithKeyword(this, owningPlayer(), WINDMAGE, 12.0);
            if (mages && mages->remainingModels() > 0) {
                heal(Dice::RollD3());
            }
        }
    }

}