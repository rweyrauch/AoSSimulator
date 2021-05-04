/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nighthaunt/ReikenorGrimhailer.h"
#include "NighthauntPrivate.h"
#include "NighthauntLore.h"

namespace Nighthaunt {

    class Wraithstorm : public Spell {
    public:
        explicit Wraithstorm(Unit *caster) :
                Spell(caster, "Wraithstorm", 7, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;
            auto numSlain = target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            if (numSlain > 0) {
                target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 75; // x42 oval
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 160;

    bool ReikenorTheGrimhailer::s_registered = false;

    Unit *ReikenorTheGrimhailer::Create(const ParameterList &parameters) {
        auto procession = (Procession) GetEnumParam("Procession", parameters, g_processions[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ReikenorTheGrimhailer(procession, lore, general);
    }

    void ReikenorTheGrimhailer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ReikenorTheGrimhailer::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    ReikenorTheGrimhailer::ComputePoints,
                    {
                            EnumParameter("Procession", g_processions[0], g_processions),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Reikenor the Grimhailer", factoryMethod);
        }
    }

    ReikenorTheGrimhailer::ReikenorTheGrimhailer(Procession procession, Lore lore, bool isGeneral) :
            Nighthaunt(procession, "Reikenor the Grimhailer", 14, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_fellreaper(Weapon::Type::Melee, "Fellreaper", 2, 4, 4, 3, -1, 2),
            m_hoovesAndTeeth(Weapon::Type::Melee, "Ghostly Hooves and Teeth", 1, 3, 4, 4, 0, 1) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, REIKENOR_THE_GRIMHAILER};
        m_weapons = {&m_fellreaper, &m_hoovesAndTeeth};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_hoovesAndTeeth.setMount(true);
        m_totalSpells = 1;
        m_totalUnbinds = 1;

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_fellreaper);
        model->addMeleeWeapon(&m_hoovesAndTeeth);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<Wraithstorm>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    Wounds
    ReikenorTheGrimhailer::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Frightful Touch
        if ((hitRoll == 6) && (weapon->name() == m_fellreaper.name())) {
            return {0, 2};
        }

        return Unit::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    Rerolls ReikenorTheGrimhailer::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Reaped Like Corn
        if ((target->remainingModels() >= 5) && (weapon->name() == m_fellreaper.name())) {
            return Rerolls::Failed;
        }

        return Nighthaunt::toHitRerolls(weapon, target);
    }

    int ReikenorTheGrimhailer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Nighthaunt
