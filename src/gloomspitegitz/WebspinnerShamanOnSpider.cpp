/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/WebspinnerShamanOnSpider.h>
#include <UnitFactory.h>
#include <iostream>
#include <spells/MysticShield.h>

namespace GloomspiteGitz {
    static const int BASESIZE = 160;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 280;

    struct TableEntry {
        int m_move;
        int m_legsAttacks;
        int m_fangsToHit;
    };

    const size_t NUM_TABLE_ENTRIES = 5;
    const int g_woundThresholds[NUM_TABLE_ENTRIES] = {2, 4, 7, 9, WOUNDS};
    const TableEntry g_damageTable[NUM_TABLE_ENTRIES] =
            {
                    {8, 8, 2},
                    {7, 7, 3},
                    {6, 6, 3},
                    {5, 5, 4},
                    {4, 4, 4}
            };

    bool WebspinnerShamanOnArachnarokSpider::s_registered = false;

    WebspinnerShamanOnArachnarokSpider::WebspinnerShamanOnArachnarokSpider() :
            GloomspiteGitzBase("Webspinner Shaman on Arachnarok Spider", 8, WOUNDS, 6, 4, true),
            m_spiderBows(Weapon::Type::Missile, "Spider-bows", 16, 8, 5, 5, 0, 1),
            m_spiderGodStaff(Weapon::Type::Melee, "Spider God Staff", 1, 1, 4, 3, -1, RAND_D3),
            m_chitinousLegs(Weapon::Type::Melee, "Chitinous Legs", 3, 8, 4, 3, -1, 1),
            m_monstrousFangs(Weapon::Type::Melee, "Monstrous Fangs", 1, 4, 2, 3, -1, RAND_D3),
            m_crookedSpears(Weapon::Type::Melee, "Crooked Spears", 1, 8, 5, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ARACHNAROK_SPIDER, GLOOMSPITE_GITZ, SPIDERFANG, MONSTER, HERO, WIZARD,
                      WEBSPINNER_SHAMAN};
        m_weapons = {&m_spiderBows, &m_spiderGodStaff, &m_chitinousLegs, &m_monstrousFangs, &m_crookedSpears};

        s_globalCastMod.connect(this, &WebspinnerShamanOnArachnarokSpider::catchwebSpidershrine, &m_shrineConnection);
        s_globalBraveryMod.connect(this, &WebspinnerShamanOnArachnarokSpider::prophetOfTheSpiderGod,
                                   &m_prophetConnection);

        m_totalUnbinds = 2;
        m_totalSpells = 2;
    }

    WebspinnerShamanOnArachnarokSpider::~WebspinnerShamanOnArachnarokSpider() {
        m_shrineConnection.disconnect();
        m_prophetConnection.disconnect();
    }

    bool WebspinnerShamanOnArachnarokSpider::configure(Lore lore) {
        auto model = new Model(BASESIZE, wounds());
        model->addMissileWeapon(&m_spiderBows);
        model->addMeleeWeapon(&m_spiderGodStaff);
        model->addMeleeWeapon(&m_chitinousLegs);
        model->addMeleeWeapon(&m_monstrousFangs);
        model->addMeleeWeapon(&m_crookedSpears);
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        //m_knownSpells.push_back(std::make_unique<SpeedOfTheSpiderGod>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void WebspinnerShamanOnArachnarokSpider::onRestore() {
        // Reset table-driven attributes
        onWounded();
    }

    void WebspinnerShamanOnArachnarokSpider::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_monstrousFangs.setToHit(g_damageTable[damageIndex].m_fangsToHit);
        m_chitinousLegs.setAttacks(g_damageTable[damageIndex].m_legsAttacks);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int WebspinnerShamanOnArachnarokSpider::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < NUM_TABLE_ENTRIES; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    Unit *WebspinnerShamanOnArachnarokSpider::Create(const ParameterList &parameters) {
        auto unit = new WebspinnerShamanOnArachnarokSpider();
        auto lore = (Lore) GetEnumParam("Lore of the Moonclans", parameters,
                                                        (int) None);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void WebspinnerShamanOnArachnarokSpider::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Lore of the Spiderfangs", None, g_loreOfTheSpiderFangs)
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };

            s_registered = UnitFactory::Register("Webspinner Shaman on Arachnarok Spider", factoryMethod);
        }
    }

    std::string WebspinnerShamanOnArachnarokSpider::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Lore of the Spiderfangs") {
            return ToString((Lore) parameter.intValue);
        }
        return ParameterValueToString(parameter);
    }

    int WebspinnerShamanOnArachnarokSpider::EnumStringToInt(const std::string &enumString) {
        Lore lore;
        if (FromString(enumString, lore)) {
            return (int) lore;
        }
        return 0;
    }

    Wounds WebspinnerShamanOnArachnarokSpider::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                            int woundRoll) const {
        // Spider Venom
        int threshold = inLightOfTheBadMoon() ? 5 : 6;
        if ((hitRoll >= threshold) && (weapon->name() == m_monstrousFangs.name())) {
            return {0, Dice::rollD3()};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int WebspinnerShamanOnArachnarokSpider::catchwebSpidershrine(const Unit *caster) {
        // Catchweb Spidershrine
        if (caster->hasKeyword(SPIDERFANG) && caster->hasKeyword(WIZARD) &&
            (caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= 12.0)) {
            return 1;
        }
        return 0;
    }

    int WebspinnerShamanOnArachnarokSpider::prophetOfTheSpiderGod(const Unit *unit) {
        // Prophet of the Spider God
        if (unit->hasKeyword(SPIDERFANG) && (unit->owningPlayer() == owningPlayer()) && (distanceTo(unit) <= 24.0)) {
            return 2;
        }
        return 0;
    }

    int WebspinnerShamanOnArachnarokSpider::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
