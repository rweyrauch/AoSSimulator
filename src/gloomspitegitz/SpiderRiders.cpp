/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/SpiderRiders.h>
#include <UnitFactory.h>

namespace GloomspiteGitz {
    static const int g_basesize = 60; // x35 oval
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 540;

    bool SpiderRiders::s_registered = false;

    SpiderRiders::SpiderRiders(int numModels, bool drummers, bool totemBearers, int points) :
            GloomspiteGitzBase("Spider Riders", 10, g_wounds, 4, 5, true, points), // Wall Crawler treated as fly
            m_spiderBow(Weapon::Type::Missile, "Spider-bow", 16, 2, 5, 5, 0, 1),
            m_crookedSpear(Weapon::Type::Melee, "Crooked Spear", 2, 1, 5, 4, 0, 1),
            m_crookedSpearBoss(Weapon::Type::Melee, "Crooked Spear", 2, 1, 4, 4, 0, 1),
            m_fangs(Weapon::Type::Melee, "Fangs", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, SPIDERFANG, GROT, SPIDER_RIDERS};
        m_weapons = {&m_spiderBow, &m_crookedSpear, &m_crookedSpearBoss, &m_fangs};
        m_hasMount = true;
        m_fangs.setMount(true);

        // Add the boss
        auto boss = new Model(g_basesize, wounds());
        boss->addMissileWeapon(&m_spiderBow);
        boss->addMeleeWeapon(&m_crookedSpearBoss);
        boss->addMeleeWeapon(&m_fangs);
        addModel(boss);

        // and the rest
        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_spiderBow);
            model->addMeleeWeapon(&m_crookedSpear);
            model->addMeleeWeapon(&m_fangs);
            if (drummers) {
                model->setName(Model::Drummer);
                drummers = false;
            } else if (totemBearers) {
                model->setName(Model::TotemBearer);
                totemBearers = false;
            }
            addModel(model);
        }
    }

    Unit *SpiderRiders::Create(const ParameterList &parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool drummers = GetBoolParam("Drummers", parameters, false);
        bool totemBearers = GetBoolParam("Totem Bearers", parameters, false);
        return new SpiderRiders(numModels, drummers, totemBearers, ComputePoints(parameters));
    }

    void SpiderRiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiderRiders::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    SpiderRiders::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            BoolParameter("Drummers"),
                            BoolParameter("Totem Bearers")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Spider Riders", factoryMethod);
        }
    }

    int SpiderRiders::braveryModifier() const {
        int modifier = GloomspiteGitzBase::braveryModifier();
        if (isNamedModelAlive(Model::TotemBearer))
            modifier += 2;
        return modifier;
    }

    int SpiderRiders::runModifier() const {
        int modifier = GloomspiteGitzBase::runModifier();
        if (isNamedModelAlive(Model::Drummer))
            modifier += 2;
        return modifier;
    }

    Wounds SpiderRiders::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Spider Venom
        if (weapon->name() == m_fangs.name() && (hitRoll == 6)) {
            return {weapon->damage(), 1};
        }
        return GloomspiteGitzBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int SpiderRiders::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

} //namespace GloomspiteGitz
