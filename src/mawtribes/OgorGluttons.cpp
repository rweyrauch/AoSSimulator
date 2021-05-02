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

    bool OgorGluttons::AreValid(const ParameterList &parameters) {
        const int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        return ((numModels >= g_minUnitSize) && (numModels <= g_maxUnitSize));
    }

    Unit *OgorGluttons::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            int numModels = GetIntParam("Models", parameters, g_minUnitSize);
            bool skullBearer = GetBoolParam("Beast Skull Bearer", parameters, true);
            bool bannerBearer = GetBoolParam("Banner Bearer", parameters, true);
            bool lookout = GetBoolParam("Lookout Gnoblar", parameters, true);
            bool bellower = GetBoolParam("Bellower", parameters, true);
            auto weapons = (WeaponOption) GetEnumParam("Weapons", parameters, Club_Or_Blade_And_Ironfist);
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            return new OgorGluttons(tribe, numModels, weapons, skullBearer, bannerBearer, lookout, bellower, ComputePoints(parameters));
        }
        return nullptr;
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

    OgorGluttons::~OgorGluttons() {
        m_connection.disconnect();
    }

    OgorGluttons::OgorGluttons(Mawtribe tribe, int numModels, OgorGluttons::WeaponOption option, bool skullBearer,
                               bool bannerBearer, bool lookoutGnoblar, bool bellower, int points) :
            MawtribesBase(tribe, "Ogor Gluttons", 6, g_wounds, 6, 5, false, points) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, OGOR_GLUTTONS};
        m_weapons = {&m_clubOrBlade, &m_bite, &m_clubOrBladeCrusher};
        m_battleFieldRole = Role::Battleline;

        s_globalBraveryMod.connect(this, &OgorGluttons::bellower, &m_connection);

        m_weaponOption = option;
        m_lookoutGnoblar = lookoutGnoblar;

        auto crusher = new Model(g_basesize, wounds());
        crusher->addMeleeWeapon(&m_clubOrBladeCrusher);
        crusher->addMeleeWeapon(&m_bite);
        addModel(crusher);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_clubOrBlade);
            model->addMeleeWeapon(&m_bite);
            if (skullBearer) {
                model->setName("Skull Bearer");
                skullBearer = false;
            } else if (bannerBearer) {
                model->setName(Model::BannerBearer);
                bannerBearer = false;
            } else if (bellower) {
                model->setName("Bellower");
                bellower = false;
            }

            addModel(model);
        }
    }

    int OgorGluttons::braveryModifier() const {
        auto mod = MawtribesBase::braveryModifier();

        // Tribal Banner Bearer
        if (isNamedModelAlive("Banner Bearer")) mod++;

        return mod;
    }

    Rerolls OgorGluttons::chargeRerolls() const {
        // Beast Skull Bearer
        if (isNamedModelAlive("Skull Bearer")) {
            return Rerolls::Failed;
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

    int OgorGluttons::ComputePoints(const ParameterList& parameters) {
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int OgorGluttons::bellower(const Unit *target) {
        // Bellower
        if (isNamedModelAlive("Bellower") && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= 6.0)) {
            return -1;
        }
        return 0;
    }

    Wounds OgorGluttons::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = Unit::applyWoundSave(wounds, attackingUnit);

        // Lookout Gnoblar
        if (m_lookoutGnoblar && (wounds.source == Wounds::Source::Weapon_Missile)) {
            Dice::RollResult result;
            Dice::RollD6(totalWounds.normal, result);
            totalWounds.normal -= result.rollsGE(6);
        }
        return totalWounds;
    }

} // namespace OgorMawtribes
