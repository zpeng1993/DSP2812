/****************************************************************************/
/*  EXIT.C v3.01                                                    */
/*  Copyright (c) 1995-2001 Texas Instruments Incorporated                  */
/****************************************************************************/
#include <stdlib.h>
#include <_lock.h>

#define MAX_FUN   32

static int    at_exit_count = 0;
static void (*at_exit_fun[MAX_FUN])();
void        (*_cleanup_ptr)(void) = NULL;
void   _DATA_ACCESS      (*_dtors_ptr)(void)   = NULL;

/****************************************************************************/
/* EXIT() - NORMAL PROGRAM TERMINATION.                                     */
/****************************************************************************/
void exit(int status)        
{
   int i;

   /*-------------------------------------------------------------------*/
   /* MUST LOCK WHEN ACCESSING at_exit_count AND at_exit_fun.           */
   /*-------------------------------------------------------------------*/
   _lock();
   i = at_exit_count;

   /*-------------------------------------------------------------------*/
   /* HANDLE FUNCTIONS REGISTERED THROUGH atexit().                     */
   /*-------------------------------------------------------------------*/
   while (i) (*at_exit_fun[--i])(); 

   /*-------------------------------------------------------------------*/
   /* IF THERE ARE STATIC DESTRUCTORS, CALL THEM.                       */
   /*-------------------------------------------------------------------*/
   if (_dtors_ptr)  (*_dtors_ptr)();

   /*-------------------------------------------------------------------*/
   /* IF FILES ARE POSSIBLY OPEN, _CLEANUP_PTR() WILL BE SETUP TO CLOSE */
   /* THEM.                                                             */
   /*-------------------------------------------------------------------*/
   if (_cleanup_ptr)  (*_cleanup_ptr)();

   /*-------------------------------------------------------------------*/
   /* SET C$$EXIT LABEL SO THE DEBUGGER KNOWS WHEN THE C++ PROGRAM HAS  */
   /* COMPLETED.  THIS CAN BE REMOVED IF THE DEBUGGER IS NOT USED.      */
   /*-------------------------------------------------------------------*/
   __asm("        .global C$$EXIT");
   __asm("C$$EXIT: nop");

   for (;;);   /* CURRENTLY, THIS SPINS FOREVER */
}

/****************************************************************************/
/* ATEXIT - ATTEMPT TO REGISTER A FUNCTION FOR CALLING AT PROGRAM END       */
/****************************************************************************/
int atexit(void (*fun)())
{
   /*-------------------------------------------------------------------*/
   /* MUST LOCK WHEN ACCESSING at_exit_count AND at_exit_fun.           */
   /*-------------------------------------------------------------------*/
   _lock();
   if (at_exit_count >= MAX_FUN) {
      _unlock();
      return 1;
   }
   at_exit_fun[at_exit_count++] = fun;
   _unlock();
   return 0;
}

/****************************************************************************/
/* ABORT - ABNORMAL PROGRAM TERMINATION.  CURRENTLY JUST HALTS EXECUTION.   */
/****************************************************************************/
void abort(void)
{
   for (;;);   /* SPINS FOREVER */
}
