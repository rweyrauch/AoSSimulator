/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Soulmason.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {

    class SoulGuide : public Spell {
    public:
        explicit SoulGuide(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    SoulGuide::SoulGuide(Unit *caster) :
            Spell(caster, "Soul-guide", 6, 24) {
        m_allowedTargets = Abilities::Target::SelfAndFriendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(OSSIARCH_BONEREAPERS);
    }

    Spell::Result SoulGuide::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        target->buffReroll(Attribute::To_Hit_Melee, Rerolls::Ones, defaultDuration());
        target->buffReroll(Attribute::To_Hit_Missile, Rerolls::Ones, defaultDuration());

        return Spell::Result::Success;
    }

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 140;

    bool MortisanSoulmason::s_registered = false;

    Unit *MortisanSoulmason::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortisanCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_soulmasonArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        return new MortisanSoulmason(legion, lore, trait, artefact, general);
    }

    void MortisanSoulmason::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MortisanSoulmason::Create,
                    OssiarchBonereaperBase::ValueToString,
                    OssiarchBonereaperBase::EnumStringToInt,
                    MortisanSoulmason::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_mortisanCommandTraits[0], g_mortisanCommandTraits),
                            EnumParameter("Artefact", g_soulmasonArtefacts[0], g_soulmasonArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortisan Soulmason", factoryMethod);
        }
    }

    MortisanSoulmason::MortisanSoulmason(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            OssiarchBonereaperBase("Mortisan Soulmason", 5, g_wounds, 10, 5, false, g_pointsPerUnit) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_SOULMASON};
        m_weapons = {&m_staff, &m_claws};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setLegion(legion);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<SoulGuide>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int MortisanSoulmason::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
