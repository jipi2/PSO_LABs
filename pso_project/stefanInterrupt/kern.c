#include <linux/kernel.h>  
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");

irqreturn_t keyboard_isr(int irq,void * dev_id , struct pt_regs * regs){
    
    unsigned char scancode;
    scancode = inb(0x60);

    printk(KERN_INFO "keyboard interrupt %d \n", scancode);

    return IRQ_HANDLED;
}

static int __init keyboard_isr_init(void){
    int val = 0;

    val = request_irq(1,
                    (irq_handler_t)keyboard_isr,
                    IRQF_SHARED,"keyboard_irq_example",
                    (void*)(keyboard_isr) );

    if(val == 0 )
        printk(KERN_INFO "ISR Registered\n");
    
    return 0;
}

static void __exit keyboard_isr_exit(void){
    free_irq(1, (void*)(keyboard_isr));
    printk(KERN_INFO "ISR FREED\n");

}

module_init(keyboard_isr_init);
module_exit(keyboard_isr_exit);