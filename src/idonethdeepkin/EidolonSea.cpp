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
#include "idonethdeepkin/EidolonSea.h"
#include "IdonethDeepkinPrivate.h"
#include "IdonethLore.h"

namespace IdonethDeepkin {

    class CloyingSeaMists : public Spell {
    public:
        explicit CloyingSeaMists(Unit *caster) :
                Spell(caster, "Cloying Sea Mists", 6, 12) {
            m_allowedTargets = Abilities::Target::Any;
            m_effect = Abilities::EffectType::Heal;
        }
    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit* target) override {
            if (target == nullptr) return Result::Failed;
            if (m_caster->isFriendly(target)) {
                target->heal(Dice::RollD3());
            }
            else {
                target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            }
            return Result::Success;
        }
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override { return Result::Failed; }
    };

    class TsunamiOfTerror : public Spell {
    public:
        explicit TsunamiOfTerror(Unit *caster) :
                Spell(caster, "TsunamiOfTerror", 7, 12) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Debuff;
        }
    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit* target) override {
             return apply(castingRoll, unmodifiedCastingRoll, 0, 0);
        }
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x, double y) override {
            auto units = Board::Instance()->getUnitsWithin(m_caster, GetEnemyId(m_caster->owningPlayer()), m_range);
            auto numUnits = std::min(Dice::RollD6(), (int)units.size());
            for (auto i = 0; i < numUnits; i++) {
                units[i]->buffModifier(To_Hit_Missile, -1, defaultDuration());
                units[i]->buffModifier(To_Hit_Melee, -1, defaultDuration());
                units[i]->buffModifier(Bravery, -1, defaultDuration());
            }
            return Result::Success;
        }
    };

    static const int g_basesize = 100;
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 330;

    bool EidolonOfMathlannAspectOfTheSea::s_registered = false;

    Unit *EidolonOfMathlannAspectOfTheSea::Create(const ParameterList &parameters) {
        auto unit = new EidolonOfMathlannAspectOfTheSea();

        auto enclave = (Enclave) GetEnumParam("Enclave", parameters, g_enclave[0]);
        unit->setEnclave(enclave);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_arcaneArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void EidolonOfMathlannAspectOfTheSea::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    IdonethDeepkinBase::ValueToString,
                    IdonethDeepkinBase::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Enclave", g_enclave[0], g_enclave),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_arcaneArtefacts[0], g_arcaneArtefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    ORDER,
                    {IDONETH_DEEPKIN}
            };

            s_registered = UnitFactory::Register("Eidolon of Mathlann Aspect of the Sea", factoryMethod);
        }
    }

    EidolonOfMathlannAspectOfTheSea::EidolonOfMathlannAspectOfTheSea() :
            IdonethDeepkinBase("Eidolon of Mathlann Aspect of the Sea", 10, g_wounds, 10, 3, true),
            m_abyssalEnergy(Weapon::Type::Missile, "Blasts of Abyssal Energy", 15, RAND_D3, 3, 3, -2, 2),
            m_trident(Weapon::Type::Melee, "Psi-trident", 2, 3, 3, 3, -2, 2),
            m_sceptre(Weapon::Type::Melee, "Deep-sea Sceptre", 1, 3, 3, 3, -1, 1),
            m_fangs(Weapon::Type::Melee, "Sharp Fangs", 3, RAND_2D6, 4, 4, 0, 1) {
        m_keywords = {ORDER, AELF, IDONETH_DEEPKIN, EIDOLON, HERO, WIZARD, ASPECT_OF_THE_SEA};
        m_weapons = {&m_abyssalEnergy, &m_trident, &m_sceptre, &m_fangs};
        m_battleFieldRole = Leader;

        s_globalBraveryMod.connect(this, &EidolonOfMathlannAspectOfTheSea::tranquilityOfTheAbyss, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    EidolonOfMathlannAspectOfTheSea::~EidolonOfMathlannAspectOfTheSea() {
        m_connection.disconnect();
    }

    bool EidolonOfMathlannAspectOfTheSea::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_abyssalEnergy);
        model->addMeleeWeapon(&m_trident);
        model->addMeleeWeapon(&m_sceptre);
        model->addMeleeWeapon(&m_fangs);

        addModel(model);

        m_knownSpells.push_back(std::make_unique<CloyingSeaMists>(this));
        m_knownSpells.push_back(std::make_unique<TsunamiOfTerror>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int EidolonOfMathlannAspectOfTheSea::tranquilityOfTheAbyss(const Unit *target) {
        // Tranquility of the Abyss
        if (target->hasKeyword(IDONETH_DEEPKIN) && (target->owningPlayer() == owningPlayer()) &&
            (distanceTo(target) <= 18.0)) {
            return 3;
        }

        return 0;
    }

    int EidolonOfMathlannAspectOfTheSea::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // IdonethDeepkin

