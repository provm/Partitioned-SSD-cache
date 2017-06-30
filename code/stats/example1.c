#include <linux/init.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kfifo.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/stat.h>

//#include "/home/shyamli/flashcache-master/src/flashcache.h"
static int val=1000;
extern int (* flashcache_change_container_size_ptr) (int, int);
extern int flashcache_get_container_status(int);
static ssize_t simpleproc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	//int ret;
	//printk("%d %d %s\n",val,count,buf);
	// if( copy_from_user (&val, buf, count) ){
	// 	return -1;
	// }
	// printk("%d\n",val);
	int x=0;
	int i;
	int containerID = 0;
	containerID = buf[0] - '0';
	for(i=1; i<count-1;i++) {
		
		x =  (buf[i]-'0') + 10 *x;
	}
	printk("%d %d\n",containerID, x);
	//flashcache_change_container_size(containerID, x);
	if(flashcache_change_container_size_ptr != NULL)
		(*flashcache_change_container_size_ptr) (containerID, x);
	return count;
	
//	flashcache_get_container_status();
	return 1;
}

static ssize_t simpleproc_read(struct file *file, char __user *buf,
						size_t count, loff_t *ppos)
{
	//int ret;
	//copy_to_user(buf, &val, count);	
	//flashcache_change_container_size(10);
	flashcache_get_container_status(0);
	printk("%d\n",val);	
	return 0;
}
static const struct file_operations fops1 = {
	.owner		= THIS_MODULE,
	.read		= simpleproc_read,
	.write		= simpleproc_write,
};
static int __init procfile_init(void)
{
	//int ret;
	proc_create("simpleproc", 0777, NULL, &fops1);
	return 0;
}

static void __exit procfile_exit(void)
{
	remove_proc_entry("simpleproc", NULL);
	printk(KERN_INFO "module removed.\n");
}

module_init(procfile_init);
module_exit(procfile_exit);
MODULE_LICENSE("GPL");
