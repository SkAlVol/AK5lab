// SPDX-License-Identifier: Dual BSD/GPL
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include "hello1.h"

MODULE_AUTHOR("Ilya Vashchishin <illa123fgh@gmail.com>");
MODULE_DESCRIPTION("Hello1 Kernel Module");
MODULE_LICENSE("Dual BSD/GPL");

static LIST_HEAD(hello_entries);

void print_hello(void)
{
	struct hello_entry *entry;

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return;

	entry->start_time = ktime_get();
	pr_info("Hello, world!\n");
	entry->end_time = ktime_get();

	list_add_tail(&entry->node, &hello_entries);
}
EXPORT_SYMBOL(print_hello);

static void cleanup_list(void)
{
	struct hello_entry *entry, *next;

	list_for_each_entry_safe(entry, next, &hello_entries, node) {
		pr_info("Entry duration: %lld ns\n",
			ktime_to_ns(entry->end_time) -
			ktime_to_ns(entry->start_time));
		list_del(&entry->node);
		kfree(entry);
	}
}

static int __init hello1_init(void)
{
	pr_info("Hello1 module loaded.\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	cleanup_list();
	pr_info("Hello1 module unloaded.\n");
}

module_init(hello1_init);
module_exit(hello1_exit);