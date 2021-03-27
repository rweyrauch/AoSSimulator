/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Vokmortian.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {

    class MortalTouch : public Spell {
    public:
        explicit MortalTouch(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Result::Failed; }
    };

    MortalTouch::MortalTouch(Unit *caster) :
            Spell(caster, "Mortal Touch", 8, 1) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result
    MortalTouch::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        if (Dice::RollD6() >= 5) {
            target->slay(1);
        }

        return Spell::Result::Success;
    }

    static const int g_basesize = 40;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 180;

    bool Vokmortian::s_registered = false;

    Unit *Vokmortian::Create(const ParameterList &parameters) {
        auto unit = new Vokmortian();

        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        unit->setLegion(legion);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        unit->configure(lore);
        return unit;
    }

    std::string Vokmortian::ValueToString(const Parameter &parameter) {
        return OssiarchBonereaperBase::ValueToString(parameter);
    }

    int Vokmortian::EnumStringToInt(const std::string &enumString) {
        return OssiarchBonereaperBase::EnumStringToInt(enumString);
    }

    void Vokmortian::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };

            s_registered = UnitFactory::Register("Vokmortian", factoryMethod);
        }
    }

    Vokmortian::Vokmortian() :
            OssiarchBonereaperBase("Vokmortian", 5, g_wounds, 10, 5, false),
            m_gazeOfDeath(Weapon::Type::Missile, "Gaze of Death", 12, 1, 3, 2, -1, RAND_D3),
            m_staff(Weapon::Type::Melee, "Staff of Retribution", 2, 2, 3, 3, -1, RAND_D3) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTIS_PRAETORIANS, HERO, WIZARD, VOKMORTIAN};
        m_weapons = {&m_gazeOfDeath, &m_staff};
        m_battleFieldRole = Role::Leader;

        s_globalBraveryMod.connect(this, &Vokmortian::grimWarning, &m_connection);

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    Vokmortian::~Vokmortian() {
        m_connection.disconnect();
    }

    void Vokmortian::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_gazeOfDeath);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_lore = lore;

        m_knownSpells.push_back(std::make_unique<MortalTouch>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int Vokmortian::grimWarning(const Unit *unit) {
        // Grim Warning
        if ((unit->owningPlayer() != owningPlayer()) && (distanceTo(unit) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    int Vokmortian::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
