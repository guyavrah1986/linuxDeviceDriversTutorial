#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <linux/uaccess.h>	/* For copy_to_user */
#include <linux/timekeeping.h>

#define procfs_entry_name "sampleModuleForPorcFs"

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple kernel module with an /proc fs entry");
MODULE_AUTHOR("Guy Avraham");

struct timespec curr_tm;

/*
* This structure hold information about the /proc file
*/
struct proc_dir_entry *proc_file_entry;

/* Return the time stamp of the "read operation" */

ssize_t procfile_read (struct file *file, char __user *buffer, size_t count, loff_t *ppos)
{
	int ret = 0, cnt;
	char tmp_buff [100] = {0};
	getnstimeofday(&curr_tm);
	if (snprintf(tmp_buff, 100, "%s read at:%.2lu:%.2lu:%.2lu:%.6lu \n", procfs_entry_name,
                   (curr_tm.tv_sec / 3600) % (24), (curr_tm.tv_sec / 60) % (60), 
                   (curr_tm.tv_sec % 60), (curr_tm.tv_nsec / 1000)));

	if (*ppos > 0) {
		/* we have finished to read, return 0 */
		ret  = 0;
	} else if (tmp_buff[0]) {
		/* fill the buffer, return the buffer size */
		cnt = sizeof(tmp_buff);
		if (count < cnt)
			cnt = count;
		copy_to_user(buffer, tmp_buff, cnt);
		*ppos += cnt;
		ret= cnt;
	}

	return ret;
}

/* File operations structure */

static const struct file_operations proc_file_fops = {
 .owner = THIS_MODULE,
 .read  = procfile_read,
};

/* Init function */

int init_module()
{
	proc_file_entry = proc_create(procfs_entry_name, 0644, NULL, &proc_file_fops);  // New in Kernel 3.10
	
	if (proc_file_entry == NULL) {
		remove_proc_entry(procfs_entry_name, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
		       procfs_entry_name);
		return -ENOMEM;
	}

	getnstimeofday(&curr_tm);
	printk(KERN_INFO "/proc/%s created at: %.2lu:%.2lu:%.2lu:%.6lu \n", procfs_entry_name,
                   (curr_tm.tv_sec / 3600) % (24), (curr_tm.tv_sec / 60) % (60), 
                   (curr_tm.tv_sec % 60), (curr_tm.tv_nsec / 1000) );
	
	return 0;
}

void cleanup_module()
{
	remove_proc_entry(procfs_entry_name, NULL);
	printk(KERN_INFO "/proc/%s removed\n", procfs_entry_name);
}

