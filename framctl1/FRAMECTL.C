/************************************************************************/
/*                                                                      */
/* Program name:   FrameCtl.C                                           */
/* Title:          A Picture Perfect Control                            */
/*                 OS/2 Magazine - GUI Corner                           */
/*                 October 1996 issue                                   */
/*                                                                      */
/* Author:         Mark Benge   IBM Corp.                               */
/*                 Matt Smith   Prominare Inc.                          */
/*                                                                      */
/* Description:    Illustrates how to add frame control extensions.     */
/*                                                                      */
/* DISCLAIMER OF WARRANTIES:                                            */
/* -------------------------                                            */
/* The following [enclosed] code is sample code created by IBM          */
/* Corporation and Prominare Inc.  This sample code is not part of any  */
/* standard IBM product and is provided to you solely for the purpose   */
/* of assisting you in the development of your applications.  The code  */
/* is provided "AS IS", without warranty of any kind.  Neither IBM nor  */
/* Prominare shall be liable for any damages arising out of your        */
/* use of the sample code, even if they have been advised of the        */
/* possibility of such damages.                                         */
/************************************************************************/

#define INCL_DOS                   /* Include OS/2 DOS Kernal           */
#define INCL_GPI                   /* Include OS/2 PM GPI Interface     */
#define INCL_WIN                   /* Include OS/2 PM Windows Interface */

#include <os2.h>
#include <string.h>
#include <stdio.h>

#include "framectl.h"

/************************************************************************/
/* This module contains example installable control that can be used    */
/* by any OS/2 2.x and Warp Presentation Manager application.  The      */
/* sample demonstrates the principles of adding frame control           */
/* extensions such that other extensions can be added using this as     */
/* a model.                                                             */
/*                                                                      */
/* Filename : FrameCtl.C                                                */
/* Version  : 1.00                                                      */
/* Created  : 1996-06-07                                                */
/* Revised  :                                                           */
/* Released :                                                           */
/*                                                                      */
/* Routines:  MRESULT EXPENTRY FrameWndProc( HWND hWnd, ULONG msg,      */
/*                                           MPARAM mp1, MPARAM mp2 )   */
/*            int main(int argc, char* argv[] )                         */
/*                                                                      */
/* Copyright ¸ International Business Machines Corp., 1991,1992,1993.   */
/* Copyright ¸ 1989-1993  Prominare Inc.  All Rights Reserved.          */
/*                                                                      */
/************************************************************************/

HWND hwndHelpBtn;                  /* Help Button Handle                */
HWND hwndCloseBtn;                 /* Close Button Handle               */
PFNWP DefFrameWndProc;             /* Frame Control Subclass Procedure  */

/************************************************************************/
/* Prototypes                                                           */
/************************************************************************/
int main( int, char* [] );
MRESULT EXPENTRY FrameWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

/* --- FrameWndProc ----------------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to process the messages for the frame      */
/*     control window.                                                  */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND   hWnd; = Window Handle                                     */
/*     ULONG  msg;  = PM Message                                        */
/*     MPARAM mp1;  = Message Parameter 1                               */
/*     MPARAM mp2;  = Message Parameter 2                               */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
MRESULT EXPENTRY FrameWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch ( msg )
  {
    case WM_FORMATFRAME :
    {
      /******************************************************************/
      /* Query the number of standard frame controls                    */
      /******************************************************************/
      ULONG ulStdCtlCount = (ULONG)DefFrameWndProc( hWnd, msg, mp1, mp2 );
      ULONG ulIdx = ulStdCtlCount;

      /******************************************************************/
      /* Access the SWP array that is passed to us                      */
      /******************************************************************/
      ULONG i;
      PSWP swpArr = (PSWP)mp1;

      for (i=0; i < ulStdCtlCount; i++)
      {
        if ( WinQueryWindowUShort( swpArr[i].hwnd, QWS_ID ) == FID_TITLEBAR )
        {
          /**************************************************************/
          /* Initialize the SWPs for our graphic button controls.       */
          /* Since the SWP array for the std frame controls is 0-based  */
          /* and occupy indexes 0 thru n-1 (where n is the total        */
          /* count), we start with index n for our graphic button       */
          /* controls.                                                  */
          /**************************************************************/
          /**************************************************************/
          /* These values are the same for both controls, so we init    */
          /* them all at once.  Please note that the width and height   */
          /* of our buttons is the same as the height of the title bar. */
          /**************************************************************/
          swpArr[ulIdx+1].fl = swpArr[ulIdx].fl =
                               SWP_MOVE | SWP_SIZE | SWP_NOADJUST;
          swpArr[ulIdx+1].cy = swpArr[ulIdx].cy = swpArr[i].cy;
          swpArr[ulIdx+1].cx = swpArr[ulIdx].cx = swpArr[i].cy;
          swpArr[ulIdx+1].y  = swpArr[ulIdx].y  = swpArr[i].y;
          swpArr[ulIdx+1].hwndInsertBehind = swpArr[ulIdx].hwndInsertBehind =
                                             HWND_TOP;

          /**************************************************************/
          /* Set the x-position for the close graphic button, and set   */
          /* its window handle.                                         */
          /**************************************************************/
          swpArr[ulIdx].x = swpArr[i].x;
          swpArr[ulIdx].hwnd = hwndCloseBtn;

          /**************************************************************/
          /* Calculate the x-position for the help graphic button, and  */
          /* set its window handle.                                     */
          /**************************************************************/
          swpArr[ulIdx+1].x = swpArr[i].x + swpArr[i].cx - swpArr[ulIdx+1].cx;
          swpArr[ulIdx+1].hwnd = hwndHelpBtn;

          /**************************************************************/
          /* Adjust the x-position and width of the title bar to        */
          /* accomodate our graphic button controls.                    */
          /**************************************************************/
          swpArr[i].x  += swpArr[i].cy;
          swpArr[i].cx -= (swpArr[ulIdx].cx * 2);
          break;
        }
      }

      /******************************************************************/
      /* Increment the number of frame controls to include our graphic  */
      /* button control.                                                */
      /******************************************************************/
      return( (MRESULT)(ulIdx + 2) );
    }

    case WM_QUERYFRAMECTLCOUNT :
      /******************************************************************/
      /* Query the standard frame controls count and increment to       */
      /* include our graphic button controls.                           */
      /******************************************************************/
      return( (MRESULT)((ULONG)DefFrameWndProc( hWnd, msg, mp1, mp2 ) + 2) );


    case WM_SYSCOMMAND :
      /******************************************************************/
      /* Process a click on the close graphic button.                   */
      /******************************************************************/
      if ( ((ULONG)mp1 == CLOSE_BUTTON_ID) &&
           (SHORT1FROMMP(mp2) == CMDSRC_PUSHBUTTON) )
      {
        /****************************************************************/
        /* Close the frame extensions sample application.               */
        /****************************************************************/
        WinPostMsg( hWnd, WM_CLOSE, 0, 0 );
      }
      else
      {
        return( DefFrameWndProc( hWnd, msg, mp1, mp2 ) );
      }
      break;

    case WM_HELP :
      /******************************************************************/
      /* Process a click on the help graphic button.                    */
      /******************************************************************/
      if ( (ULONG)mp1 == HELP_BUTTON_ID )
      {
        /****************************************************************/
        /* Invalidate the min/max control to repaint the area that the  */
        /* help button clips when you click on it.                      */
        /****************************************************************/
        WinInvalidateRect( WinWindowFromID( hWnd, FID_MINMAX ), NULL, FALSE );

        /****************************************************************/
        /* Display the help panel for this sample.                      */
        /****************************************************************/
        return( WinSendMsg( WinQueryHelpInstance( hWnd ),
                            HM_DISPLAY_HELP,
                            mp1,
                            HM_RESOURCEID ) );
      }

    default:
      return( DefFrameWndProc( hWnd, msg, mp1, mp2 ) );
  }

  return( (MRESULT)FALSE );
}


/* --- main ----------------------------------------------------------- */
/*     Main function.                                                   */
/* -------------------------------------------------------------------- */
int main( int argc, char* argv[] )
{
  HAB      hAB;                    /* Anchor Block Handle               */
  HMQ      hMQ;                    /* Message Queue Handle              */
  HWND     hwndClient;             /* Client Handle                     */
  HWND     hwndFrame;              /* Frame Handle                      */
  QMSG     qmsg;                   /* PM Message Queue Holder           */
  ULONG    flCreateFlags;          /* Window Creation Flags             */
  LONG     lClr;                   /* Colour Holder                     */
  HWND     hwndHelp;               /* Help Instance Handle              */
  HELPINIT helpInit;               /* Help Instance Structure           */

  /**********************************************************************/
  /* Initialize the program for PM, create the message queue, and set   */
  /* the codepage.                                                      */
  /**********************************************************************/
  hAB = WinInitialize( 0 );
  hMQ = WinCreateMsgQueue( hAB, 0 );
  WinSetCp( hMQ, 850 );

  /**********************************************************************/
  /* Create a standard frame window, specifying a static text control   */
  /* for the client area.                                               */
  /**********************************************************************/
  flCreateFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU | FCF_SIZEBORDER |
                  FCF_MINMAX | FCF_VERTSCROLL | FCF_HORZSCROLL |
                  FCF_SHELLPOSITION | FCF_TASKLIST;

  hwndFrame = WinCreateStdWindow( HWND_DESKTOP,
                                  0,
                                  &flCreateFlags,
                                  WC_STATIC,
                                  (PCSZ) "Frame Extensions",
                                  (WS_VISIBLE | SS_TEXT | DT_CENTER |
                                   DT_VCENTER),
                                  (HMODULE)0L,
                                  FRAME_WND_ID,
                                  &hwndClient );
  if ( !hwndFrame )
    return( TRUE );

  /**********************************************************************/
  /* Set the text for the static text control as well as the            */
  /* background color of the control.                                   */
  /**********************************************************************/
  lClr = SYSCLR_DIALOGBACKGROUND;
  WinSetPresParam(hwndClient, PP_BACKGROUNDCOLORINDEX, 4UL, &lClr);

  WinSetWindowText( hwndClient, (PCSZ) "Frame Extensions Test" );

  /**********************************************************************/
  /* Create and associate the help instance.                            */
  /**********************************************************************/
  helpInit.cb                       = sizeof(HELPINIT);
  helpInit.ulReturnCode             = 0;
  helpInit.pszTutorialName          = 0;
  helpInit.phtHelpTable             = (PHELPTABLE)MAKELONG(0, 0xffff);
  helpInit.hmodHelpTableModule      = 0;
  helpInit.hmodAccelActionBarModule = 0;
  helpInit.idAccelTable             = 0;
  helpInit.idActionBar              = 0;
  helpInit.pszHelpWindowTitle       = (PSZ) "Frame Extensions Sample";
  helpInit.fShowPanelId             = CMIC_HIDE_PANEL_ID;
  helpInit.pszHelpLibraryName       = (PSZ) "framectl.hlp";

  hwndHelp = WinCreateHelpInstance( hAB, &helpInit );
  if ( !hwndHelp )
    return( TRUE );

  if ( !WinAssociateHelpInstance( hwndHelp, hwndFrame ) )
    return( TRUE );

  /**********************************************************************/
  /* Create the close graphic button which we will use as a frame       */
  /* extension.                                                         */
  /**********************************************************************/
  hwndCloseBtn = WinCreateWindow( hwndFrame,
                                  WC_BUTTON,
                                  (PCSZ) "#200",
                                  (BS_BITMAP | BS_PUSHBUTTON | BS_NOBORDER |
                                   BS_NOPOINTERFOCUS | BS_AUTOSIZE |
                                   BS_SYSCOMMAND | WS_VISIBLE),
                                  0L, 0L, -1L, -1L,
                                  hwndFrame,
                                  HWND_TOP,
                                  CLOSE_BUTTON_ID,
                                  (PVOID)NULL,
                                  (PVOID)NULL );
  if ( !hwndCloseBtn )
    return( TRUE );

  /**********************************************************************/
  /* Create the help graphic button which we will use as a frame        */
  /* extension.                                                         */
  /**********************************************************************/
  hwndHelpBtn = WinCreateWindow( hwndFrame,
                                 WC_BUTTON,
                                 (PCSZ) "#300",
                                 (BS_BITMAP | BS_PUSHBUTTON | BS_NOBORDER |
                                  BS_NOPOINTERFOCUS | BS_AUTOSIZE |
                                  BS_HELP | WS_VISIBLE),
                                 0L, 0L, -1L, -1L,
                                 hwndFrame,
                                 HWND_TOP,
                                 HELP_BUTTON_ID,
                                 (PVOID)NULL,
                                 (PVOID)NULL );
  if ( !hwndHelpBtn )
    return( TRUE );

  /**********************************************************************/
  /* Subclass the frame control.  The subclass procedure is where we    */
  /* will add the frame extensions during processing of WM_FORMATFRAME. */
  /**********************************************************************/
  DefFrameWndProc = WinSubclassWindow( hwndFrame, (PFNWP)FrameWndProc );

  if ( !DefFrameWndProc )
    return( TRUE );

  /**********************************************************************/
  /* Indicate to PM that the frame needs updating.                      */
  /**********************************************************************/
  WinSendMsg( hwndFrame, WM_UPDATEFRAME, (MPARAM)~0, NULL );

  /**********************************************************************/
  /* Show the frame since we have finished formatting it.               */
  /**********************************************************************/
  WinShowWindow( hwndFrame, TRUE );

  /**********************************************************************/
  /* Message dispatch loop.                                             */
  /**********************************************************************/
  while ( WinGetMsg( hAB, &qmsg, (HWND)NULL, 0, 0 ) )
    WinDispatchMsg( hAB, &qmsg );

  /**********************************************************************/
  /* Termination processing                                             */
  /**********************************************************************/
  WinDestroyHelpInstance( hwndHelp );
  WinDestroyWindow( hwndFrame );
  WinDestroyMsgQueue( hMQ );
  WinTerminate( hAB );
  return( FALSE );
}
