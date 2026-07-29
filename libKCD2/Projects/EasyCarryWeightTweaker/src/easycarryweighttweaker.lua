-- EasyCarryWeightTweaker -- in-game tuning of carry-weight / encumbrance RPG
-- params through the Mod Configuration Menu.
--
-- Requires (both KCSE plugins):
--   MCM              -- the menu + this Lua API (MCM.* global)
--   ConsoleRpgParam  -- exposes every RPG param as a kcse_rpgparam_* CVar
--
-- Sliders declare the game's REAL compiled-in default (read live off a
-- fresh install via the remote console -- KCD2/rpgparams_kcd2.txt has the
-- name/description/unit metadata dumped from S_RpgParamDefs, but not the
-- values themselves, which only exist in the live S_Constants store) and
-- separately report the LIVE cvar value via MCM.SetValue, so an X-reset in
-- the menu restores the game's own default rather than whatever was live.
-- Changes apply instantly via System.SetCVar; nothing is persisted across
-- restarts by design (RPG params reset to table values on game start).

local MOD_ID   = "easycarryweighttweaker"
local MOD_NAME = "Easy Carry Weight Tweaker"
local PREFIX   = "kcse_rpgparam_"

-- { id, label, min, max, step, default, suffix, tooltip [, pct=true] }
-- pct=true: the CVar stores a 0..1(+) fraction but is edited as a 0-100+
-- percentage (min/max/step/default are already in DISPLAY units); converted
-- on read (*100) and on write (/100). Tooltips are the binary's own param
-- descriptions (S_RpgParamDefs); or { category = "..." }.
local SETTINGS = {
	{ category = "Inventory Capacity" },
	{ "BaseInventoryCapacity", "Base Inventory Capacity", 0, 2000, 1, 90, " lb",
	  "base inventory capacity [lb]" },
	{ "MaxBaseInventoryCapacity", "Max Base Inventory Capacity", 0, 2000, 1, 600, " lb",
	  "Base maximum of inventory capacity without taking the archetype multiplier into account [lb]" },
	{ "StrengthToInventoryCapacity", "Strength to Inventory Capacity", 0, 100, 1, 10, " lb",
	  "derives the inventory capacity from the strength stat [lb/str]" },
	{ "EquippedWeightSubWithWellWornPerk", "Well Worn Equipped Weight Reduction", 0, 1, 0.05, 1 / 3, "",
	  "when player has Well Worn perk, weight of items is lowered when they are equipped; equippedWeight=standardWeight*(1.0 - EquippedWeightSubWithWellWornPerk)" },

	{ category = "Carrying Bodies & Sacks" },
	{ "CarriedBodyWeightCoef", "Carried Body Weight Coef", 0, 2, 0.05, 0.25, "",
	  "how much of the carried body weight is added to the carried weight of the carrier" },
	{ "CarriedCarriedWeightCoef", "Carried NPC's Carried Weight Coef", 0, 2, 0.05, 0.5, "",
	  "how much of the carried weight of the carried NPC is added to the carried weight of the carrier" },
	{ "CarriedBodyMaxStamConsumption", "Carried Body Max Stamina Drain", 0, 50, 1, 10, "",
	  "uses encumberence to interpolate from 0 to this" },
	{ "AddedDirtOnCarryItem", "Dirt Added on Carry Item", 0, 100, 1, 4, "%",
	  "Dirt added on player when picking or putting down an item through the carry item action (the sack carrying feature).",
	  pct = true },

	{ category = "Encumbrance Effects" },
	{ "HorseMountMaxRelativeEncumberance", "Horse Mount Max Encumbrance", 0, 300, 1, 100, "%",
	  "maximum allowed relative encumberance for player mounting a horse [%]", pct = true },
	{ "EncumberedToSpeedSurfaceCoef", "Encumbered Surface Speed Coef", 0, 2, 0.05, 0.3, "",
	  "Coef that controls terrain/surface influence in encumbered speed mod" },
	{ "EncumberanceForSecondaryModifiers", "Encumbrance for Secondary Modifiers", 0, 2, 0.05, 0.5, "",
	  "when the buff will activate the secondary group" },

	{ category = "Encumbrance XP" },
	{ "MinEncumberanceXPAwardDistance", "Min XP Award Distance", 0, 1000, 10, 50, " m",
	  "Distance that has to be covered while minimally encumbered to get StatXPStrAndVitPerDistanceEncumbered (linear interpolation with EncumberanceXPAwardDistanceMax)." },
	{ "MaxEncumberanceXPAwardDistance", "Max XP Award Distance", 0, 5000, 50, 10, " m",
	  "Distance that has to be covered while encumbered by at least double of carry weight to get StatXPStrAndVitPerDistanceEncumbered (linear interpolation with EncumberanceXPAwardDistanceMin)." },
	{ "StatXPStrAndVitPerDistanceEncumbered", "Str/Vit XP per Distance", 0, 2048, 1, 1024, "",
	  "Strength and vitality xp gain after walking EncumberanceXPAwardDistance meters while encumbered" },

	{ category = "Armor Load" },
	{ "ArmorLoadToSprint", "Armor Load to Sprint", 0, 2, 0.05, 1, "",
	  "how armor load coefficient affects sprint" },
	{ "ArmorLoadToRun", "Armor Load to Run Speed", 0, 2, 0.05, 0.5, "",
	  "how armor load coefficient affects running speed" },
	{ "ArmorLoadToJumpCost", "Armor Load to Jump Cost", 0, 5, 0.1, 1.8, "",
	  "how armor load coefficient affects the stamina jump cost (times base)" },
	{ "StrToEqwArmorLoad", "Strength to Armor Load", 0, 200, 1, 40, "",
	  "maximal gain on level cap, GDD: 17/218" },
	{ "AgiToEqwArmorLoad", "Agility to Armor Load", 0, 200, 1, 40, "",
	  "maximal gain on level cap, GDD: 17/218" },
	{ "VitToEqwArmorLoad", "Vitality to Armor Load", 0, 200, 1, 0, "",
	  "maximal gain on level cap, GDD: 17/218" },
	{ "ArmorLoadDiffToMax", "Armor Load Diff to Max", 0, 200, 1, 60, "",
	  "GDD: 17/218" },
	-- NOTE: range corrected from the old 0-500 guess to 0-1 (step 0.01, not
	-- the usual 1) -- the live default is 0.2, and HeavyLightArmorThreshold
	-- below is documented as living inside [-MaxArmorLoad, MaxArmorLoad], so
	-- this is a small coefficient, not a large "weight" number
	{ "MaxArmorLoad", "Max Armor Load", 0, 1, 0.01, 0.2, "",
	  "#ArmorLoad" },
	{ "HeavyLightArmorThreshold", "Heavy/Light Armor Threshold", -1, 1, 0.01, 0.05, "",
	  "should be between -MaxArmorLoad and MaxArmorLoad. #ArmorLoad" },

	{ category = "Combat" },
	{ "DodgeRelArmorLoadThreshold", "Dodge Armor Load Threshold", 0, 1, 0.01, 0.62, "",
	  "relative armor load threshold for dodge distance computation" },
	{ "DodgeRelDistanceThreshold", "Dodge Distance Threshold", 0, 1, 0.01, 0.7, "",
	  "relative distance threshold refering to DodgeRelArmorLoadThreshold" },
	{ "HeavyWeaponWeight", "Heavy Weapon Weight", 0, 500, 1, 99, " lb",
	  "used to deduce 'attack weight' [lb]" },
}

-- ids whose CVar stores a fraction but are edited as a percentage (see pct above)
local PCT = {}
for _, s in ipairs(SETTINGS) do
	if not s.category and s.pct then
		PCT[s[1]] = true
	end
end

local function BuildMenu()
	-- sentinel: ConsoleRpgParam registers its CVars at DataLoaded, well before
	-- the menu can open; missing here means the plugin isn't installed
	if System.GetCVar(PREFIX .. "BaseInventoryCapacity") == nil then
		System.LogAlways("[EasyCarryWeightTweaker] kcse_rpgparam_* CVars missing - is the ConsoleRpgParam KCSE plugin installed?")
		return
	end
	MCM.AddMod(MOD_ID, MOD_NAME)
	for _, s in ipairs(SETTINGS) do
		if s.category then
			MCM.AddCategory(MOD_ID, s.category, "")
		else
			local id, label, mn, mx, step, def, suffix, tip = s[1], s[2], s[3], s[4], s[5], s[6], s[7], s[8]
			local cur = System.GetCVar(PREFIX .. id)
			if cur ~= nil then
				if PCT[id] then
					cur = cur * 100
				end
				-- the declared range is the game's real default range, but
				-- another mod/console/a future PersistCVarEdits restore may
				-- have pushed the live value outside it -- grow to fit
				-- rather than clip it from view
				if cur > mx then
					mx = math.ceil(cur / step) * step
				end
				if cur < mn then
					mn = math.floor(cur / step) * step
				end
				MCM.AddSlider(MOD_ID, id, label, tip, mn, mx, step, def, suffix)
				MCM.SetValue(MOD_ID, id, cur)
			end
		end
	end
end

if MCM == nil then
	System.LogAlways("[EasyCarryWeightTweaker] MCM global missing - install the Mod Configuration Menu (KCSE plugin + mod)")
	return
end

MCM.RegisterBuildSettingsListener(BuildMenu)
MCM.RegisterValueChangeListener(MOD_ID, function(settingId, value)
	if PCT[settingId] then
		value = value / 100
	end
	System.SetCVar(PREFIX .. settingId, value)
end)

System.LogAlways("[EasyCarryWeightTweaker] MCM listeners registered")
