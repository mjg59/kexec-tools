#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <elf.h>
#include <boot/elf_boot.h>
#include <ip_checksum.h>
#include <x86/x86-linux.h>
#include "../../kexec.h"
#include "../../kexec-elf.h"
#include "../../kexec-syscall.h"
#include "kexec-x86.h"
#include "x86-linux-setup.h"
#include "crashdump-x86.h"
#include <arch/options.h>

int dummy_probe(const char *buf, off_t len)
{
	return 0;
}

void dummy_usage(void)
{
	return;
}

int dummy_load(int argc, char **argv, const char *buf, off_t len,
	       struct kexec_info *info)
{
	long long address;
	long long value;
	int opt;

	static const struct option options[] = {
		KEXEC_ARCH_OPTIONS
		{ "address",	1, 0, OPT_ADDRESS },
		{ "value",	1, 0, OPT_VALUE },
		{ 0,		0, 0, 0 },
	};
	static const char short_options[] = "";

	while ((opt = getopt_long(argc, argv, short_options, options, 0)) != -1) {
		switch(opt) {
		default:
			/* Ignore core options */
			if (opt < OPT_ARCH_MAX) {
				break;
			}
		case OPT_ADDRESS:
			address = strtoll(optarg, NULL, 0);
			break;
		case OPT_VALUE:
			value = strtoll(optarg, NULL, 0); 
			break;	
		}
	}

	elf_rel_build_load(info, &info->rhdr, dummy_purgatory,
			   dummy_purgatory_size, 0x3000, 640*1024, -1, 0);
	elf_rel_set_symbol(&info->rhdr, "hack_address", &address, sizeof(address));
	elf_rel_set_symbol(&info->rhdr, "hack_value", &value, sizeof(value));
	return 0;
}

