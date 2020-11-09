/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "mawtribes/OgorGluttons.h"
#include "MawtribesPrivate.h"

namespace OgorMawtribes {
    static const int g_basesize = 40;
    static const int g_wounds = 4;
    static const int g_minUnitSize = 3;
    static const int g_maxUnitSize = 12;
    static const int g_pointsPerBlock = 120;
    static const int g_pointsMaxUnitSize = 400;

    bool OgorGluttons::s_registered = false;

    Unit *OgorGluttons::Create(const ParameterList &parameters) {
        auto unit = new OgorGluttons();

        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        bool skullBearer = GetBoolParam("Beast Skull Bearer", parameters, true);
        bool bannerBearer = GetBoolParam("Banner Bearer", parameters, true);
        bool lookout = GetBoolParam("Lookout Gnoblar", parameters, true);
        bool bellower = GetBoolParam("Bellower", parameters, true);
        auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Club_Or_Blade_And_Ironfist);

        auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
        unit->setMawtribe(tribe);

        bool ok = unit->configure(numModels, weapons, skullBearer, bannerBearer, lookout, bellower);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string OgorGluttons::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Club_Or_Blade_And_Ironfist) return "Club or Blade and Ironfist";
            else if (parameter.intValue == Paired_Club_Or_Blade) return "Paired Club or Blade";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int OgorGluttons::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Club or Blade and Ironfist") return Club_Or_Blade_And_Ironfist;
        else if (enumString == "Paired Club or Blade") return Paired_Club_Or_Blade;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void OgorGluttons::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Club_Or_Blade_And_Ironfist, Paired_Club_Or_Blade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Club_Or_Blade_And_Ironfist, weapons),
                            BoolParameter("Beast Skull Bearer"),
                            BoolParameter("Banner Bearer"),
                            BoolParameter("Lookout Gnoblar"),
                            BoolParameter("Bellower"),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe)
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Ogor Gluttons", factoryMethod);
        }
    }

    OgorGluttons::OgorGluttons() :
            MawtribesBase("Ogor Gluttons", 6, g_wounds, 6, 5, false),
            m_clubOrBlade(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 3, 3, 3, 0, 2),
            m_bite(Weapon::Type::Melee, "Gulping Bite", 1, 1, 3, 3, 0, 1),
            m_clubOrBladeCrusher(Weapon::Type::Melee, "Club(s) or Blade(s)", 1, 4, 3, 3, 0, 2) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, OGOR_GLUTTONS};
        m_weapons = {&m_clubOrBlade, &m_bite, &m_clubOrBladeCrusher};
        m_battleFieldRole = Battleline;

        s_globalBraveryMod.connect(this, &OgorGluttons::bellower, &m_connection);
    }

    OgorGluttons::~OgorGluttons() {
        m_connection.disconnect();
    }

    bool OgorGluttons::configure(int numModels, WeaponOption option, bool skullBearer, bool bannerBearer,
                                 bool lookoutGnoblar, bool bellower) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }

        m_weaponOption = option;
        m_skullBearer = skullBearer;
        m_bannerBearer = bannerBearer;
        m_lookoutGnoblar = lookoutGnoblar;
        m_bellower = bellower;

        auto crusher = new Model(g_basesize, wounds());
        crusher->addMeleeWeapon(&m_clubOrBladeCrusher);
        crusher->addMeleeWeapon(&m_bite);
        addModel(crusher);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clubOrBlade);
            model->addMeleeWeapon(&m_bite);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    int OgorGluttons::braveryModifier() const {
        auto mod = MawtribesBase::braveryModifier();

        // Tribal Banner Bearer
        if (m_bannerBearer) mod++;

        return mod;
    }

    Rerolls OgorGluttons::chargeRerolls() const {
        // Beast Skull Bearer
        if (m_skullBearer) {
            return Reroll_Failed;
        }
        return Unit::chargeRerolls();
    }

    int OgorGluttons::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Paired Clubs or Blades
        if ((unmodifiedHitRoll == 6) && (weapon->name() == m_clubOrBlade.name()) &&
            (m_weaponOption == Paired_Club_Or_Blade)) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    Wounds OgorGluttons::computeReturnedDamage(const Weapon *weapon, int saveRoll) const {
        // Ironfist
        if ((saveRoll == 6) && (m_weaponOption == Club_Or_Blade_And_Ironfist)) {
            return {0, 1};
        }
        return Unit::computeReturnedDamage(weapon, saveRoll);
    }

    int OgorGluttons::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int OgorGluttons::bellower(const Unit *target) {
        // Bellower
        if (m_bellower && (target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 6.0)) {
            return -1;
        }

        return 0;
    }

    Wounds OgorGluttons::applyWoundSave(const Wounds &wounds) {
        auto totalWounds = Unit::applyWoundSave(wounds);

        // Lookout Gnoblar
        if (m_lookoutGnoblar && (wounds.source == Wounds::Source::Weapon_Missile)) {
            Dice::RollResult result;
            Dice::RollD6(totalWounds.normal, result);
            totalWounds.normal -= result.rollsGE(6);
        }

        return totalWounds;
    }

} // namespace OgorMawtribes
