#include <linux/kernel.h>

int called_by_module_main(void)
{
	printk(KERN_CRIT "David: this is the module printed by a function\n");
	return 0;
}

EXPORT_SYMBOL(called_by_module_main);
