/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <Board.h>
#include "citiesofsigmar/Battlemage.h"
#include "CitiesOfSigmarPrivate.h"
#include "CoSLore.h"

namespace CitiesOfSigmar {

    class ChainLightning : public Spell {
    public:
        explicit ChainLightning(Unit *caster) :
                Spell(caster, "Chain Lightning", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            auto units = Board::Instance()->getUnitsWithin(target, target->owningPlayer(), 6.0);
            for (auto unit : units) {
                if (Dice::RollD6() >= 4) {
                    unit->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
                }
            }
            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    class Fireball : public Spell {
    public:
        explicit Fireball(Unit *caster) :
                Spell(caster, "Fireball", 5, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            Wounds wounds = {0, 1, Wounds::Source::Spell};
            if (target->remainingModels() >= 10) {
                wounds.mortal = Dice::RollD6();
            } else if (target->remainingModels() >= 2) {
                wounds.mortal = Dice::RollD3();
            }
            target->applyDamage(wounds, m_caster);

            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    class MystifyingMiasma : public Spell {
    public:
        explicit MystifyingMiasma(Unit *caster) :
                Spell(caster, "Mystifying Miasma", 4, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->buffMovement(MovementRule::Can_Run, false, defaultDuration());
            target->buffModifier(Attribute::Charge_Distance, -2, defaultDuration());

            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    class TransmutationOfLead : public Spell {
    public:
        explicit TransmutationOfLead(Unit *caster) :
                Spell(caster, "Transmutation of Lead", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->buffMovement(MovementRule::Halve_Movement, true, defaultDuration());
            if (target->save() <= 4) {
                target->buffReroll(Attribute::Target_To_Hit_Missile, Rerolls::Ones, defaultDuration());
                target->buffReroll(Attribute::Target_To_Hit_Melee, Rerolls::Ones, defaultDuration());
            }
            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 110;

    bool Battlemage::s_registered = false;

    Unit *Battlemage::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto realm = (Realm) GetEnumParam("Realm", parameters, g_realm[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Battlemage(city, realm, lore, drug, trait, artefact, general);
    }

    std::string Battlemage::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Battlemage::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Battlemage::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Battlemage::Create,
                    Battlemage::ValueToString,
                    Battlemage::EnumStringToInt,
                    Battlemage::ComputePoints,
                    {
                            EnumParameter("Realm", g_realm[0], g_realm),
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Battlemage", factoryMethod);
        }
    }

    Battlemage::Battlemage(City city, Realm realm, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Battlemage", 5, g_wounds, 6, 6, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, HERO, WIZARD, BATTLEMAGE};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;
        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setNarcotic(narcotic);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        switch (realm) {
            case Realm::Azyr:
                m_knownSpells.push_back(std::make_unique<ChainLightning>(this));
                break;
            case Realm::Aqshy:
                m_knownSpells.push_back(std::make_unique<Fireball>(this));
                break;
            case Realm::Ulgu:
                m_knownSpells.push_back(std::make_unique<MystifyingMiasma>(this));
                break;
            case Realm::Shyish:
                m_knownSpells.push_back(
                        std::make_unique<BuffModifierSpell>(this, "Pall of Doom", 6, 18, Attribute::Bravery, -2,
                                                            Abilities::Target::Enemy));
                break;
            case Realm::Hysh:
                m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Pha's Protection", 5, 18,
                                                                            std::vector<std::pair<Attribute, int>>{{Attribute::Target_To_Hit_Missile, -1},
                                                                                                                   {Attribute::Target_To_Hit_Melee,   -1}},
                                                                            Abilities::Target::SelfAndFriendly));
                break;
            case Realm::Ghyran:
                break;
            case Realm::Chamon:
                m_knownSpells.push_back(std::make_unique<TransmutationOfLead>(this));
                break;
            case Realm::Ghur:
                m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Wildform", 5, 12,
                                                                            std::vector<std::pair<Attribute, int>>{{Attribute::Run_Distance,    2},
                                                                                                                   {Attribute::Charge_Distance, 2}},
                                                                            Abilities::Target::SelfAndFriendly));
                break;
            default:
                break;
        }
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_realm = realm;
    }

    int Battlemage::castingModifier() const {
        auto mod = CitizenOfSigmar::castingModifier();

        // Magic of the Realms
        if (m_realm == Board::Instance()->getRealm()) mod++;

        return mod;
    }

    int Battlemage::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

}//namespace CitiesOfSigmar