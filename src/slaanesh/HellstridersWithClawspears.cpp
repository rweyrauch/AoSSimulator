/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <slaanesh/HellstridersWithClawspears.h>
#include <UnitFactory.h>
#include "SlaaneshPrivate.h"

namespace Slaanesh {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 20;
    static const int g_pointsPerBlock = 140;
    static const int g_pointsMaxUnitSize = (g_maxUnitSize / g_minUnitSize) * g_pointsPerBlock;

    bool HellstridersWithClawspears::s_registered = false;

    HellstridersWithClawspears::HellstridersWithClawspears() :
            SlaaneshBase("Hellstriders with Claw-spears", 14, g_wounds, 6, 4, false),
            m_clawSpear(Weapon::Type::Melee, "Claw-spear", 1, 1, 3, 4, -1, 1),
            m_clawSpearReaver(Weapon::Type::Melee, "Claw-spear", 1, 2, 3, 4, -1, 1),
            m_poisonedTongue(Weapon::Type::Melee, "Poisoned Tongue", 1, 2, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, SLAANESH, HEDONITE, HELLSTRIDERS};
        m_weapons = {&m_clawSpear, &m_clawSpearReaver, &m_poisonedTongue};
        m_hasMount = true;
        m_poisonedTongue.setMount(true);
        s_globalBattleshockReroll.connect(this, &HellstridersWithClawspears::hornblowerBattleshockReroll,
                                          &m_hornblowerSlot);
    }

    HellstridersWithClawspears::~HellstridersWithClawspears() {
        m_hornblowerSlot.disconnect();
    }

    bool
    HellstridersWithClawspears::configure(int numModels, bool iconBearer, bool bannerBearer, bool hornblower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        auto reaver = new Model(g_basesize, wounds());
        reaver->addMeleeWeapon(&m_clawSpearReaver);
        reaver->addMeleeWeapon(&m_poisonedTongue);
        addModel(reaver);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clawSpear);
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

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *HellstridersWithClawspears::Create(const ParameterList &parameters) {
        auto unit = new HellstridersWithClawspears();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool iconBearer = GetBoolParam("Icon Bearer", parameters, false);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, false);
        bool hornblowers = GetBoolParam("Hornblowers", parameters, false);

        auto host = (Host) GetEnumParam("Host", parameters, g_host[0]);
        unit->setHost(host);

        bool ok = unit->configure(numModels, iconBearer, bannerBearer, hornblowers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void HellstridersWithClawspears::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    SlaaneshBase::ValueToString,
                    SlaaneshBase::EnumStringToInt,
                    ComputePoints,
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
            s_registered = UnitFactory::Register("Hellstriders with Claw-spears", factoryMethod);
        }
    }

    Rerolls HellstridersWithClawspears::chargeRerolls() const {
        if (isNamedModelAlive("Banner Bearer")) {
            return Rerolls::Failed;
        }
        return Unit::chargeRerolls();
    }

    int HellstridersWithClawspears::braveryModifier() const {
        int modifier = Unit::braveryModifier();
        if (isNamedModelAlive(Model::IconBearer)) {
            modifier += 2;
        }
        return modifier;
    }

    int HellstridersWithClawspears::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    Rerolls HellstridersWithClawspears::hornblowerBattleshockReroll(const Unit *unit) {
        if (!isFriendly(unit) && isNamedModelAlive(Model::Hornblower) && (distanceTo(unit) <= 6.0))
            return Rerolls::Ones;

        return Rerolls::None;
    }

    Wounds HellstridersWithClawspears::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                    int woundRoll) const {
        auto wounds = UnitModifierInterface::weaponDamage(weapon, target, hitRoll, woundRoll);
        // Piercing Strike
        if (m_charged && (weapon->name() == m_clawSpear.name())) {
            wounds.normal++;
        }
        return wounds;
    }

} // namespace Slaanesh
