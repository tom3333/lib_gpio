/* ****************************************************************************************************
 * lib_gpio_STM32F1.c within the following project: lib_gpio_STM32F1
 *
 *  compiler:   GNU Tools ARM Embedded (4.7.201xqx)
 *  target:     Cortex Mx
 *  author:		Tom
 * ****************************************************************************************************/

/* ****************************************************************************************************/

/* *******************************************************************
 * includes
 * ******************************************************************/

/* c-runtime */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>

/* system */
#include <unistd.h>

/* own libs */
#include <lib_isr.h>
#include <lib_log.h>
#include <lib_convention__errno.h>

/* project */
#include <gpio_types.h>
#include "gpio_event.h"

/* *******************************************************************
 * defines
 * ******************************************************************/
#define GPIO_EVENT__MODULE_NAME		"GPIO_EVT"

 /* ******************************************************************
  * (static) variables declarations
  * *****************************************************************/

/* *******************************************************************
 * Static Function Prototypes
 * ******************************************************************/
IRQn_Type gpio_event__get_irq_type(unsigned int _pin);
static void gpio_event__isr_handler(IRQn_Type _isr_vector, unsigned int _vector, void *_arg);

/* *******************************************************************
 * GLOBAL FUNCTION DEFINITIONS
 * ******************************************************************/

/* *******************************************************************
 * \brief Init of the gpio event handling
 * ---------
 * \remark The GPIO init routine is called by each event driven pin
 * ---------
 * \return	if successfully, < '0' if not successful
 * ******************************************************************/
int gpio_event__init()
{
	return EOK;
}

/* *******************************************************************
 * \brief Cleanup of the gpio event handling
 * ---------
 * \remark The GPIO cleanup routine is called by each event driven pin
 * ---------
 * \return	if successfully, < '0' if not successful
 * ******************************************************************/
int gpio_event__cleanup()
{
	return EOK;
}

/* *******************************************************************
 * \brief current pin is registerd at the event handling
 * ---------
 * \return	if successfully, < '0' if not successful
 * ******************************************************************/
int gpio_event__register(gpio_hdl_t _hdl)
{
  int ret;
  IRQn_Type irq_vector;
  irq_vector = gpio_event__get_irq_type(_hdl->access->pinNumber);
  if (irq_vector == NonMaskableInt_IRQn ) {
    return -ESTD_NOENT;    
  }

  ret = lib_isr__attach(&_hdl->access->event_hdl,irq_vector,&gpio_event__isr_handler,(void*)_hdl);
	return ret;
}

/* *******************************************************************
 * \brief current pin is unregisterd from the event handling
 * ---------
 * \return	if successfully, < '0' if not successful
 * ******************************************************************/
int gpio_event__unregister(gpio_hdl_t _hdl) 
{
  int ret;
  ret = lib_isr__detach(&_hdl->access->event_hdl);
  return ret;
}


/* *******************************************************************
 * Static Functions
 * ******************************************************************/

/* *******************************************************************
 * \brief	Translation between pin and isr vector
 * ---------
 * \remark	Enable of the corresponding clock for an GPIO module
 * ---------
 * \param	_pin Pin to request corresponding IRQn_Type
 * ---------
 * \return	'0', if successful, < '0' if not successful
 * ******************************************************************/
IRQn_Type gpio_event__get_irq_type(unsigned int _pin)
{
	switch (_pin)
	{
		case 0 	: return EXTI0_IRQn;
		case 1 	: return EXTI1_IRQn;
		case 2 	: return EXTI2_IRQn;
		case 3 	: return EXTI3_IRQn;
		case 4 	: return EXTI4_IRQn;
		case 5 	: return EXTI9_5_IRQn;
		case 6  : return EXTI9_5_IRQn;
		case 7  : return EXTI9_5_IRQn;
		case 8  : return EXTI9_5_IRQn;
		case 9  : return EXTI9_5_IRQn;
		case 10 : return EXTI15_10_IRQn;
		case 11 : return EXTI15_10_IRQn;
		case 12 : return EXTI15_10_IRQn;
		case 13 : return EXTI15_10_IRQn;
		case 14 : return EXTI15_10_IRQn;
		case 15 : return EXTI15_10_IRQn;
		default: return NonMaskableInt_IRQn;
	}
}

static void gpio_event__isr_handler(IRQn_Type _isr_vector, unsigned int _vector, void *_arg)
{
	gpio_hdl_t arg;
	arg = (gpio_hdl_t)_arg;
	if (arg->event_handler != NULL)
	{
		(*arg->event_handler)(arg->event_arg);
	}
}


