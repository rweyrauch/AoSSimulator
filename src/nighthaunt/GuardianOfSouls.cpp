/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "nighthaunt/GuardianOfSouls.h"
#include "NighthauntPrivate.h"
#include "NighthauntLore.h"

namespace Nighthaunt {

    class SpectralLure : public Spell {
    public:
        explicit SpectralLure(Unit *caster) :
                Spell(caster, "Spectral Lure", 6, 24) {
            m_allowedTargets = Abilities::Target::Friendly;
            m_effect = Abilities::EffectType::Heal;
            m_targetKeywords = {SUMMONABLE, NIGHTHAUNT};
        }

    protected:

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;

            if (target->numOfWoundedModels()) {
                target->heal(Dice::RollD6());
            } else {
                int woundsRestored = Dice::RollD6();
                int modelsRestored = woundsRestored / target->wounds();
                target->returnModels(modelsRestored);
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool GuardianOfSouls::s_registered = false;

    Unit *GuardianOfSouls::Create(const ParameterList &parameters) {
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        return new GuardianOfSouls(lore, trait, artefact, general);
    }

    void GuardianOfSouls::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    GuardianOfSouls::Create,
                    Nighthaunt::ValueToString,
                    Nighthaunt::EnumStringToInt,
                    GuardianOfSouls::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {NIGHTHAUNT}
            };
            s_registered = UnitFactory::Register("Guardian of Souls with Nightmare Lantern", factoryMethod);
        }
    }

    GuardianOfSouls::GuardianOfSouls(Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Nighthaunt("Guardian of Souls with Nightmare Lantern", 6, g_wounds, 10, 4, true, g_pointsPerUnit) {
        m_keywords = {DEATH, MALIGNANT, NIGHTHAUNT, HERO, WIZARD, GUARDIAN_OF_SOULS};
        m_weapons = {&m_blade, &m_maul};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        s_globalToWoundMod.connect(this, &GuardianOfSouls::nightmareLantern, &m_nightmareLanternSlot);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_blade);
        model->addMeleeWeapon(&m_maul);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<SpectralLure>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    GuardianOfSouls::~GuardianOfSouls() {
        m_nightmareLanternSlot.disconnect();
    }

    int GuardianOfSouls::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int GuardianOfSouls::nightmareLantern(const Unit *attacker, const Weapon *weapon, const Unit *target) {
        if (isFriendly(attacker) && attacker->hasKeyword(NIGHTHAUNT) && weapon->isMelee() && (distanceTo(attacker) < 12.0)) {
            return 1;
        }
        return 0;
    }

} // namespace Nighthaunt
