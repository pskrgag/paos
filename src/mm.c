#include <paos/mm.h>
#include <paos/string.h>
#include <paos/videobuffer.h>

#define MEMMAP_SIZE_ADDR	0x9000

enum mem_types {
	UNDEFINED,
	RAM,
	RESERVED,
	ACPI_REC,
	ACPI_NVS,
	BAD,
};

struct BIOS_memmap {
	uint64_t base_addr;
	uint64_t length;
	uint32_t type;
} __attribute__((packed));

struct free_memory_region {
	uint64_t base_addr;
	uint64_t size;
};

static struct free_memory_region free_memory_region[20];

static const char *type_to_string(uint32_t type)
{
	static char buffer[30];

	memset(buffer, 0, sizeof(buffer));

	switch (type)
	{
	case UNDEFINED:
		strcpy(buffer, "undefined");
		break;
	case RAM:
		strcpy(buffer, "ram");
		break;
	case RESERVED:
		strcpy(buffer, "reserved");
		break;
	case ACPI_REC:
		strcpy(buffer, "acpi rec");
		break;
	case ACPI_NVS:
		strcpy(buffer, "acpi nvs");
		break;
	case BAD:
		strcpy(buffer, "bad");
		break;
	default:
		break;
	}

	return buffer;
}

static inline const struct BIOS_memmap const *get_memory_map(uint32_t *size)
{
	*size = *((uint32_t *) 0x9000);

	return (const struct BIOS_memmap *) (MEMMAP_SIZE_ADDR + 8);
}

void init_memory(void)
{
	uint32_t size, i;
	const struct BIOS_memmap const *map = get_memory_map(&size);
	struct free_memory_region *free_mem = free_memory_region;

	for (i = 0; i < size; ++i) {
		if (map[i].type == RAM) {
			free_mem->base_addr = map[i].base_addr;
			free_mem->size = map[i].length;
		
			++free_mem;
		}

		printk("memregion %s 0x%x 0x%x\n", type_to_string(map[i].type), map[i].base_addr, map[i].base_addr + map[i].length);
	}
}
