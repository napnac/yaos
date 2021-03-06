#include <stdint.h>

#include <kernel/interrupts.h>
#include <kernel/io.h>
#include <kernel/timer.h>

/* We need to use the 'volatile' keyword to prevent
   the compiler from optimizing the variable */
volatile uint32_t timer_ticks = 0;

void timer_wait(const uint32_t ticks)
{
   uint32_t total_ticks = timer_ticks + ticks;
   while(timer_ticks < total_ticks);
}

void timer_handler(Stack *registers)
{
   /* Unused parameter (avoid a warning) */
   (void)(registers);

   ++timer_ticks;
}

static void timer_phase(const uint32_t hz)
{
   uint32_t divisor = 1193180 / hz;

   /* Bit definitions for command register

      0: BCD
      1-3: Mode
      4-5: RW (read/write)
      6-7: CNTR (counter)

   */
   write_port(COMMAND_REG, 0x36);
   /* Set the low byte of 'divisor' */
   write_port(DATA_REG1, divisor & 0xFF);
   /* Set the high byte of 'divisor' */
   write_port(DATA_REG1, divisor >> 8);
}

void timer_install(void)
{
   irq_install_handler(0, timer_handler);

   /* 100Hz */
   timer_phase(100);
}
