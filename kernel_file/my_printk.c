#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>

asmlinkage void sys_my_printk(char __user *s, int len){
	char buf[100];
	//printk("call my_printk/n");
	copy_from_user(buf, s, len * sizeof(char));
	printk("%s", buf);
	return;
}

