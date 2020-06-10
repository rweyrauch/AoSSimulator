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
    static const int BASESIZE = 60; // x35 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 540;

    bool SpiderRiders::s_registered = false;

    SpiderRiders::SpiderRiders() :
            GloomspiteGitzBase("Spider Riders", 10, WOUNDS, 4, 5, true), // Wall Crawler treated as fly
            m_spiderBow(Weapon::Type::Missile, "Spider-bow", 16, 2, 5, 5, 0, 1),
            m_crookedSpear(Weapon::Type::Melee, "Crooked Spear", 2, 1, 5, 4, 0, 1),
            m_crookedSpearBoss(Weapon::Type::Melee, "Crooked Spear", 2, 1, 4, 4, 0, 1),
            m_fangs(Weapon::Type::Melee, "Fangs", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, GLOOMSPITE_GITZ, SPIDERFANG, GROT, SPIDER_RIDERS};
        m_weapons = {&m_spiderBow, &m_crookedSpear, &m_crookedSpearBoss, &m_fangs};
    }

    bool SpiderRiders::configure(int numModels, bool drummers, bool totemBearers) {
        // validate inputs
        if (numModels < MIN_UNIT_SIZE || numModels > MAX_UNIT_SIZE) {
            // Invalid model count.
            return false;
        }

        m_drummers = drummers;
        m_totemBearers = totemBearers;

        // Add the boss
        auto boss = new Model(BASESIZE, wounds());
        boss->addMissileWeapon(&m_spiderBow);
        boss->addMeleeWeapon(&m_crookedSpearBoss);
        boss->addMeleeWeapon(&m_fangs);
        addModel(boss);

        // and the rest
        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(BASESIZE, wounds());
            model->addMissileWeapon(&m_spiderBow);
            model->addMeleeWeapon(&m_crookedSpear);
            model->addMeleeWeapon(&m_fangs);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *SpiderRiders::Create(const ParameterList &parameters) {
        auto unit = new SpiderRiders();
        int numModels = GetIntParam("Models", parameters, MIN_UNIT_SIZE);
        bool drummers = GetBoolParam("Drummers", parameters, false);
        bool totemBearers = GetBoolParam("Totem Bearers", parameters, false);

        bool ok = unit->configure(numModels, drummers, totemBearers);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void SpiderRiders::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    SpiderRiders::Create,
                    nullptr,
                    nullptr,
                    SpiderRiders::ComputePoints,
                    {
                            IntegerParameter("Models", MIN_UNIT_SIZE, MIN_UNIT_SIZE, MAX_UNIT_SIZE, MIN_UNIT_SIZE),
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
        if (m_totemBearers)
            modifier += 2;
        return modifier;
    }

    int SpiderRiders::runModifier() const {
        int modifier = GloomspiteGitzBase::runModifier();
        if (m_drummers)
            modifier += 2;
        return modifier;
    }

    Wounds SpiderRiders::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Spider Venom
        if (weapon->name() == m_fangs.name() && (hitRoll == 6)) {
            return {weapon->damage(), 1};
        }
        return GloomspiteGitzBase::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int SpiderRiders::ComputePoints(int numModels) {
        auto points = numModels / MIN_UNIT_SIZE * POINTS_PER_BLOCK;
        if (numModels == MAX_UNIT_SIZE) {
            points = POINTS_MAX_UNIT_SIZE;
        }
        return points;
    }

} //namespace GloomspiteGitz
