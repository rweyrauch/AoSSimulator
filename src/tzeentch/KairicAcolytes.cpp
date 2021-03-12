/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <tzeentch/KairicAcolytes.h>
#include <Board.h>
#include <UnitFactory.h>
#include "TzeentchPrivate.h"

namespace Tzeentch {

    class GestaltSorcery : public Spell {
    public:
        explicit GestaltSorcery(Unit *caster) :
                Spell(caster, "Gestalt Sorcery", 6, 9) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
            m_targetKeywords.push_back(KAIRIC_ACOLYTES);
        }

    protected:
        Spell::Result
        apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) {
                return Spell::Result::Failed;
            }
            auto result = Spell::Result::Failed;

            auto ka = dynamic_cast<KairicAcolytes *>(target);
            if (ka) {
                ka->activateGestaltSorcery();
                result = Spell::Result::Success;;
            }
            return result;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 1;
    static const int g_minUnitSize = 10;
    static const int g_maxUnitSize = 40;
    static const int g_pointsPerBlock = 100;
    static const int g_pointsMaxUnitSize = 400;

    bool KairicAcolytes::s_registered = false;

    KairicAcolytes::KairicAcolytes() :
            TzeentchBase("Kairic Acolytes", 6, g_wounds, 5, 5, false),
            m_sorcerousBolt(Weapon::Type::Missile, "Sorcerous Bolt", 18, 1, 4, 3, 0, 1),
            m_cursedBlade(Weapon::Type::Melee, "Cursed Blade", 1, 1, 4, 3, 0, 1),
            m_cursedGlaive(Weapon::Type::Melee, "Cursed Glaive", 1, 1, 4, 3, -1, 2),
            m_cursedBladeAdept(Weapon::Type::Melee, "Cursed Blade", 1, 2, 4, 3, 0, 1),
            m_cursedGlaiveAdept(Weapon::Type::Melee, "Cursed Glaive", 1, 2, 4, 3, -1, 2) {
        m_keywords = {CHAOS, MORTAL, TZEENTCH, ARCANITE, KAIRIC_ACOLYTES};
        m_weapons = {&m_sorcerousBolt, &m_cursedBlade, &m_cursedGlaive, &m_cursedBladeAdept, &m_cursedGlaiveAdept};
        m_battleFieldRole = Role::Battleline;
        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool KairicAcolytes::configure(int numModels, WeaponOptions weapons, int numCursedGlaives, int numScrollsOfDarkArts,
                                   int numVulcharcs) {
        if (numModels < g_minUnitSize || numModels > g_maxUnitSize) {
            return false;
        }
        const int maxGlaives = (numModels / 10) * 3;
        if (numCursedGlaives > maxGlaives) {
            // Invalid weapon configuration.
            return false;
        }
        const int maxScrolls = numModels / 10;
        const int maxVulcharcs = numModels / 10;
        if (numScrollsOfDarkArts > maxScrolls || numVulcharcs > maxVulcharcs) {
            return false;
        }

        m_weaponOption = weapons;
        m_numCursedGlaives = numCursedGlaives;
        m_numScrollsOfDarkArts = numScrollsOfDarkArts;
        m_numVulcharcs = numVulcharcs;

        auto adept = new Model(g_basesize, wounds());
        adept->addMissileWeapon(&m_sorcerousBolt);
        adept->addMeleeWeapon(&m_cursedBladeAdept);
        addModel(adept);

        for (auto i = 0; i < m_numCursedGlaives; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_sorcerousBolt);
            model->addMeleeWeapon(&m_cursedGlaive);
            addModel(model);
        }

        int currentModelCount = (int) m_models.size();
        for (auto i = currentModelCount; i < numModels; i++) {
            auto model = new Model(g_basesize, wounds());
            model->addMissileWeapon(&m_sorcerousBolt);
            model->addMeleeWeapon(&m_cursedBlade);
            addModel(model);
        }

        m_knownSpells.push_back(std::make_unique<GestaltSorcery>(this));

        m_points = ComputePoints(numModels);

        return true;
    }

    Wounds KairicAcolytes::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = TzeentchBase::applyWoundSave(wounds, attackingUnit);

        // Arcanite Shield
        if (m_weaponOption == Cursed_Blade_And_Shield) {
            totalWounds = ignoreWounds(totalWounds, 6);
        }
        return totalWounds;
    }

    void KairicAcolytes::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Cursed_Blade, Paired_Cursed_Blades, Cursed_Blade_And_Shield};
            static FactoryMethod factoryMethod = {
                    KairicAcolytes::Create,
                    KairicAcolytes::ValueToString,
                    KairicAcolytes::EnumStringToInt,
                    KairicAcolytes::ComputePoints,
                    {
                            IntegerParameter("Models", g_minUnitSize, g_minUnitSize, g_maxUnitSize, g_minUnitSize),
                            EnumParameter("Weapons", Cursed_Blade, weapons),
                            IntegerParameter("Cursed Glaives", 0, 0, g_maxUnitSize / g_minUnitSize * 3, 1),
                            IntegerParameter("Scrolls Of Dark Arts", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            IntegerParameter("Vulcharcs", 0, 0, g_maxUnitSize / g_minUnitSize, 1),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Kairic Acolytes", factoryMethod);
        }
    }

    Unit *KairicAcolytes::Create(const ParameterList &parameters) {
        auto unit = new KairicAcolytes();
        int numModels = GetIntParam("Models", parameters, g_minUnitSize);
        auto weapons = (WeaponOptions) GetEnumParam("Weapons", parameters, Cursed_Blade);
        int numCursedGlaives = GetIntParam("Cursed Glaives", parameters, 0);
        int numScrollsOfDarkArts = GetIntParam("Scrolls Of Dark Arts", parameters, 0);
        int numVulcharcs = GetIntParam("Vulcharcs", parameters, 0);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, g_changeCoven[0]);
        unit->setChangeCoven(coven);

        bool ok = unit->configure(numModels, weapons, numCursedGlaives, numScrollsOfDarkArts, numVulcharcs);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string KairicAcolytes::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapons") {
            if (parameter.intValue == Cursed_Blade) {
                return "Cursed Blade";
            } else if (parameter.intValue == Paired_Cursed_Blades) {
                return "Paired Cursed Blades";
            } else if (parameter.intValue == Cursed_Blade_And_Shield) {
                return "Cursed Blade And Shield";
            }
        }

        return TzeentchBase::ValueToString(parameter);
    }

    int KairicAcolytes::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Cursed Blade") {
            return Cursed_Blade;
        } else if (enumString == "Paired Cursed Blades") {
            return Paired_Cursed_Blades;
        } else if (enumString == "Cursed Blade And Shield") {
            return Cursed_Blade_And_Shield;
        }
        return TzeentchBase::EnumStringToInt(enumString);
    }

    int KairicAcolytes::castingModifier() const {
        int modifier = TzeentchBase::castingModifier();

        // Scroll of Dark Arts
        if (m_numScrollsOfDarkArts) modifier++;

        return modifier;
    }

    Rerolls KairicAcolytes::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Paired Cursed Blades
        if ((m_weaponOption == Paired_Cursed_Blades) && (weapon->name() == m_cursedBlade.name())) {
            return Rerolls::Failed;
        }
        return TzeentchBase::toHitRerolls(weapon, target);
    }

    int KairicAcolytes::ComputePoints(int numModels) {
        auto points = numModels / g_minUnitSize * g_pointsPerBlock;
        if (numModels == g_maxUnitSize) {
            points = g_pointsMaxUnitSize;
        }
        return points;
    }

    int KairicAcolytes::weaponRend(const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        if (m_gestaltSorceryActive && (weapon->name() == m_sorcerousBolt.name())) {
            return weapon->rend() - 1;
        }
        return TzeentchBase::weaponRend(weapon, target, hitRoll, woundRoll);
    }

    void KairicAcolytes::onRestore() {
        TzeentchBase::onRestore();

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

} //namespace Tzeentch