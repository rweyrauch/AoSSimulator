/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/CelestialHurricanum.h"
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

    class CometOfCasandora : public Spell {
    public:
        explicit CometOfCasandora(Unit *caster) :
                Spell(caster, "Comet of Casandora", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Spell};
            auto roll = Dice::Roll2D6();
            if (roll > target->move()) {
                wounds.mortal = Dice::RollD6();
            }
            target->applyDamage(wounds, m_caster);

            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 105;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 220;
    static const int g_pointsPerUnitWithBattlemage = 280;

    struct TableEntry {
        int m_move;
        double m_portentsOfBattle;
        int m_stormOfShemtek;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 10, 3},
                    {9,  8,  2},
                    {8,  6,  2},
                    {7,  4,  1},
                    {6,  2,  1}
            };

    bool CelestialHurricanum::s_registered = false;

    Unit *CelestialHurricanum::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto battlemage = GetBoolParam("Battlemage", parameters, true);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new CelestialHurricanum(city, battlemage, lore, drug, trait, artefact, general);
    }

    std::string CelestialHurricanum::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int CelestialHurricanum::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void CelestialHurricanum::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            BoolParameter("Battlemage"),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };

            s_registered = UnitFactory::Register("Celestial Hurricanum", factoryMethod);
        }
    }

    CelestialHurricanum::CelestialHurricanum(City city, bool battlemage, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Celestial Hurricanum", 10, g_wounds, 6, 4, false, battlemage ? g_pointsPerUnitWithBattlemage : g_pointsPerUnit),
            m_stormOfShemtek(Weapon::Type::Missile, "Storm of Shemtek", 18, 1, 0, 0, 0, 0),
            m_wizardStaff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_arcaneTools(Weapon::Type::Melee, "Arcane Tools", 1, 4, 5, 5, 0, 1),
            m_hooves(Weapon::Type::Melee, "Steel-shod Hooves", 1, 4, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, CELESTIAL_HURRICANUM};
        m_weapons = {&m_stormOfShemtek, &m_wizardStaff, &m_arcaneTools, &m_hooves};
        m_hasMount = true;
        m_hooves.setMount(true);
        m_battleFieldRole = Role::Behemoth;

        s_globalCastMod.connect(this, &CelestialHurricanum::locusOfAzyr, &m_locusConnection);
        s_globalToHitMod.connect(this, &CelestialHurricanum::portentsOfBattle, &m_portentsConnection);

        setNarcotic(narcotic);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        if (battlemage) {
            addKeyword(WIZARD);
            addKeyword(HERO);
            m_battleFieldRole = Role::Leader_Behemoth;
            m_totalSpells = 1;
            m_totalUnbinds = 1;
        }

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_stormOfShemtek);
        model->addMeleeWeapon(&m_arcaneTools);
        model->addMeleeWeapon(&m_hooves);
        if (battlemage) {
            model->addMeleeWeapon(&m_wizardStaff);
        }
        addModel(model);

        if (battlemage) {
            m_knownSpells.push_back(std::make_unique<ChainLightning>(this));
            m_knownSpells.push_back(std::make_unique<CometOfCasandora>(this));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
            m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        }
    }

    CelestialHurricanum::~CelestialHurricanum() {
        m_locusConnection.disconnect();
        m_portentsConnection.disconnect();
    }

    void CelestialHurricanum::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void CelestialHurricanum::onWounded() {
        CitizenOfSigmar::onWounded();

        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t CelestialHurricanum::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    void CelestialHurricanum::onStartShooting(PlayerId player) {
        CitizenOfSigmar::onStartShooting(player);

        if (m_shootingTarget) {
            // Storm of Shemtek
            auto dist = distanceTo(m_shootingTarget);
            if (dist <= (double) m_stormOfShemtek.range()) {
                Dice::RollResult result;
                Dice::RollD6(g_damageTable[getDamageTableIndex()].m_stormOfShemtek, result);
                auto numHits = result.rollsGE(2);
                for (auto i = 0; i < numHits; i++) {
                    auto mw = Dice::RollD3();
                    m_shootingTarget->applyDamage({0, mw}, this);
                }
            }
        }
    }

    int CelestialHurricanum::castingModifier() const {
        auto mod = CitizenOfSigmar::castingModifier();

        // Celestial Battlemage
        if (Board::Instance()->getRealm() == Realm::Azyr) mod++;

        return mod;
    }

    int CelestialHurricanum::locusOfAzyr(const Unit *caster) {
        // Locus of Azyr
        if (caster->hasKeyword(COLLEGIATE_ARCANE) && caster->hasKeyword(WIZARD) &&
            (caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    int CelestialHurricanum::portentsOfBattle(const Unit * /*attacker*/, const Weapon * /*weapon*/, const Unit *unit) {
        // Portents of Battle
        if (unit->hasKeyword(CITIES_OF_SIGMAR) &&
            (unit->owningPlayer() == owningPlayer()) &&
            (distanceTo(unit) <= g_damageTable[getDamageTableIndex()].m_portentsOfBattle)) {
            return 1;
        }
        return 0;
    }

    int CelestialHurricanum::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace CitiesOfSigmar