//It is not working yet


#include <linux/module.h>  // Needed by all modules
#include <linux/kernel.h>  // Needed for KERN_INFO
#include <linux/fs.h>      // Needed by filp
#include <asm/uaccess.h>   // Needed by segment descriptors

#define BUFFSIZE (128)

int init_module(void)
{
    struct file *f;
    char buffer[128];
    mm_segment_t fs;
    int i;

    for(i = 0;i<BUFFSIZE;i++)
    {
        buffer[i] = 0;
    }

    printk( KERN_INFO "Module was loaded\n");

    f = filp_open("/home/jipi/pso_project/KM_Read/in.txt", O_RDONLY,0);
    if(f == NULL)
        printk(KERN_ALERT "Error flip_open");
    else
    {
        fs = get_fs();
        set_fs(get_ds());
        f->f_op->read(f,buffer,128,&f->f_pos);
        set_fs(fs);
        printk(KERN_INFO "buffer: %s\n",buffer);
    }
    filp_close(f,NULL);
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "My module is unloaded\n");
}