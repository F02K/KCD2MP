#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_PerkStat -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum, 103 values.
// -----------------------------------------------
// Modifier-stat keys used by perks and buffs. The code registry is built by sub_1801C2E30;
// C_RPGUtils vtable+0x70 (sub_180BE2B8C) converts these string codes to ids. The native
// accumulator sub_180649F1C walks C_Soul::m_sortedModifierLists[2] (C_Soul+0xB8), compares
// each active modifier node's target id at +0x28, and applies matching modifiers.
//
// Human meanings were recovered from retail Tables.pak buff_params users and matching
// English_xml.pak perk/buff descriptions. Confidence tags:
//   [V] verified by a localized description or directly named live-data use
//   [S] strong: isolated/paired use identifies the quantity consistently
//   [I] inferred: domain is known, but the exact engine quantity is not pinned
//   [?] unknown: no naming-grade evidence; no meaning is fabricated
//
// IMPORTANT: this is distinct from E_DerivedStat's 218-value native evaluator domain.
// Soul:GetDerivedStat(string) contains a verified converter/evaluator mismatch: handler
// sub_180BE3834 converts the string through this registry, then passes the resulting ordinal
// to sub_180648B18, whose id domain is E_DerivedStat. Do not cast E_PerkStat to E_DerivedStat.

namespace wh::rpgmodule {

enum class E_PerkStat : uint32_t {
    ade = 0,    // [?] UNKNOWN -- only turtle-skin/tournament data uses it
    uat = 1,    // [S] Unarmed attack damage
    wat = 2,    // [V] Weapon attack damage
    wac = 3,    // [V] Weapon/combat-action stamina cost
    ahm = 4,    // [V] Mounted attack damage
    rtm = 5,    // [?] UNKNOWN -- mount-related; only the Warhorse perk uses it
    act = 6,    // [?] UNKNOWN -- no retail buff/perk user
    asp = 7,    // [S] Attack-success probability
    pbs = 8,    // [V] Perfect-block/master-strike chance
    cli = 9,    // [V] Clinch-overpower chance
    hlh = 10,   // [V] Incoming health-damage multiplier
    slh = 11,   // [S] Incoming stamina-damage multiplier
    sls = 12,   // [V] Shield-block stamina cost
    dig = 13,   // [V] Digestion/hunger rate
    exh = 14,   // [V] Exhaustion/energy-drain rate
    hod = 15,   // [V] Hangover recovery time (lower clears it faster)
    psp = 16,   // [V] Healing-potion healing speed
    lcs = 17,   // [S] Aim sway/hand-shake magnitude
    srg = 18,   // [V] Stamina-regeneration rate
    sco = 19,   // [?] UNKNOWN -- stamina/action-cost domain, exact quantity unpinned
    srb = 20,   // [?] UNKNOWN -- stamina-state cap domain, exact quantity unpinned
    sra = 21,   // [S] In-combat/backswing stamina regeneration
    ard = 22,   // [V] Stamina drain while aiming
    skp = 23,   // [S] Stealth-kill success probability
    rst = 24,   // [V] Resistance to health-derived maximum-stamina loss
    pdp = 25,   // [V] Resistance to being pulled from a horse
    eep = 26,   // [S] Fast-travel encounter-escape chance
    lpd = 27,   // [S] Lockpicking difficulty
    lpn = 28,   // [S] Lockpicking noise
    ain = 29,   // [V] Chance to inflict bleeding
    hin = 30,   // [V] Incoming bleeding susceptibility
    fae = 31,   // [S] First-aid/bandage effectiveness
    dee = 32,   // [V] Armour-condition damage dealt
    dew = 33,   // [V] Enemy weapon/shield condition damage dealt
    osb = 34,   // [V] Opponent stamina cost when blocking your attacks
    dsl = 35,   // [V] Combat-dodge chance/effectiveness
    cos = 36,   // [?] UNKNOWN -- defensive-reaction probability, exact action unpinned
    pac = 37,   // [S] Poison-application chance
    lpb = 38,   // [V] Lockpick break/snap chance
    res = 39,   // [I] Crime-search/recognition responsiveness
    ors = 40,   // [S] AI observability/detectability
    ptp = 41,   // [?] UNKNOWN -- tremor/combat-direction probability domain
    shr = 42,   // [V] Health regeneration while sleeping
    ser = 43,   // [V] Energy regeneration while sleeping
    pim = 44,   // [S] Poison immunity/cure control
    ald = 45,   // [V] Alcoholism-effect multiplier
    aam = 46,   // [I] Alcohol/potion metabolism or effect-duration multiplier
    uab = 47,   // [V] Damage bonus against an unaware target
    fhm = 48,   // [V] Food-healing multiplier
    fne = 49,   // [V] Crime-fine amount multiplier
    dth = 50,   // [V] Ranged chest/heart-hit damage multiplier
    dtd = 51,   // [?] UNKNOWN -- no retail buff/perk user
    tdh = 52,   // [V] Head-hit damage taken/helmet protection multiplier
    adc = 53,   // [?] UNKNOWN -- no retail buff/perk user
    sla = 54,   // [V] Slashing-damage multiplier
    sta = 55,   // [V] Stabbing/piercing-damage multiplier
    ssa = 56,   // [V] Smashing/blunt-damage multiplier
    wsr = 57,   // [?] UNKNOWN -- no retail buff/perk user
    war = 58,   // [?] UNKNOWN -- no retail buff/perk user
    wrs = 59,   // [?] UNKNOWN -- no retail buff/perk user
    wra = 60,   // [?] UNKNOWN -- no retail buff/perk user
    efc = 61,   // [S] Fast-travel event reaction/avoidance chance
    dsc = 62,   // [V] Dodge stamina cost
    wde = 63,   // [V] Own weapon-defence effectiveness
    ewd = 64,   // [V] Enemy weapon/shield-defence effectiveness
    scv = 65,   // [V] Skill-check value bonus
    scx = 66,   // [V] Skill-check experience multiplier
    skv = 67,   // [S] Dialogue skill-check effectiveness
    cat = 68,   // [V] Clinch-attack damage multiplier
    acm = 69,   // [V] Charged-attack damage multiplier
    rat = 70,   // [V] Ranged-attack damage multiplier
    rls = 71,   // [V] Ranged reload/cocking speed
    ptw = 72,   // [V] Aiming time-warp/slow-motion factor
    ala = 73,   // [V] Extra alchemy products brewed
    apq = 74,   // [V] Alchemy-process quality/tolerance
    bid = 75,   // [V] Blocking-item durability-loss multiplier
    fob = 76,   // [V] Feint effectiveness/opponent response difficulty
    ode = 77,   // [?] UNKNOWN -- no retail buff/perk user
    dmd = 78,   // [V] Mutt obedience-decay multiplier
    dmh = 79,   // [V] Damage taken by Mutt
    drn = 80,   // [V] Mutt return time after fleeing
    bqc = 81,   // [S] Bed-quality contribution/bonus
    bqy = 82,   // [V] Bed/sleep-quality multiplier
    ifc = 83,   // [V] Time for an item's stolen flag to clear
    pwc = 84,   // [V] Poisoned-weapon coating longevity/hit count
    paa = 85,   // [V] Poisoned-arrow count per poison dose
    itd = 86,   // [V] Item/food spoilage rate
    rkm = 87,   // [V] Repair-kit material consumption
    rhm = 88,   // [V] Horse-rearing fear/morale effect
    phv = 89,   // [V] Healing-potion healing value/effectiveness
    bnv = 90,   // [V] Beer nourishment value
    pnv = 91,   // [V] Potion nourishment value
    fnv = 92,   // [V] Food nourishment value
    aps = 93,   // [V] Arrow/bolt ballistic speed, range, and penetration
    fbm = 94,   // [V] Foliage/bush movement-noise multiplier
    tdm = 95,   // [S] Criminal-brand disappearance/decay rate
    cps = 96,   // [V] Carrying-capacity gain per Strength level
    spv = 97,   // [V] Stamina-point gain per Vitality level
    pct = 98,   // [V] Pickpocket collection-time gain rate
    nhp = 99,   // [I] Secondary hangover duration/severity multiplier
    nrb = 100,  // [S] NPC crime-search/recognition bonus
    btd = 101,  // [S] Visible bleeding/blood-trail persistence
    opd = 102,  // [?] UNKNOWN -- no retail buff/perk user

    Count = 103, // last registered id + 1 (SYNTHETIC enumerator)
};

}  // namespace wh::rpgmodule
