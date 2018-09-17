/**
 *  procfs.c -  create a "file" in /proc
 *
 */

#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */
#include <asm/uaccess.h>	/* for copy_from_user */

#define PROCFS_MAX_SIZE		1024
#define PROCFS_NAME 		"buffer1k"

/**
 * This structure hold information about the /proc file
 *
 */
static struct proc_dir_entry *Our_Proc_File = NULL;

/**
 * The buffer used to store character for this module
 *
 */
static char procfs_buffer[PROCFS_MAX_SIZE] = "brown fox jumps over the lazy dog";
static unsigned int procfs_buffer_ptr;



int procfile_open(struct inode * inode_p, struct file *filep)
{
	return 0;
}

/** 
 * This function is called then the /proc file is read
 *
 */
ssize_t procfile_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	int size;
	
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", PROCFS_NAME);
	size = (len < procfs_buffer_ptr ? len : procfs_buffer_ptr);
	
	if (*offset > 0) {
		/* we have finished to read, return 0 */
		size  = 0;
	} else {
		/* fill the buffer, return the buffer size */
		memcpy(buffer, procfs_buffer, size);
	}

	return ret;
}
/**
 * This function is called with the /proc file is written
 *
 */
ssize_t procfile_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
	len = (len > PROCFS_MAX_SIZE? PROCFS_MAX_SIZE : len);
	
	/* write data to the buffer */
	if ( copy_from_user(procfs_buffer, buffer, len) ) {
		return -EFAULT;
	}
	
	return len;
}

static const struct file_operations procfs_file_ops = {
	.read = procfile_read,
	.write = procfile_write,
	.open = procfile_open,
	.release = NULL
};

/**
 *This function is called when the module is loaded
 *
 */
int procfs_init_module(void)
{
	/* create the /proc file */
	Our_Proc_File = proc_create(PROCFS_NAME,0644, NULL, &procfs_file_ops );
	
	if (Our_Proc_File == NULL) {
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
			PROCFS_NAME);
		return -ENOMEM;
	}

	procfs_buffer_ptr = strlen(procfs_buffer);

	printk(KERN_INFO "/proc/%s created\n", PROCFS_NAME);	
	return 0;	/* everything is ok */
}
EXPORT_SYMBOL(procfs_init_module);

/**
 *This function is called when the module is unloaded
 *
 */
void procfs_cleanup_module(void)
{
	if(Our_Proc_File)
	{
		proc_remove(Our_Proc_File);
		Our_Proc_File = NULL;
	}
	printk(KERN_INFO "/proc/%s removed\n", PROCFS_NAME);
}
EXPORT_SYMBOL(procfs_cleanup_module);
