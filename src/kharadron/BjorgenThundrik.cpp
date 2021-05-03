/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <Board.h>
#include "kharadron/BjorgenThundrik.h"

namespace KharadronOverlords {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 0;

    bool BjorgenThundrik::s_registered = false;

    Unit *BjorgenThundrik::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new BjorgenThundrik(general);
    }

    int BjorgenThundrik::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    void BjorgenThundrik::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BjorgenThundrik::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    BjorgenThundrik::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Bjorgen Thundrik", factoryMethod);
        }
    }

    BjorgenThundrik::BjorgenThundrik(bool isGeneral) :
            KharadronBase(Skyport::Barak_Nar, Artycle::None, Amendment::None, Footnote::None, "Bjorgen Thundrik", 4, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_anatomiser(Weapon::Type::Missile, "Atmospheric Anatomiser", 9, RAND_3D6, 4, 4, -2, 1),
            m_instruments(Weapon::Type::Melee, "Heavy Instruments", 1, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, BARAK_NAR, HERO, SKYFARER, MARINE,
                      AETHER_KHEMIST, BJORGEN_THUNDRIK};
        m_weapons = {&m_anatomiser, &m_instruments};
        m_battleFieldRole = Role::Leader;
        s_globalToHitMod.connect(this, &BjorgenThundrik::atmosphericIsolation, &m_connection);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_anatomiser);
        model->addMeleeWeapon(&m_instruments);
        addModel(model);
    }

    BjorgenThundrik::~BjorgenThundrik() {
        m_connection.disconnect();
    }

    void BjorgenThundrik::onStartHero(PlayerId player) {
        KharadronBase::onStartHero(player);

        // Aetheric Augmentation
        auto skyfarers = Board::Instance()->getUnitsWithin(this, owningPlayer(), 12.0);
        for (auto unit : skyfarers) {
            if (unit->hasKeyword(SKYFARER)) {
                unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Ones,
                                 {GamePhase::Hero, m_battleRound + 1, owningPlayer()});
            }
        }
    }

    int BjorgenThundrik::atmosphericIsolation(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        // Atmospheric Isolation
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

} // namespace KharadronOverlords
