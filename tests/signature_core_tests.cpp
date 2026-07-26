#include "signatures/signature_core.hpp"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace
{
	constexpr uint64_t base = 0x180000000;

	kcd2::signatures::pe_image make_image(std::vector<uint8_t> bytes)
	{
		using kcd2::signatures::section_view;
		return kcd2::signatures::pe_image::from_test_image(
		    std::move(bytes),
		    {
		        section_view{".text", 0x100, 0x100, IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE},
		        section_view{".rdata", 0x300, 0x100, IMAGE_SCN_MEM_READ},
		    },
		    base);
	}

	void write_i32(std::vector<uint8_t> &bytes, size_t offset, int32_t value)
	{
		std::memcpy(bytes.data() + offset, &value, sizeof(value));
	}

	void write_u64(std::vector<uint8_t> &bytes, size_t offset, uint64_t value)
	{
		std::memcpy(bytes.data() + offset, &value, sizeof(value));
	}

	bool check(bool condition, std::string_view message)
	{
		if (!condition)
		{
			std::cerr << "FAILED: " << message << '\n';
		}
		return condition;
	}
}

int main()
{
	using namespace kcd2::signatures;
	bool success = true;
	std::string error;

	{
		std::vector<uint8_t> bytes(0x400);
		bytes[0x110] = 0xAA;
		bytes[0x111] = 0xBB;
		bytes[0x112] = 0xCC;
		auto image = make_image(std::move(bytes));
		auto matches = scan_pattern(image, "AA ? CC", error);
		success &= check(matches == std::vector<uint64_t>{0x110}, "unique wildcard scan");
		matches = scan_pattern(image, "AA BB DD", error);
		success &= check(matches.empty(), "missing scan");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		bytes[0x110] = bytes[0x120] = 0xAA;
		bytes[0x111] = bytes[0x121] = 0xBB;
		auto image = make_image(std::move(bytes));
		const auto matches = scan_pattern(image, "AA BB", error);
		success &= check(matches.size() == 2, "ambiguous scan reports every match");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		bytes[0x1FF] = 0xAA;
		bytes[0x200] = 0xBB;
		auto image = pe_image::from_test_image(
		    std::move(bytes),
		    {
		        section_view{".text1", 0x100, 0x100, IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE},
		        section_view{".text2", 0x200, 0x100, IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE},
		    },
		    base);
		const auto matches = scan_pattern(image, "AA BB", error);
		success &= check(matches.empty(), "pattern cannot cross PE section boundaries");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		bytes[0x110] = 0xE8;
		write_i32(bytes, 0x111, static_cast<int32_t>(0x180 - 0x115));
		auto image = make_image(std::move(bytes));
		const auto target = resolve_relative_call(image, 0x110, error);
		success &= check(target && *target == 0x180, "relative CALL target");
		const auto wrong = resolve_relative_call(image, 0x116, error);
		success &= check(!wrong, "wrong CALL opcode is rejected");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		bytes[0x3FC] = 0xE8;
		auto image = make_image(std::move(bytes));
		const auto target = resolve_relative_call(image, 0x3FC, error);
		success &= check(!target, "truncated CALL at the image boundary is rejected");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		bytes[0x110] = 0x48;
		bytes[0x111] = 0x8D;
		bytes[0x112] = 0x05;
		write_i32(bytes, 0x113, static_cast<int32_t>(0x300 - 0x117));
		auto image = make_image(std::move(bytes));
		const auto target = resolve_rip_relative_memory(image, 0x110, error);
		success &= check(target && *target == 0x300, "RIP-relative LEA target");
		const auto wrong = resolve_rip_relative_memory(image, 0x117, error);
		success &= check(!wrong, "instruction without RIP operand is rejected");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		// lea rax,[rip+vtable]; mov [rcx],rax; ret
		const uint8_t prefix[]{0x48, 0x8D, 0x05};
		std::memcpy(bytes.data() + 0x110, prefix, sizeof(prefix));
		write_i32(bytes, 0x113, static_cast<int32_t>(0x300 - 0x117));
		const uint8_t store[]{0x48, 0x89, 0x01, 0xC3};
		std::memcpy(bytes.data() + 0x117, store, sizeof(store));
		write_u64(bytes, 0x300, base + 0x180);
		auto image = make_image(std::move(bytes));
		const auto target =
		    resolve_constructor_vtable_assignment(image, 0x110, 0x20, error);
		success &= check(target && *target == 0x300, "semantic constructor VTable assignment");
	}

	{
		std::vector<uint8_t> bytes(0x400);
		// mov rax,[rip+global]; ret
		const uint8_t instruction[]{0x48, 0x8B, 0x05};
		std::memcpy(bytes.data() + 0x110, instruction, sizeof(instruction));
		write_i32(bytes, 0x113, static_cast<int32_t>(0x320 - 0x117));
		bytes[0x117] = 0xC3;
		auto image = make_image(std::move(bytes));
		const auto target = resolve_unique_rip_data_reference(image, 0x110, 0x20, error);
		success &= check(target && *target == 0x320, "semantic RIP data reference");
	}

	return success ? 0 : 1;
}
