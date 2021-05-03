/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/HellstridersWithHellscourges.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 150;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool HellstridersWithHellscourges::s_registered = false;

    HellstridersWithHellscourges::HellstridersWithHellscourges(Host host, int numModels, bool iconBearer, bool bannerBearer, bool hornblower, int points) :
            SlaaneshBase(host, "Hellstriders with Hellscourges", 14, g_wounds, 6, 4, false, points),
            m_hellscourge(Weapon::Type::Melee, "Hellscourge", 3, 2, 3, 4, 0, 1),
            m_hellscourgeReaver(Weapon::Type::Melee, "Hellscourge", 3, 3, 3, 4, 0, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HELLSTRIDERS};
        m_weapons = {&m_hellscourge, &m_hellscourgeReaver, &m_poisonedTongue};
        m_hasMount = true;
        m_poisonedTongue.setMount(true);
        s_globalBattleshockReroll.connect(this, &HellstridersWithHellscourges::hornblowerBattleshockReroll,
                                          &m_hornblowerSlot);


        auto reaver = new Model(g_basesize, wounds());
        reaver->addMeleeWeapon(&m_hellscourgeReaver);
        reaver->addMeleeWeapon(&m_poisonedTongue);
        addModel(reaver);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_hellscourge);
            model->addMeleeWeapon(&m_poisonedTongue);
            if (iconBearer) {
                model->setName(Model::IconBearer);
                iconBearer = false;
            } else if (bannerBearer) {
                model->setName(Model::BannerBearer);
                bannerBearer = false;
            } else if (hornblower) {
                model->setName(Model::Hornblower);
                hornblower = false;
            }
            addModel(model);
        }
    }

    HellstridersWithHellscourges::~HellstridersWithHellscourges() {
        m_hornblowerSlot.disconnect();
    }

    Unit *HellstridersWithHellscourges::Create(const ParameterList &parameters) {
        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);
        return new HellstridersWithHellscourges(host, numModels, iconBearer, bannerBearer, hornblowers, ComputePoints(parameters));
    }

    void HellstridersWithHellscourges::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    HellstridersWithHellscourges::Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    HellstridersWithHellscourges::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Icon Bearer"),
                            BoolParameter("Banner Bearer"),
                            BoolParameter("Hornblower"),
                            EnumParameter("Host", g_host[0], g_host),
                    },
                    CHAOS,
                    {SLAANESH}
            };
            s_registered = UnitFactory::Register("Hellstriders with Hellscourges", factoryMethod);
        }
    }

    Rerolls HellstridersWithHellscourges::chargeRerolls() const {
        if (isNamedModelAlive("Banner Bearer")) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    int HellstridersWithHellscourges::braveryModifier() const {
        int modifier = Unit::braveryModifier();
        if (isNamedModelAlive(Model::IconBearer)) {
            modifier += 2;
        }
        return modifier;
    }

    int HellstridersWithHellscourges::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls HellstridersWithHellscourges::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && isNamedModelAlive(Model::Hornblower) && (distanceTo(unit) <= 6.0))
            return Rerolls::Ones;

        return Rerolls::None;
    }

    int HellstridersWithHellscourges::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        auto mod = Unit::targetHitModifier(weapon, attacker);

        // Hooked Tendrils
        if (m_charged && (!weapon->isMissile()) && (distanceTo(attacker) <= 3.0)) {
            mod--;
        }

        return mod;
    }

} // namespace Slaanesh
