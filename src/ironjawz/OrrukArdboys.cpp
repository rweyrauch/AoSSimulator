/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <ironjawz/OrrukArdboys.h>
#include <UnitFactory.h>
#include "IronjawzPrivate.h"

namespace Ironjawz {
    static const int g_basesize = 32;
    static const int g_wounds = 2;
    static const int g_minUnitSize = 5;
    static const int g_maxUnitSize = 30;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 600;

    bool OrrukArdboys::s_registered = false;

    OrrukArdboys::OrrukArdboys() :
            Ironjawz("Orruk Ardboys", 4, g_wounds, 6, 4, false),
            m_choppa(Weapon::Type::Melee, "Ardboy Choppa", 1, 2, 3, 3, -1, 1),
            m_bossChoppa(Weapon::Type::Melee, "Ardboy Choppa", 1, 4, 3, 3, -1, 1) {
        m_keywords = {DESTRUCTION, ORRUK, IRONJAWZ, ARDBOYS};
        m_weapons = {&m_choppa, &m_bossChoppa};

        s_globalBraveryMod.connect(this, &OrrukArdboys::glyphBearer, &m_connection);
    }

    OrrukArdboys::~OrrukArdboys() {
        m_connection.disconnect();
    }

    bool OrrukArdboys::configure(int numModels, int numShields, bool drummer, StandardOption standard) {
        // validate inputs
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            // Invalid model count.
            return false;
        }
        const int maxShields = (numModels / 5) * 2;
        if (numShields > maxShields) {
            // Invalid weapon configuration.
            return false;
        }

        m_numShields = numShields;
        m_drummer = drummer;
        m_standardBearer = standard;

        // Add the Boss
        auto bossModel = new Model(g_basesize, wounds());
        bossModel->addMeleeWeapon(&m_bossChoppa);
        addModel(bossModel);

        for (auto i = 1; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMeleeWeapon(&m_choppa);
            addModel(model);
        }

        m_points = ComputePoints(numModels);

        return true;
    }

    Unit *OrrukArdboys::Create(const ParameterList &parameters) {
        auto unit = new OrrukArdboys();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        int numShields = GetIntParam("Shields", parameters, 0);
        bool drummer = GetBoolParam("Drummer", parameters, false);
        StandardOption standard = (StandardOption) GetEnumParam("Standard", parameters, None);

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, g_warclan[0]);
        unit->setWarclan(warclan);

        bool ok = unit->configure(numModels, numShields, drummer, standard);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void OrrukArdboys::Init() {
        if (!s_registered) {
            static const std::array<int, 3> standards = {None, BannerBearer, GlyphBearer};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            IntegerParameter("Shields", 0, 0, g_maxUnitSize / 5 * 2, 1),
                            BoolParameter("Drummer"),
                            EnumParameter("Standard", None, standards),
                            EnumParameter("Warclan", g_warclan[0], g_warclan),
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };

            s_registered = UnitFactory::Register("Orruk Ardboys", factoryMethod);
        }
    }

    int OrrukArdboys::braveryModifier() const {
        auto mod = Ironjawz::braveryModifier();

        if (m_standardBearer == BannerBearer) {
            mod += 2;
        }
        return mod;
    }

    void OrrukArdboys::computeBattleshockEffect(int roll, int &numFled, int &numAdded) const {
        Ironjawz::computeBattleshockEffect(roll, numFled, numAdded);
    }

    int OrrukArdboys::chargeModifier() const {
        int modifier = Ironjawz::chargeModifier();

        if (m_drummer)
            modifier += 2;

        return modifier;
    }

    Wounds OrrukArdboys::applyWoundSave(const Wounds &wounds, Unit* attackingUnit) {
        // Orruk-forged Shields
        if (m_numShields > 0) {
            Dice::RollResult result;
            Dice::RollD6(wounds.normal, result);
            Wounds modWounds = wounds;
            modWounds.normal -= result.numUnmodified6s();
            return modWounds;
        }
        return Ironjawz::applyWoundSave(wounds, attackingUnit);
    }

    std::string OrrukArdboys::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Standard") {
            if (parameter.intValue == None) {
                return "None";
            } else if (parameter.intValue == BannerBearer) {
                return "Banner Bearer";
            } else if (parameter.intValue == GlyphBearer) {
                return "Glyph Bearer";
            }
        }

        return Ironjawz::ValueToString(parameter);
    }

    int OrrukArdboys::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Banner Bearer") {
            return BannerBearer;
        } else if (enumString == "Glyph Bearer") {
            return GlyphBearer;
        }
        return Ironjawz::EnumStringToInt(enumString);
    }

    int OrrukArdboys::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int OrrukArdboys::glyphBearer(const Unit *target) {
        // Gorkamorka Glyph Bearer
        if ((m_standardBearer == GlyphBearer) && (target->owningPlayer() != owningPlayer()) &&
            (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

} // namespace Ironjawz
