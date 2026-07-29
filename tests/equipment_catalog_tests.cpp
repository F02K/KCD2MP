#include "npc/equipment_catalog.hpp"

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include <zip.h>

namespace
{
	struct temporary_directory
	{
		temporary_directory()
		{
			path = std::filesystem::temp_directory_path()
			    / ("kcd2mp-equipment-"
			       + std::to_string(
			           std::chrono::steady_clock::now()
			               .time_since_epoch()
			               .count()));
			std::filesystem::create_directories(path);
		}

		~temporary_directory()
		{
			std::error_code error;
			std::filesystem::remove_all(path, error);
		}

		std::filesystem::path path;
	};

	void write_pak(
	    const std::filesystem::path &path,
	    const std::vector<kcd2mp::npc::catalog_document> &documents)
	{
		std::filesystem::create_directories(path.parent_path());
		auto *archive = zip_open(path.string().c_str(), 6, 'w');
		assert(archive);
		for (const auto &document : documents)
		{
			assert(zip_entry_open(archive, document.source.c_str()) == 0);
			assert(zip_entry_write(
			           archive,
			           document.xml.data(),
			           document.xml.size())
			    == 0);
			assert(zip_entry_close(archive) == 0);
		}
		zip_close(archive);
	}
}

int main()
{
	using namespace kcd2mp::npc;
	const std::vector<catalog_document> documents{
	    {
	        "Libs/Tables/item/equipment_slot.xml",
	        R"xml(<database><EquipmentSlots>
<EquipmentSlot Name="body_cloth_padded" BodyLayerTypeId="2" ArmorTypes="GambesonShort"/>
<EquipmentSlot Name="body_plate" BodyLayerTypeId="5" ArmorTypes="Cuirass"/>
<EquipmentSlot Name="boot" BodyLayerTypeId="3" ArmorTypes="BootsKnee"/>
<EquipmentSlot Name="horse_body" BodyLayerTypeId="1" ArmorTypes="HorseBody"/>
</EquipmentSlots></database>)xml"},
	    {
	        "Libs/Tables/item/armor_type.xml",
	        R"xml(<database><armor_types>
<armor_type Id="1" Name="GambesonShort"/>
<armor_type Id="2" Name="Cuirass"/>
<armor_type Id="3" Name="BootsKnee"/>
<armor_type Id="4" Name="HorseBody"/>
</armor_types></database>)xml"},
	    {
	        "Libs/Tables/item/weapon_class.xml",
	        R"xml(<database><WeaponClasss>
<WeaponClass id="1" name="sword" equip_slot="PrimaryMainHand" is_twohanded="false"/>
<WeaponClass id="2" name="bow" equip_slot="SecondaryMainHand" is_twohanded="true"/>
</WeaponClasss></database>)xml"},
	    {
	        "Libs/Tables/item/item.xml",
	        R"xml(<database><ItemClasses>
<Armor Id="11111111-1111-1111-1111-111111111111" Clothing="GambesonShort"/>
<Armor Id="22222222-2222-2222-2222-222222222222" Clothing="Cuirass"/>
<Armor Id="33333333-3333-3333-3333-333333333333" Clothing="BootsKnee"/>
<Armor Id="44444444-4444-4444-4444-444444444444" Clothing="HorseBody"/>
<MeleeWeapon Id="55555555-5555-5555-5555-555555555555" Class="1"/>
<MissileWeapon Id="66666666-6666-6666-6666-666666666666" Class="2"/>
<Food Id="77777777-7777-7777-7777-777777777777"/>
</ItemClasses></database>)xml"}};

	equipment_catalog catalog;
	std::string error;
	assert(catalog.load_documents(documents, error));
	assert(error.empty());
	assert(catalog.size() == 5);

	const auto *padded =
	    catalog.find("11111111-1111-1111-1111-111111111111");
	assert(padded);
	assert(padded->equipped_slot == "body_cloth_padded");
	assert(padded->layer == 2);
	assert(padded->weapon == weapon_class::none);

	const auto *plate =
	    catalog.find("22222222-2222-2222-2222-222222222222");
	assert(plate && plate->layer > padded->layer);

	const auto *sword =
	    catalog.find("55555555-5555-5555-5555-555555555555");
	assert(sword);
	assert(sword->equipped_slot == "PrimaryMainHand");
	assert(sword->weapon == weapon_class::one_handed);

	const auto *bow =
	    catalog.find("66666666-6666-6666-6666-666666666666");
	assert(bow);
	assert(bow->equipped_slot == "SecondaryMainHand");
	assert(bow->weapon == weapon_class::bow);

	assert(!catalog.find("44444444-4444-4444-4444-444444444444"));
	assert(!catalog.find("77777777-7777-7777-7777-777777777777"));

	temporary_directory installation;
	write_pak(installation.path / "Data" / "Tables.pak", documents);
	const auto mod = installation.path / "mods" / "visible_items";
	write_pak(
	    mod / "data" / "visible_items.pak",
	    {{
	        "Libs/Tables/item/item__visible_items.xml",
	        R"xml(<database><ItemClasses>
<Armor Id="88888888-8888-4888-8888-888888888888" Clothing="GambesonShort"/>
</ItemClasses></database>)xml"}});
	{
		std::ofstream log(installation.path / "kcd.log");
		log << "[Mod] Opening paks in mods/visible_items/data/*.pak\n";
	}
	equipment_catalog active_catalog;
	assert(active_catalog.load_game_install(installation.path, error));
	assert(active_catalog.size() == 6);
	const auto *modded =
	    active_catalog.find("88888888-8888-4888-8888-888888888888");
	assert(modded);
	assert(modded->equipped_slot == "body_cloth_padded");
	return 0;
}
