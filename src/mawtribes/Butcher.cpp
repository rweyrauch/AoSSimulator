/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "mawtribes/Butcher.h"
#include "MawtribesPrivate.h"
#include "MawtribesLore.h"

namespace OgorMawtribes {

    class VoraciousMaw : public Spell {
    public:
        explicit VoraciousMaw(Unit *caster) :
            Spell(caster, "Voracious Maw", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Result::Failed;

            auto mortalWounds = Dice::RollD3();
            while (Dice::RollD6() >= 4) {
                mortalWounds += Dice::RollD3();
            }
            target->applyDamage({0, mortalWounds, Wounds::Source::Spell}, m_caster);

            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    static const int g_basesize = 50;
    static const int g_wounds = 7;
    static const int g_pointsPerUnit = 140;

    bool Butcher::s_registered = false;

    bool Butcher::AreValid(const ParameterList &parameters) {
        return true;
    }

    Unit *Butcher::Create(const ParameterList &parameters) {
        if (AreValid(parameters)) {
            auto tribe = (Mawtribe) GetEnumParam("Mawtribe", parameters, g_mawtribe[0]);
            auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Cleaver);
            auto lore = (Lore) GetEnumParam("Lore", parameters, g_butcherLore[0]);
            auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_butcherTraits[0]);
            auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_butcherArtefacts[0]);
            auto general = GetBoolParam("General", parameters, false);
            return new Butcher(tribe, weapon, lore, trait, artefact, general);
        }
        return nullptr;
    }

    void Butcher::Init() {
        if (!s_registered) {
            static const std::array<int, 2> weapons = {Tenderiser, Cleaver};
            static FactoryMethod factoryMethod = {
                    Butcher::Create,
                    Butcher::ValueToString,
                    Butcher::EnumStringToInt,
                    Butcher::ComputePoints,
                    {
                            EnumParameter("Weapon", Cleaver, weapons),
                            EnumParameter("Mawtribe", g_mawtribe[0], g_mawtribe),
                            EnumParameter("Command Trait", g_butcherTraits[0], g_butcherTraits),
                            EnumParameter("Artefact", g_butcherArtefacts[0], g_butcherArtefacts),
                            EnumParameter("Lore", g_butcherLore[0], g_butcherLore),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {OGOR_MAWTRIBES}
            };
            s_registered = UnitFactory::Register("Butcher", factoryMethod);
        }
    }

    Butcher::Butcher(Mawtribe tribe, WeaponOption weaponOption, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            MawtribesBase(tribe, "Butcher", 6, g_wounds, 8, 5, false) {
        m_keywords = {DESTRUCTION, OGOR, OGOR_MAWTRIBES, GUTBUSTERS, HERO, WIZARD, BUTCHER};
        m_weapons = {&m_tenderizer, &m_cleaver, &m_bite};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());

        if (weaponOption == Tenderiser) {
            model->addMeleeWeapon(&m_tenderizer);
        } else if (weaponOption == Cleaver) {
            model->addMeleeWeapon(&m_cleaver);
        }
        model->addMeleeWeapon(&m_bite);

        addModel(model);

        m_knownSpells.push_back(std::make_unique<VoraciousMaw>(this));
        if (trait != CommandTrait::Gastromancer) {
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        }
        else {
            // Knows all of the spells of Gutmagic
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Fleshcrave_Curse, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Blood_Feast, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Ribcracker, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Blubbergrub_Stench, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Molten_Entrails, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(Lore::Greasy_Deluge, this)));
        }
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        if (hasKeyword(BLOODGULLET)) {
            m_totalSpells++;
            m_totalUnbinds++;
        }

        if (trait == CommandTrait::Questionable_Hygiene) {
            s_globalToHitMod.connect(this, &Butcher::questionableHygiene, &m_questionableHygiene);
        }
        if (trait == CommandTrait::Herald_Of_The_Gulping_God) {
            s_globalBraveryMod.connect(this, &Butcher::heraldOfGulpingGod, &m_heraldOfGulpingGod);
        }
        if (trait == CommandTrait::Growling_Stomach) {
            s_globalBraveryMod.connect(this, &Butcher::growlingStomach, &m_growlingStomach);
        }

        m_points = g_pointsPerUnit;
    }

    Butcher::~Butcher() {
        m_questionableHygiene.disconnect();
        m_heraldOfGulpingGod.disconnect();
        m_growlingStomach.disconnect();
    }

    std::string Butcher::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Tenderiser) return "Tenderiser";
            else if (parameter.intValue == Cleaver) return "Cleaver";
        }
        return MawtribesBase::ValueToString(parameter);
    }

    int Butcher::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Tenderiser") return Tenderiser;
        else if (enumString == "Cleaver") return Cleaver;

        return MawtribesBase::EnumStringToInt(enumString);
    }

    void Butcher::onCastSpell(const Spell *spell, const Unit *target) {
        Unit::onCastSpell(spell, target);

        // Bloodgruel
        const auto roll = Dice::RollD6();
        if (roll == 1) applyDamage({0, 1}, this);
        else heal(1);
    }

    void Butcher::onUnboundSpell(Unit *caster, int castRoll) {
        Unit::onUnboundSpell(caster, castRoll);

        // Bloodgruel
        const auto roll = Dice::RollD6();
        if (roll == 1) applyDamage({0, 1}, this);
        else heal(1);
    }

    int Butcher::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    int Butcher::woundModifier() const {
        auto mod = UnitModifierInterface::woundModifier();
        if (m_commandTrait == CommandTrait::Rolls_Of_Fat) {
            mod += 2;
        }
        return mod;
    }

} // namespace OgorMawtribes
